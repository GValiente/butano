//
// 8bpp bitmap surfaces.
//
//! \file tonc_sbmp8.c
//! \author J Vijn
//! \date 20080127 - 20080128
//! \todo Code consistency.
//
/* === NOTES ===
*/

#include "tonc_surface.h"
#include "tonc_video.h"

#include "tonc_warn.h"

typedef u8 pixel_t;
#define PXSIZE	sizeof(pixel_t)
#define PXPTR(psrf, x, y)	\
	(pixel_t*)(psrf->data + (y)*psrf->pitch + (x)*sizeof(pixel_t) )


void sbmp8_floodfill_internal(const TSurface *dst, int x, int y, 
	u8 clrNew, u8 clrOld);

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

EWRAM_DATA TSurface m4_surface= 
{
	(u8*)m4_mem, M4_WIDTH, M4_WIDTH, M4_HEIGHT, SRF_BMP8, 8, 
	256, pal_bg_mem
};

const TSurfaceProcTab bmp8_tab= 
{
	"bmp8",
	sbmp8_get_pixel,
	sbmp8_plot, 
	sbmp8_hline, 
	sbmp8_vline, 
	sbmp8_line, 
	sbmp8_rect, 
	sbmp8_frame, 
	sbmp8_blit, 
	sbmp8_floodfill, 
};

// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

//! Get the pixel value of \a src at (\a x, \a y).
u32 sbmp8_get_pixel(const TSurface *src, int x, int y)
{
	return src->data[y*src->pitch + x];
}


//! Plot a single pixel on a 8-bit buffer
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y		Y-coord.
	\param clr		Color.
	\note	Slow as fuck. Inline plotting functionality if possible.
*/
void sbmp8_plot(const TSurface *dst, int x, int y, u32 clr)
{
	u16 *dstD= (u16*)PXPTR(dst, x&~1, y);

	if(x&1)
	   *dstD= (*dstD& 255) | (clr<<8); 
	else
	   *dstD= (*dstD&~255) | (clr&0xFF);
}


//! Draw a horizontal line on an 8-bit buffer
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y		Y-coord.
	\param x2		Second X-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp8_hline(const TSurface *dst, int x1, int y, int x2, u32 clr)
{
	if(x2<x1)
	{	int tmp= x1; x1= x2; x2= tmp;	}

	toncset(PXPTR(dst, x1, y), clr, x2-x1+1);
}


//! Draw a vertical line on an 8-bit buffer
/*!
	\param dst		Destination surface.
	\param x		X-coord.
	\param y1		First Y-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp8_vline(const TSurface *dst, int x, int y1, int y2, u32 clr)
{
	// --- Normalize ---
	if(y2<y1)
	{	int tmp= y1; y1= y2; y2= tmp;	}
		
	u16 *dstL= (u16*)PXPTR(dst, x&~1, y1);
	uint dstP= dst->pitch/2;
	uint height= y2-y1+1;

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


//! Draw a line on an 8-bit buffer.
/*!
	\param dst		Destination surface.
	\param x1		First X-coord.
	\param y1		First Y-coord.
	\param x2		Second X-coord.
	\param y2		Second Y-coord.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp8_line(const TSurface *dst, int x1, int y1, int x2, int y2, u32 clr)
{
	// Trivial lines: horz and vertical
	if(y1 == y2)		// Horizontal
	{
		// Adjust for inclusive ends
		if(x2 == x1)
		{	sbmp8_plot(dst, x1, y1, clr);	return;	}
		else if(x2>x1)
			x2++;
		else
			x1++;
		
		sbmp8_hline(dst, x1, y1, x2, clr);
		return;
	}
	else if(x1 == x2)	// Vertical
	{
		// Adjust for inclusive ends
		if(y2 == y1)
		{	sbmp8_plot(dst, x1, y1, clr);	return;	}
		else if(y2>y1)
			y2++;
		else
			y1++;
		
		sbmp8_vline(dst, x1, y1, y2, clr);
		return;
	}

	// --- Slogging through the diagonal ---

	int ii, dx, dy, xstep, ystep, dd;
	uint dstP= dst->pitch;
	u32 addr= (u32)(dst->data + y1*dstP + x1), mask= 255;
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

//! Draw a rectangle in 8-bit mode.
/*!
	\param dst		Destination surface.
	\param left		Left side of rectangle;
	\param top		Top side of rectangle.
	\param right	Right side of rectangle.
	\param bottom	Bottom side of rectangle.
	\param clr		Color.
	\note	Does normalization, but not bounds checks.
*/
void sbmp8_rect(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	pixel_t *dstL= PXPTR(dst, left, top);
	uint dstP= dst->pitch/PXSIZE;
	uint width= right-left, height= bottom-top;

	// --- Draw ---
	while(height--)
	{	toncset(dstL, clr, width);	dstL += dstP;	}
}

