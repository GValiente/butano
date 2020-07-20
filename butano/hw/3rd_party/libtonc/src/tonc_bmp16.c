//
// 16bpp bitmap functionality
//
//! \file tonc_bmp16.c
//! \author J Vijn
//! \date 20060604 - 20070703
//
/* === NOTES ===
	* 20070704. Tested except for bmp16_line
*/

#include "tonc_memmap.h"
#include "tonc_core.h"
#include "tonc_video.h"

#include "tonc_warn.h"

// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

//! Plot a single pixel on a 16-bit buffer
/*!
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void bmp16_plot(int x, int y, u32 clr, void *dstBase, uint dstP)
{
	u16 *dstL= (u16*)(dstBase+y*dstP + x*2);

	dstL[0]= clr;
}


//! Draw a horizontal line on an 16bit buffer
/*!	
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp16_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP)
{
	// --- Normalize ---
	if(x2<x1)
	{	int tmp= x1; x1= x2; x2= tmp;	}

	u16 *dstL= (u16*)(dstBase+y*dstP + x1*2);
	
	// --- Draw ---	
	memset16(dstL, clr, x2-x1+1);
}


//! Draw a vertical line on an 16bit buffer
/*!
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp16_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP)
{
	// --- Normalize ---
	if(y2<y1)
	{	int tmp= y1; y1= y2; y2= tmp;	}
		
	uint height= y2-y1+1;
	u16 *dstL= (u16*)(dstBase+y1*dstP);
	dstP /= 2;

	// --- Draw ---
	while(height--)
	{	dstL[x]= clr;	dstL += dstP;	}
}


//! Draw a line on an 16bit buffer
/*!
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp16_line(int x1, int y1, int x2, int y2, u32 clr, 
	void *dstBase, uint dstP)
{
	int ii, dx, dy, xstep, ystep, dd;
	u16 *dstL= (u16*)(dstBase + y1*dstP + x1*2);
	dstP /= 2;

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


//! Draw a rectangle in 16bit mode; internal routine.
/*!
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color.
	\param dstBase	Canvas pointer.
	\param dstP		Canvas pitch in bytes
	\note	Does normalization, but not bounds checks.
*/
void bmp16_rect(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP)
{
	int tmp;

	// --- Normalization ---
	if(right<left)
	{	tmp= left; left= right; right= tmp;	}

	if(bottom<top)
	{	tmp= top; top= bottom; bottom= tmp;	}


	uint width= right-left, height= bottom-top;
	u16 *dstL= (u16*)(dstBase+top*dstP + left*2);
	dstP /= 2;

	// --- Draw ---
	while(height--)
	{	memset16(dstL, clr, width);	dstL += dstP;	}
}

//! Draw a rectangle in 16bit mode; internal routine.
/*!
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color.
	\param dstBase	Canvas pointer.
	\param dstP		Canvas pitch in bytes
	\note	Does normalization, but not bounds checks.
	\note	PONDER: RB in- or exclusive?
*/
void bmp16_frame(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP)
{
	int tmp;

	// --- Normalization ---
	if(right<left)
	{	tmp= left; left= right; right= tmp;	}

	if(bottom<top)
	{	tmp= top; top= bottom; bottom= tmp;	}

	uint width= right-left, height= bottom-top;
	u16 *dstL= (u16*)(dstBase+top*dstP + left*2);
	dstP /= 2;

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

// EOF
