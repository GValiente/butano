//
// 8bpp bitmap functionality
//
//! \file tonc_bmp8.c
//! \author J Vijn
//! \date 20060604 - 20080516
//
/* === NOTES ===
	* 20070704. Tested all.
*/

#include "tonc_memmap.h"
#include "tonc_core.h"
#include "tonc_video.h"

#include "tonc_warn.h"

// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Plot a single pixel on a 8-bit buffer
/*!
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void bmp8_plot(int x, int y, u32 clr, void *dstBase, uint dstP)
{
	u16 *dstD= (u16*)(dstBase+y*dstP+(x&~1));

	if(x&1)
	   *dstD= (*dstD& 0xFF) | (clr<<8); 
	else
	   *dstD= (*dstD&~0xFF) | (clr&0xFF);
}


//! Draw a horizontal line on an 8bit buffer
/*!
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_hline(int x1, int y, int x2, u32 clr, void *dstBase, uint dstP)
{
	// --- Normalize ---
	clr &= 0xFF;
	if(x2<x1)
	{	int tmp= x1; x1= x2; x2= tmp;	}
		
	uint width= x2-x1+1;
	u16 *dstL= (u16*)(dstBase+y*dstP + (x1&~1));

	// --- Left unaligned pixel ---
	if(x1&1)
	{
		*dstL= (*dstL & 0xFF) + (clr<<8);
		width--;
		dstL++;
	}

	// --- Right unaligned pixel ---
	if(width&1)
		dstL[width/2]= (dstL[width/2]&~0xFF) + clr;
	width /= 2;

	// --- Aligned line ---
	if(width)
		memset16(dstL, dup8(clr), width);
}


//! Draw a vertical line on an 8bit buffer
/*!
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_vline(int x, int y1, int y2, u32 clr, void *dstBase, uint dstP)
{
	// --- Normalize ---
	if(y2<y1)
	{	int tmp= y1; y1= y2; y2= tmp;	}
		
	uint height= y2-y1+1;
	u16 *dstL= (u16*)(dstBase+y1*dstP + (x&~1));
	dstP /= 2;

	if(x&1)
	{
		clr <<= 8;
		while(height--)
		{	*dstL= (*dstL& 0xFF) + clr;	dstL += dstP;	}
	}
	else
	{
		clr &= 0xFF;
		while(height--)
		{	*dstL= (*dstL&~0xFF) + clr;	dstL += dstP;	}
	}
}


//! Draw a line on an 8bit buffer
/*!
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color index.
	\param dstBase	Canvas pointer (halfword-aligned plz).
	\param dstP		Canvas pitch in bytes.
	\note	Does normalization, but not bounds checks.
*/
void bmp8_line(int x1, int y1, int x2, int y2, u32 clr, 
	void *dstBase, uint dstP)
{

	// Trivial lines: horz and vertical
	if(y1 == y2)		// Horizontal
	{
		// Adjust for inclusive ends
		if(x2 == x1)
		{	bmp8_plot(x1, y1, clr, dstBase, dstP);	return;	}
		
		bmp8_hline(x1, y1, x2, clr, dstBase, dstP);
		return;
	}
	else if(x1 == x2)	// Vertical
	{
		// Adjust for inclusive ends
		if(y2 == y1)
		{	bmp8_plot(x1, y1, clr, dstBase, dstP);	return;	}
		
		bmp8_vline(x1, y1, y2, clr, dstBase, dstP);
		return;
	}

	// --- Slogging through the diagonal ---

	int ii, dx, dy, xstep, ystep, dd;
	u32 addr= (u32)(dstBase + y1*dstP + x1), mask= 255;
	u16 *dstL;

	clr &= mask;
	clr |= clr<<8;
	if(x1 & 1)
		mask= ~mask;

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
	// NOTE: because xstep is alternating, you can do marvels 
	//	with mask-flips
	// NOTE: (mask>>31) is equivalent to (x&1) ? 0 : 1

	if(dx>=dy)		// Diagonal, slope <= 1
	{
		dd= 2*dy - dx;

		for(ii=dx; ii>=0; ii--)
		{
			dstL= (u16*)(addr - (mask>>31));
			*dstL= (*dstL &~ mask) | (clr & mask);

			if(dd >= 0)
			{	dd -= 2*dx;	addr += ystep;	}

			dd += 2*dy;
			addr += xstep;
			mask = ~mask;
		}				
	}
	else				// # Diagonal, slope > 1
	{
		dd= 2*dx - dy;

		for(ii=dy; ii>=0; ii--)
		{
			dstL= (u16*)(addr - (mask>>31));
			*dstL= (*dstL &~ mask) | (clr & mask);

			if(dd >= 0)
			{
				dd -= 2*dy;
				addr += xstep;	
				mask = ~mask;
			}

			dd += 2*dx;
			addr += ystep;
		}
	}
}


//! Draw a rectangle in 8bit mode; internal routine.
/*!
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color-index.
	\param dstBase	Canvas pointer.
	\param dstP		Canvas pitch in bytes
	\note	Does normalization, but not bounds checks.
*/
void bmp8_rect(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP)
{
	int tmp, iy;

	// --- Normalization ---
	clr &= 0xFF;
	if(right<left)
	{	tmp= left; left= right; right= tmp;	}

	if(bottom<top)
	{	tmp= top; top= bottom; bottom= tmp;	}


	uint width= right-left, height= bottom-top;
	u16 *dstD= (u16*)(dstBase+top*dstP + (left&~1)), *dstL;
	dstP /= 2;

	// --- Unaligned left ---
	if(left&1)
	{
		dstL= dstD;
		for(iy=0; iy<height; iy++)
		{	*dstL= (*dstL & 0xFF) | clr<<8;	dstL += dstP;	}
		
		width -= 1;
		dstD += 1;
	}

	// --- Unaligned right ---
	if(width&1)
	{
		dstL= dstD + width/2;
		for(iy=0; iy<height; iy++)
		{	*dstL= (*dstL &~0xFF) | clr;	dstL += dstP;	}
		
		width -= 1;
	}

	if(width == 0)
		return;

	// --- Center ---
	clr += clr<<8;
	dstL = dstD;
	for(iy=0; iy<height; iy++)
	{	memset16(dstL, clr, width/2);		dstL += dstP;	}


}

//! Draw a rectangle in 8bit mode; internal routine.
/*!
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color-index.
	\param dstBase	Canvas pointer.
	\param dstP		Canvas pitch in bytes
	\note	Does normalization, but not bounds checks.
	\note	PONDER: RB in- or exclusive?
*/
void bmp8_frame(int left, int top, int right, int bottom, u32 clr,
	void *dstBase, uint dstP)
{
	int tmp;

	// --- Normalization ---
	if(right<left)
	{	tmp= left; left= right; right= tmp;	}

	if(bottom<top)
	{	tmp= top; top= bottom; bottom= tmp;	}

	right--;
	bottom--;

	bmp8_hline(left, top, right, clr, dstBase, dstP);
	bmp8_hline(left, bottom, right, clr, dstBase, dstP);

	bmp8_vline(left, top, bottom, clr, dstBase, dstP);
	bmp8_vline(right, top, bottom, clr, dstBase, dstP);
}

// EOF
