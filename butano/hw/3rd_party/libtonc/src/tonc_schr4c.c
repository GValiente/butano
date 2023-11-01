//
// 4bpp tiled surfaces, column-major (chr4c).
//
//! \file tonc_schr4c.c
//! \author J Vijn
//! \date 20080427 - 20080503
//! \todo Code consistency.
//
/* === NOTES ===
	Tested: plot, hline, vline, line, rect, frame
*/

#include "tonc_surface.h"
#include "tonc_video.h"
#include "tonc_math.h"

#include "tonc_warn.h"

// --------------------------------------------------------------------
// PROTOTYPES 
// --------------------------------------------------------------------


INLINE u32 chr4_lmask(uint left);
INLINE u32 chr4_rmask(uint right);
INLINE void chr4c_plot(int x, int y, u32 clr, void *dstBase, u32 dstP);
INLINE void chr4c_colset(u32 *dstD, uint left, uint right, uint height, u32 clr);

void schr4c_floodfill_internal(const TSurface *dst, int x, int y, 
    u32 clrNew, u32 clrOld);

// --------------------------------------------------------------------
// HELPERS 
// --------------------------------------------------------------------

//! Returns the clear-mask for the left side of a fill rect.
INLINE u32 chr4_lmask(uint left)
{
	return 0xFFFFFFFFU<<((left&7)*4);
}


//! Returns the clear-mask for the right side of a fill rect.
INLINE u32 chr4_rmask(uint right)
{
	return 0xFFFFFFFFU>>((-right&7)*4);
}

INLINE void chr4c_plot(int x, int y, u32 clr, void *dstBase, u32 dstP)
{
	uint xx= x, yy= y;
	u32 *dstD= (u32*)(dstBase + yy*4 + xx/8*dstP);
	u32 shift= xx%8*4;

    *dstD = (*dstD &~ (15<<shift)) | (clr&15)<<shift;
}

//! Fill a rectangle inside a simple tile-column.
/*!
	\note	\a left and \a right must already be between 0 and 8.
*/
INLINE void chr4c_colset(u32 *dstD, uint left, uint right, uint height, u32 clr)
{
	u32 mask= chr4_lmask(left) & chr4_rmask(right);
	clr &= mask;

	while(height--)
	{
		*dstD= (*dstD &~ mask) | clr;
		dstD++;
	}
}

//! Prepare a screen-entry map for use with chr4.
/*!	
	\param srf	Surface with size information.
	\param map	Screen-blocked map to initialize.
	\param se0	Additive base screen-entry.
*/
void schr4c_prep_map(const TSurface *srf, u16 *map, u16 se0)
{
	uint ix, iy;
	uint width= srf->width/8, height= srf->height/8;
	uint pitch= srf->pitch/32;

	for(iy=0; iy<height; iy++)
		for(ix=0; ix<width; ix++)
			map[iy*32+ix]= ix*pitch+iy + se0;
}

//! Special pointer getter for chr4: start of in-tile line.
u32 *schr4c_get_ptr(const TSurface *srf, int x, int y)
{
	uint xx= x, yy= y;
	return (u32*)(srf->data + yy*4 + xx/8*srf->pitch);	
}


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

//! Get the pixel value of \a src at (\a x, \a y).
u32 schr4c_get_pixel(const TSurface *src, int x, int y)
{
	u32 *srcD= schr4c_get_ptr(src, x, y);
	uint shift= (x&7)*4;

	return (*srcD>>shift)&15;
}


//! Plot a single pixel on a 4bpp tiled surface
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\note	Fairly slow. Inline plotting functionality if possible.
*/
void schr4c_plot(const TSurface *dst, int x, int y, u32 clr)
{
	uint xx= x;
	u32 *dstD= (u32*)(dst->data + xx/8*dst->pitch);
	uint shift= xx%8*4;

	dstD[y] = (dstD[y] &~ (15<<shift)) | (clr&15)<<shift;
}

//! Draw a horizontal line on a 4bpp tiled surface
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void schr4c_hline(const TSurface *dst, int x1, int y, int x2, u32 clr)
{
	if(x2<x1)	{	int tmp= x1; x1= x2; x2= tmp;	}

	u32 *dstL= schr4c_get_ptr(dst, x1, y);
	u32 dstP= dst->pitch/4;
	clr= octup(clr&15);

	uint width= x2-x1+1;
	uint left= x1&7;
	u32 maskL= chr4_lmask(x1), maskR= chr4_rmask(x2+1);

	// Inner-tile line
	if(left+width <= 8)
	{
		u32 mask= maskL & maskR;
		*dstL= (*dstL &~ mask) | (clr&mask);
		return;
	}

	// Non-zero left edge
	if(left != 0)
	{
		*dstL= (*dstL &~ maskL) | (clr&maskL);
		width -= 8-left;
		dstL += dstP;	
	}

	// Non-zero right edge. Note the right edge is at width/8*8
	if(width&7)
	{
		width &= ~7;
		dstL[width/8*dstP]= (dstL[width/8*dstP] &~ maskR) | (clr&maskR);
	}

	// Centers, if any left
	while(width)
	{
		*dstL  = clr;
		dstL  += dstP;
		width -= 8;
	}
}



//! Draw a vertical line on a 4bpp tiled surface
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void schr4c_vline(const TSurface *dst, int x, int y1, int y2, u32 clr)
{
	// --- Normalize ---
	if(y2<y1)	{	int tmp= y1; y1= y2; y2= tmp;	}
	
	u32 *dstL= schr4c_get_ptr(dst, x, y1);
	uint height= y2-y1+1;
	
	uint shift= (x&7)*4;
	u32 mask  = 15<<shift;
	clr       = (clr&15)<<shift;

	while(height--)
	{
		*dstL= (*dstL &~ mask) | clr;
		dstL++;
	}
}