//! Draw a rectangle in 8-bit mode.
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
void sbmp8_frame(const TSurface *dst, 
	int left, int top, int right, int bottom, u32 clr)
{
	if(left==right || top==bottom)
		return;

	if(right<left)	{	int tmp= left; left= right; right= tmp;	}
	if(bottom<top)	{	int tmp= top; top= bottom; bottom= tmp;	}

	pixel_t *dstL= PXPTR(dst, left, top);
	uint dstP= dst->pitch/PXSIZE;
	uint width= right-left, height= bottom-top;

	toncset(dstL, clr, width);
	if(height==1)
		return;
	toncset(dstL+(height-1)*dstP, clr, width);

	if(height>2)
	{
		sbmp8_vline(dst, left, top, bottom-1, clr);
		sbmp8_vline(dst, right-1, top, bottom-1, clr);
	}
}

//! 16bpp blitter. Copies a rectangle from one surface to another.
/*!
	\param dst		Destination surface.
	\param dstX		Left coord of rectangle on \a dst.
	\param dstY		Top coord of rectangle on \a dst.
	\param width	Width of rectangle to blit.
	\param height	Height of rectangle to blit.
	\param src		Source surface.
	\param srcX		Left coord of rectangle on \a src.
	\param srcY		Top coord of rectangle on \a src.
	\note			The rectangle will be clipped to both \a src and \a dst.
*/
void sbmp8_blit(const TSurface *dst, int dstX, int dstY, 
	uint width, uint height, const TSurface *src, int srcX, int srcY)
{
	// Safety checks
	if(src==NULL || dst==NULL || src->data==NULL || dst->data==NULL)
		return;

	// --- Clip ---
	int w= width, h= height;

/// Temporary blitter clipping macro
#define BLIT_CLIP(_ax, _aw, _w, _bx)				\
	do {											\
		if( (_ax) >= (_aw) || (_ax)+(_w) <= 0 )		\
			return;									\
		if( (_ax)<0 )								\
		{	_w += (_ax); _bx += (_ax); _ax= 0;	}	\
		if( (_w) > (_aw)-(_ax) )					\
			_w = (_aw)-(_ax);						\
	} while(0)

	// Clip horizontal
	BLIT_CLIP(dstX, dst->width, w, srcX);
	BLIT_CLIP(srcX, src->width, w, dstX);

	// Clip vertical
	BLIT_CLIP(dstY, dst->height, h, srcY);
	BLIT_CLIP(srcY, src->height, h, dstY);

	pixel_t *srcL= PXPTR(src, srcX, srcY);
	pixel_t *dstL= PXPTR(dst, dstX, dstY);
	uint srcP= src->pitch/PXSIZE, dstP= dst->pitch/PXSIZE;

	// Copy clipped rectangle.
	while(h--)
	{
		tonccpy(dstL, srcL, w);
		srcL += srcP;
		dstL += dstP;		
	}

#undef BLIT_CLIP
}


//! Floodfill an area of the same color with new color \a clr.
/*!
	\param dst	Destination surface.
	\param x	X-coordinate.
	\param y	Y-coordinate;
	\param clr	Color.
*/
void sbmp8_floodfill(const TSurface *dst, int x, int y, u32 clr)
{
	pixel_t *dstL= PXPTR(dst, x, y);
	pixel_t clrOld= dstL[0];

	if(clr == clrOld)
		return;

	sbmp8_floodfill_internal(dst, x, y, clr, clrOld);
}

//! Internal routine for floodfill
void sbmp8_floodfill_internal(const TSurface *dst, int x, int y, 
	pixel_t clrNew, pixel_t clrOld)
{
	pixel_t *dstL= PXPTR(dst, 0, y);
	uint dstPitch= dst->pitch/PXSIZE, dstH= dst->height;
	
	// Find horz edges, then fill
	int ii, left=x, right=x;

	while(dstL[left-1]==clrOld && left>0)
		left--;
	while(dstL[right+1]==clrOld && right<dst->width-1)
		right++;
	toncset(&dstL[left], clrNew, right+1-left);

	// Recursive *ick*. All the stack-work's making GBA's head spin.
	for(ii=left; ii<=right; ii++)
	{
		// clr-first check saves 
		if(dstL[ii-dstPitch] == clrOld && y>0)
			sbmp8_floodfill_internal(dst, ii, y-1, clrNew, clrOld);
		
		if(dstL[ii+dstPitch] == clrOld && y+1<dstH)
			sbmp8_floodfill_internal(dst, ii, y+1, clrNew, clrOld);		
	}	
}

// EOF
