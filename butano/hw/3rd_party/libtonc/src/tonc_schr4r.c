//
// 4bpp tiled surfaces, row-major layout (chr4r).
//
//! \file tonc_schr4r.c
//! \author J Vijn
//! \date 20080409 - 20080409
//
/* === NOTES ===
	Tested: plot, hline, vline, line, rect, frame
*/

#include "tonc_surface.h"
#include "tonc_video.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// PROTOTYPES 
// --------------------------------------------------------------------


INLINE u32 chr4_lmask(uint left);
INLINE u32 chr4_rmask(uint right);
INLINE void chr4r_plot(int x, int y, u32 clr, void *dstBase, u32 dstP);
INLINE void chr4r_colset(u32 *dstD, uint dstP4, 
    uint left, uint right, uint height, u32 clr);


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

INLINE void chr4r_plot(int x, int y, u32 clr, void *dstBase, u32 dstP)
{
	uint xx= x, yy= y;
	u32 *dstD= (u32*)(dstBase+yy/8*dstP + yy%8*4 + xx/8*32);
	u32 shift= (x&7)*4;

	*dstD = (*dstD &~ (15<<shift)) | (clr&15)<<shift;
}

//! Fill a rectangle inside a simple tile-column.
/*!
	\note	\a left and \a right must already be between 0 and 8.
*/
INLINE void chr4r_colset(u32 *dstD, uint dstP4, 
	uint left, uint right, uint height, u32 clr)
{
	u32 mask= chr4_lmask(left) & chr4_rmask(right);
	clr &= mask;

	while(height--)
	{
		*dstD= (*dstD &~ mask) | clr;
        dstD++;
		if((u32)dstD%32 == 0)
			dstD += dstP4-8;
	}
}

//! Prepare a screen-entry map for use with chr4.
/*!	
    \param srf	Surface with size information.
	\param map	Screen-blocked map to initialize.
	\param se0	Additive base screen-entry.
*/
void schr4r_prep_map(const TSurface *srf, u16 *map, u16 se0)
{
	uint ix, iy;
	uint width= srf->width/8, height= srf->height/8;
	uint pitch= srf->pitch/32;

	for(iy=0; iy<height; iy++)
		for(ix=0; ix<width; ix++)
			map[iy*32+ix]= iy*pitch+ix + se0;
}

//! Special pointer getter for chr4: start of in-tile line.
u32 *schr4r_get_ptr(const TSurface *srf, int x, int y)
{
	uint xx= x, yy= y;
	return (u32*)(srf->data+yy/8*srf->pitch + yy%8*4 + xx/8*32);	
}


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Get the pixel value of \a src at (\a x, \a y).
u32 schr4r_get_pixel(const TSurface *src, int x, int y)
{
	uint xx= x, yy= y;
	u32 *srcD= (u32*)(src->data+yy/8*src->pitch + yy%8*4 + xx/8*32);
	uint shift= xx%8*4;

	return (*srcD>>shift)&15;
}


