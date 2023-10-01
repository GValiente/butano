//
// 16bpp bitmap surfaces.
//
//! \file tonc_sbmp16.c
//! \author J Vijn
//! \date 20080120 - 20080128
//! \todo Code consistency.
//
/* === NOTES ===
*/

#include "tonc_surface.h"
#include "tonc_video.h"

#include "tonc_warn.h"

typedef u16 pixel_t;
#define PXSIZE	sizeof(pixel_t)
#define PXPTR(psrf, x, y)	\
	(pixel_t*)(psrf->data + (y)*psrf->pitch + (x)*sizeof(pixel_t) )


void sbmp16_floodfill_internal(const TSurface *dst, int x, int y, 
	u16 clrNew, u16 clrOld);

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

const TSurface m3_surface= 
{
	(u8*)m3_mem , M3_WIDTH*2, M3_WIDTH, M3_HEIGHT, SRF_BMP16, 16, 
	0, NULL
};

// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Get the pixel value of \a src at (\a x, \a y).
u32 sbmp16_get_pixel(const TSurface *src, int x, int y)
{
	return *(u16*)PXPTR(src, x, y);
}

//! Plot a single pixel on a 16-bit buffer
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void sbmp16_plot(const TSurface *dst, int x, int y, u32 clr)
{
	*(u16*)PXPTR(dst, x, y)= clr;
}


//! Draw a horizontal line on an 16bit buffer
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp16_hline(const TSurface *dst, int x1, int y, int x2, u32 clr)
{
	// --- Normalize ---
	if(x2<x1)
	{	int tmp= x1; x1= x2; x2= tmp;	}
	
	// --- Draw ---	
	memset16(PXPTR(dst, x1, y), clr, x2-x1+1);
}


//! Draw a vertical line on an 16bit buffer
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp16_vline(const TSurface *dst, int x, int y1, int y2, u32 clr)
{
	// --- Normalize ---
	if(y2<y1)
	{	int tmp= y1; y1= y2; y2= tmp;	}
		
	u32 height= y2-y1+1;
	pixel_t *dstL= PXPTR(dst, x, y1);
	u32 dstP= dst->pitch/PXSIZE;

	// --- Draw ---
	while(height--)
	{	*dstL= clr;	dstL += dstP;	}
}


//! Draw a line on an 16bit buffer.
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp16_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr)
{
	int ii, dx, dy, xstep, ystep, dd;
	pixel_t *dstL= PXPTR(dst, x1, y1);
	u32 dstP= dst->pitch/PXSIZE;

	// --- Normalization ---
	if(x1>x2)
	{	xstep= -1;	dx= x1-x2;	}
	else
	{	xstep= +1;	dx= x2-x1;	}

	if(y1>y2)
	{	ystep= -dstP;	dy= y1-y2;	}
	else
	{	ystep= +dstP;	dy= y2-y1;	}


	// --- Drawing ---

	if(dy == 0)			// Horizontal
	{
		for(ii=0; ii<=dx; ii++)
			dstL[ii*xstep]= clr;
	}
	else if(dx == 0)	// Vertical
	{
		for(ii=0; ii<=dy; ii++)
			dstL[ii*ystep]= clr;
	}
	else if(dx>=dy)		// Diagonal, slope <= 1
	{
		dd= 2*dy - dx;

		for(ii=0; ii<=dx; ii++)
		{
			*dstL= clr;
			if(dd >= 0)
			{	dd -= 2*dx;	dstL += ystep;	}

			dd += 2*dy;
			dstL += xstep;
		}				
	}
	else				// Diagonal, slope > 1
	{
		dd= 2*dx - dy;

		for(ii=0; ii<=dy; ii++)
		{
			*dstL= clr;
			if(dd >= 0)
			{	dd -= 2*dy;	dstL += xstep;	}

			dd += 2*dx;
			dstL += ystep;
		}		
	}
}

//! Draw a rectangle in 16bit mode.
/*!
	\param dst		Destination surface.
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp16_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	u32 width= right-left, height= bottom-top;
	pixel_t *dstL= PXPTR(dst, left, top);
	u32 dstP= dst->pitch/PXSIZE;

	// --- Draw ---
	while(height--)
	{	memset16(dstL, clr, width);	dstL += dstP;	}
}

//! Draw a rectangle in 16bit mode.
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
void sbmp16_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	u32 width= right-left, height= bottom-top;
	pixel_t *dstL= PXPTR(dst, left, top);
	u32 dstP= dst->pitch/PXSIZE;

	// --- Top line ---
	memset16(dstL, clr, width);
	if(height<2)
		return;
	dstL += dstP;
	height -= 2;

	// --- Left/right lines ---
	while(height--)
	{
		dstL[0]= clr;
		dstL[width-1]= clr;
		dstL += dstP;
	}

	// --- Bottom line ---
	memset16(dstL, clr, width);
}

//! Floodfill an area of the same color with new color \a clr.
/*!
	\param dst	Destination surface.
	\param x	X-coordinate.
	\param y	Y-coordinate;
	\param clr	Color.
*/
void sbmp16_floodfill(const TSurface *dst, int x, int y, u32 clr)
{
	pixel_t *dstL= PXPTR(dst, x, y);
	pixel_t clrOld= dstL[0];

	if(clr == clrOld)
		return;

	sbmp16_floodfill_internal(dst, x, y, clr, clrOld);
}

//! Internal routine for floodfill
void sbmp16_floodfill_internal(const TSurface *dst, int x, int y, 
	pixel_t clrNew, pixel_t clrOld)
{
	pixel_t *dstL= PXPTR(dst, 0, y);
	u32 dstPitch= dst->pitch/PXSIZE, dstH= dst->height;
	
	// Find horz edges, then fill
	int ii, left=x, right=x;

	while(dstL[left-1]==clrOld && left>0)
		left--;
	while(dstL[right+1]==clrOld && right<dst->width-1)
		right++;
	memset16(&dstL[left], clrNew, right+1-left);

	// Recursive *ick*. All the stack-work's making GBA's head spin.
	for(ii=left; ii<=right; ii++)
	{
		// clr-first check saves 
		if(dstL[ii-dstPitch] == clrOld && y>0)
			sbmp16_floodfill_internal(dst, ii, y-1, clrNew, clrOld);
		
		if(dstL[ii+dstPitch] == clrOld && y+1<dstH)
			sbmp16_floodfill_internal(dst, ii, y+1, clrNew, clrOld);		
	}	
}

// EOF