//! Draw a line on a 4bpp tiled surface.
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void schr4c_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr)
{
	// Trivial lines: horz and vertical
	if(y1 == y2)		// Horizontal
	{
		// Adjust for inclusive ends
		if(x2 == x1)
		{	schr4c_plot(dst, x1, y1, clr);	return;	}
		
		schr4c_hline(dst, x1, y1, x2, clr);
		return;
	}
	else if(x1 == x2)	// Vertical
	{
		// Adjust for inclusive ends
		if(y2 == y1)
		{	schr4c_plot(dst, x1, y1, clr);	return;	}
		
		schr4c_vline(dst, x1, y1, y2, clr);
		return;
	}

	// --- Slogging through the diagonal ---

	int ii, dx, dy, xstep, ystep, dd;

	// --- Normalization ---
	if(x1>x2)
	{	xstep= -1;	dx= x1-x2;	}
	else
	{	xstep= +1;	dx= x2-x1;	}

	if(y1>y2)
	{	ystep= -1;	dy= y1-y2;	}
	else
	{	ystep= +1;	dy= y2-y1;	}


	// --- Drawing ---

	if(dx>=dy)		// Diagonal, slope <= 1
	{
		dd= 2*dy - dx;

		for(ii=0; ii<=dx; ii++)
		{
			chr4c_plot(x1, y1, clr, dst->data, dst->pitch);
			if(dd >= 0)
			{	dd -= 2*dx;	y1 += ystep;	}

			dd += 2*dy;
			x1 += xstep;
		}				
	}
	else				// Diagonal, slope > 1
	{
		dd= 2*dx - dy;

		for(ii=0; ii<=dy; ii++)
		{
			chr4c_plot(x1, y1, clr, dst->data, dst->pitch);
			if(dd >= 0)
			{	dd -= 2*dy;	x1 += xstep;	}

			dd += 2*dx;
			y1 += ystep;
		}		
	}
}

//! Render a rectangle on a 4bpp tiled canvas.
/*!
	\param dst		Destination surface.
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color-index.
*/
void schr4c_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(left > right)	{	int t= left; left= right; right=t;	}
	if(top > bottom)	{	int t= top; top= bottom; bottom=t;	}

	clr= octup(clr&15);

	uint width= right-left;
	uint height= bottom-top;

	uint dstP= dst->pitch/4;
	u32 *dstD= schr4c_get_ptr(dst, left, top);

	if((left|right)&7)
	{
		// Left side / single column.
		uint x0= left&7;
		if(x0!=0)
		{
			//clr= octup(2);
			right= min(x0+width, 8);
			chr4c_colset(dstD, x0, right, height, clr);		
			width -= right-x0;
			dstD += dstP;
		}

		// Right column
		if(width&7)
		{
			//clr= octup(4);
			chr4c_colset(&dstD[width/8*dstP], 0, width&7, height, clr);
			width &= ~7;
		}					
	}

	// Center parts (if anything left)
	// Centers, if any left
	while(width)
	{
		//clr= octup(6);
		memset32(dstD, clr, height);
		dstD  += dstP;
		width -= 8;
	}

	return;
}

//! Draw a rectangle on a 4bpp tiled surface.
/*!
	\param dst		Destination surface.
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
	\note	PONDER: RB in- or exclusive?
*/
void schr4c_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	uint height= bottom-top;
	schr4c_hline(dst, left, top, right-1, clr);
	if(height==1)
		return;
	schr4c_hline(dst, left, bottom-1, right-1, clr);

	if(height>2)
	{
		schr4c_vline(dst, left, top, bottom-1, clr);
		schr4c_vline(dst, right-1, top, bottom-1, clr);
	}
}


//! Floodfill an area of the same color with new color \a clr.
/*!
	\param dst	Destination surface.
	\param x	X-coordinate.
	\param y	Y-coordinate;
	\param clr	Color.
	\note		This routines is probably very, very slow. 
*/
void schr4c_floodfill(const TSurface *dst, int x, int y, u32 clr)
{
	u32 clrOld= _schr4c_get_pixel(dst, x, y);

	if(clr == clrOld)
		return;

	schr4c_floodfill_internal(dst, x, y, clr, clrOld);
}

//! Internal routine for floodfill.
/*!
	\note	This traverses the lines horizontally. Amazingly, this
		seems faster than vertically.
*/
void schr4c_floodfill_internal(const TSurface *dst, int x, int y, 
	u32 clrNew, u32 clrOld)
{
	uint dstW= dst->width, dstH= dst->height;

	// Find horz edges, then fill.
	int ii, left=x, right=x;
	while(_schr4c_get_pixel(dst, left-1, y)==clrOld && left>0)
		left--;
	while(_schr4c_get_pixel(dst, right+1, y)==clrOld && right+1<dstW)
		right++;
	schr4c_hline(dst, left, y, right, clrNew);

	// Recursive *ick*. All the stack-work's making GBA's head spin.
	for(ii=left; ii<=right; ii++)
	{
		// clr-first check saves 
		if(_schr4c_get_pixel(dst, ii, y-1) == clrOld && y>0)
			schr4c_floodfill_internal(dst, ii, y-1, clrNew, clrOld);
		
		if(_schr4c_get_pixel(dst, ii, y+1) == clrOld && y+1<dstH)
			schr4c_floodfill_internal(dst, ii, y+1, clrNew, clrOld);		
	}	
}

// EOF