//! Plot a single pixel on a 4bpp tiled surface
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void schr4r_plot(const TSurface *dst, int x, int y, u32 clr)
{
	uint xx= x, yy= y;
	u32 *dstD= (u32*)(dst->data+yy/8*dst->pitch + yy%8*4 + xx/8*32);
	uint shift= xx%8*4;

	*dstD = (*dstD &~ (15<<shift)) | (clr&15)<<shift;
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
void schr4r_hline(const TSurface *dst, int x1, int y, int x2, u32 clr)
{
	if(x2<x1)	{	int tmp= x1; x1= x2; x2= tmp;	}

	u32 *dstD= (u32*)(dst->data+(y>>3)*dst->pitch + (y&7)*4 + (x1>>3)*32);
	clr= octup(clr&15);

	uint width= x2-x1+1;
	uint left= x1&7;
	u32 maskL= chr4_lmask(x1), maskR= chr4_rmask(x2+1);

	// Inner-tile line
	if(left+width <= 8)
	{
		u32 mask= maskL & maskR;
		*dstD= (*dstD &~ mask) | (clr&mask);
		return;
	}

	// Non-zero left edge
	if(left != 0)
	{
		*dstD= (*dstD &~ maskL) | (clr&maskL);
		width -= 8-left;
		dstD += 8;	
	}

	// Non-zero right edge. Note the right edge is at width/8*8
	if(width&7)
	{
		width &= ~7;
		dstD[width]= (dstD[width] &~ maskR) | (clr&maskR);
	}

	// Centers, if any left
	while(width)
	{
		*dstD  = clr;
		dstD  += 8;
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
void schr4r_vline(const TSurface *dst, int x, int y1, int y2, u32 clr)
{
	// --- Normalize ---
	if(y2<y1)	{	int tmp= y1; y1= y2; y2= tmp;	}
	
	u32 *dstL= schr4r_get_ptr(dst, x, y1);
	uint dstP= dst->pitch/4;
	uint height= y2-y1+1;
	
	uint shift= (x&7)*4;
	u32 mask  = 15<<shift;
	clr       = (clr&15)<<shift;

	while(height--)
	{
		*dstL= (*dstL &~ mask) | clr;
		dstL++;
		
		if( ((u32)dstL)%32 == 0)
			dstL += dstP-8;
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
void schr4r_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr)
{
	// Trivial lines: horz and vertical
	if(y1 == y2)		// Horizontal
	{
		// Adjust for inclusive ends
		if(x2 == x1)
		{	schr4r_plot(dst, x1, y1, clr);	return;	}
		
		schr4r_hline(dst, x1, y1, x2, clr);
		return;
	}
	else if(x1 == x2)	// Vertical
	{
		// Adjust for inclusive ends
		if(y2 == y1)
		{	schr4r_plot(dst, x1, y1, clr);	return;	}
		
		schr4r_vline(dst, x1, y1, y2, clr);
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
			chr4r_plot(x1, y1, clr, dst->data, dst->pitch);
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
			chr4r_plot(x1, y1, clr, dst->data, dst->pitch);
			if(dd >= 0)
			{	dd -= 2*dy;	x1 += xstep;	}

			dd += 2*dx;
			y1 += ystep;
		}		
	}
}

//! Render a rectangle on a tiled canvas.
/*!
	\param dst		Destination surface.
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color-index. Octupled if > 16.
	\note	For a routine like this you can strive for programmer sanity
		or speed. This is for speed. Except for very small rects, this 
		is between 5x and 300x faster than the trivial version.
	Here's how it works:
	   | c |
	   +---+
	 a | d | b
	   +---+
	   | e |

	Boundaries are tile-boundaries; 
	- If unaligned left : draw A [left,8), update dstD/width
	- If unaligned right: draw B [right&~7,right), Adjust dstD/width
	- If width>0
	  - if unaligned top : draw C in ix/iy loop. Adjust dstD/height
	  - If height>8 : draw D in memset32 blocks, adjust height
	  - Final sets : draw E in ix/iy loop
*/
void schr4r_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(left > right)	{	int t= left; left= right; right=t;	}
	if(top > bottom)	{	int t= top; top= bottom; bottom=t;	}

	clr= octup(clr&15);

	uint width= right-left;
	uint height= bottom-top;

	uint dstP= dst->pitch;
	u32 *dstD= (u32*)(dst->data+(top>>3)*dstP + (top&7)*4 + (left>>3)*32);

	// --- Trivial case: all 8px boundaries ---
	if( ((left|right)&7)==0 && ((top|bottom)&7)==0 )
	{
		height /= 8;
		while(height--)
		{
			//clr= octup(1);
			memset32(dstD, clr, width);
			dstD += dstP/4;
		}
		return;
	}

	// -- Non-trivial case. Aw fuck. ---

	uint x0= left&7;

	// Single column
	if(x0+width <= 8)
	{
		//clr= octup(2);
		chr4r_colset(dstD, dstP/4, x0, x0+width, height, clr);		
		return;
	}

	// Left column
	if(x0 != 0)
	{
		//clr= octup(3);
		chr4r_colset(dstD, dstP/4, x0, 8, height, clr);
		width -= 8-x0;
		dstD += 8;

	}

	// Right column
	if(width&7)
	{
		//clr= octup(4);
		chr4r_colset(&dstD[width&~7], dstP/4, 0, width&7, height, clr);
		width &= ~7;
	}

	// Check if we're done (narrow rects)
	if(width==0)
		return;
	
	uint ix, iy;
	uint y0= top&7;

	// Top Bar
	if(y0 != 0)
	{
		//clr= octup(5);
		uint h2= y0+height<=8 ? height : 8-y0;
		for(iy=0; iy<h2; iy++)
		{
			for(ix=0; ix<width; ix += 8)
				dstD[ix]= clr;
			dstD++;
		}
		height -= h2;
		dstD += dstP/4-8;
	}

	// Main stint
	for(iy=0; iy<height/8; iy++)
	{
		//clr= octup(6);
		memset32(dstD, clr, width);
		dstD += dstP/4;		
	}

	height &= 7;

	// Bottom bar
	for(iy=0; iy<height; iy++)
	{
		//clr= octup(7);
		for(ix=0; ix<width; ix += 8)
			dstD[ix]= clr;
		dstD++;
	}
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
void schr4r_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	uint height= bottom-top;
	schr4r_hline(dst, left, top, right-1, clr);
	if(height==1)
		return;
	schr4r_hline(dst, left, bottom-1, right-1, clr);

	if(height>2)
	{
		schr4r_vline(dst, left, top, bottom-1, clr);
		schr4r_vline(dst, right-1, top, bottom-1, clr);
	}
}




// EOF
