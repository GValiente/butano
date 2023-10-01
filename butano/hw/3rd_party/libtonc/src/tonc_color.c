//
//  Some interesting color routines
//
//! \file tonc_color.c
//! \author J Vijn
//! \date 20070823 - 20070907
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_video.h"
#include "tonc_bios.h"
#include "tonc_math.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------

//!	Blends color arrays \a srca and \a srcb into \a dst.
/*!	Specific transitional blending effects can be created by making 
	a 'target' color array with other routines, then using \a alpha 
	to morph into it.
	\param srca	Source array A.
	\param srcb	Source array B
	\param dst	Destination array.
	\param nclrs	Number of colors.
	\param alpha	Blend weight (range: 0-32). 0 Means full \a srca
*/
void clr_blend(const COLOR *srca, const COLOR *srcb, COLOR *dst, 
		uint nclrs, u32 alpha)
{
	if(!nclrs)
		return;

	int ii;
	u32 clra, clrb, clr;
	u32 parta, partb, part;
	const u32 rbmask= (RED_MASK|BLUE_MASK), gmask= GREEN_MASK;
	const u32 rbhalf= 0x4010, ghalf= 0x0200;

	for(ii=0; ii<nclrs; ii++)
	{
		clra= srca[ii];
		clrb= srcb[ii];
		
		// Red and blue
		parta= clra & rbmask;
		partb= clrb & rbmask;
		part = (partb-parta)*alpha + parta*32 + rbhalf;
		clr  = (part/32 ) & rbmask;

		// Green
		parta= clra & gmask;
		partb= clrb & gmask;
		part = (partb-parta)*alpha + parta*32 + ghalf;
		clr |= (part/32 ) & gmask;

		dst[ii]= clr;
	}
}

//!	Fades color arrays \a srca to \a clr into \a dst.
/*!
	\param src	Source array.
	\param clr	Final color (at alpha=32).
	\param dst	Destination array.
	\param nclrs	Number of colors.
	\param alpha	Blend weight (range: 0-32). 0 Means full \a srca
*/
void clr_fade(const COLOR *src, COLOR clr, COLOR *dst, 
		uint nclrs, u32 alpha)
{
	if(!nclrs)
		return;

	int ii;
	u32 clra, parta, part;
	const u32 rbmask= (RED_MASK|BLUE_MASK), gmask= GREEN_MASK;
	const u32 rbhalf= 0x4010, ghalf= 0x0200;

	// precalc color B parts
	u32 partb_rb= clr & rbmask, partb_g= clr & gmask;

	for(ii=0; ii<nclrs; ii++)
	{
		clra= src[ii];

		// Red and blue
		parta= clra & rbmask;
		part = (partb_rb-parta)*alpha + parta*32 + rbhalf;
		clr  = (part/32) & rbmask;

		// Green
		parta= clra & gmask;
		part = (partb_g-parta)*alpha + parta*32 + ghalf;
		clr |= (part/32) & gmask;

		dst[ii]= clr;
	}
}


//! Transform colors to grayscale.
/*!
	\param dst	Destination color array
	\param src	Source color array.
	\param nclrs	Number of colors.
*/
void clr_grayscale(COLOR *dst, const COLOR *src, uint nclrs)
{
	u32 ii;
	u32 clr, gray, rr, gg, bb;

	for(ii=0; ii<nclrs; ii++)
	{
		clr= *src++;

		// Do RGB conversion in .8 fixed point
		rr= ((clr    )&31)*0x4C;	// 29.7%
		gg= ((clr>> 5)&31)*0x96;	// 58.6%
		bb= ((clr>>10)&31)*0x1E;	// 11.7%
		gray= (rr+gg+bb+0x80)>>8;

		*dst++= RGB15(gray, gray, gray);
	}
}

//! Create a gradient between pal[first] and pal[last].
/*!
	\param pal		Palette to work on.
	\param first	First index of gradient.
	\param last		Last index of gradient.
*/
void pal_gradient(COLOR *pal, int first, int last)
{
	// Normalize
	if(first > last)
		SWAP(first, last);

	// 2 entries means nothing to do
	if(last-first < 2)
		return;

	int ii;
	COLOR clra= pal[first], clrb= pal[last];

	int r= (clra    )&31, dr= ((clrb    )&31) - r;
	int g= (clra>> 5)&31, dg= ((clrb>> 5)&31) - g;
	int b= (clra>>10)&31, db= ((clrb>>10)&31) - b;

	// r,g,b will be .20 fixeds. Note rounding
	const u32 SCALE= 1<<16;
	r= r*SCALE + SCALE/2;	dr= (int)(dr*SCALE)/(last-first);
	g= g*SCALE + SCALE/2;	dg= (int)(dg*SCALE)/(last-first);
	b= b*SCALE + SCALE/2;	db= (int)(db*SCALE)/(last-first);

	for(ii=first+1; ii<last; ii++)
	{
		r += dr;
		g += dg;
		b += db;

		pal[ii]= RGB15(r/SCALE, g/SCALE, b/SCALE);
	}
}

//! Create a gradient between pal[first] and pal[last].
/*!
	\param pal		Palette to work on.
	\param first	First index of gradient.
	\param last		Last index of gradient.
	\param clr_first	Color of first index.
	\param clr_last		Color of last index.
*/
void pal_gradient_ex(COLOR *pal, int first, int last, 
	COLOR clr_first, COLOR clr_last)
{
	pal[first]= clr_first;
	pal[last]= clr_last;
	pal_gradient(pal, first, last);
}


// --------------------------------------------------------------------
// Color adjustments
// --------------------------------------------------------------------


//! Adjust brightness by \a bright
/*!	Operation: color= color+dB;
	\param dst	Destination color array
	\param src	Source color array.
	\param nclrs	Number of colors.
	\param bright	Brightness difference, dB (in .8f)
	\note	Might be faster if preformed via lut.
*/
void clr_adj_brightness(COLOR *dst, const COLOR *src, uint nclrs, FIXED bright)
{
	u32 ii, clr;
	int rr, gg, bb;

	bright >>= 3;
	for(ii=0; ii<nclrs; ii++)
	{
		clr= src[ii];

		rr= ((clr    )&31) + bright;
		gg= ((clr>>5 )&31) + bright;
		bb= ((clr>>10)&31) + bright;

		dst[ii]= RGB15(bf_clamp(rr,5), bf_clamp(gg,5), bf_clamp(bb,5));
	}
}


//! Adjust contrast by \a contrast
/*!	Operation: color = color*(1+dC) - MAX*dC/2
	\param dst	Destination color array
	\param src	Source color array.
	\param nclrs	Number of colors.
	\param contrast	Contrast differencem dC (in .8f)
	\note	Might be faster if preformed via lut.
*/
void clr_adj_contrast(COLOR *dst, const COLOR *src, uint nclrs, FIXED contrast)
{
	u32 ii, clr;
	int rr, gg, bb;
	FIXED ca= contrast+FIX_ONE, cb= (-contrast>>1)*32;

	for(ii=0; ii<nclrs; ii++)
	{
		clr= src[ii];

		rr= ca*((clr    )&31) + cb;
		gg= ca*((clr>>5 )&31) + cb;
		bb= ca*((clr>>10)&31) + cb;

		// Shift here, because it'd be unsigned-shift otherwise
		dst[ii]= RGB15(bf_clamp(rr>>8,5), bf_clamp(gg>>8,5), bf_clamp(bb>>8,5));
	}
}

//! Adjust intensity by \a intensity. 
/*!	Operation: color = (1+dI)*color.
	\param dst	Destination color array
	\param src	Source color array.
	\param nclrs	Number of colors.
	\param intensity	Intensity difference, dI (in .8f)
	\note	Might be faster if preformed via lut.
*/
void clr_adj_intensity(COLOR *dst, const COLOR *src, uint nclrs, FIXED intensity)
{
	u32 ii, clr;
	int rr, gg, bb;
	FIXED ia= intensity+FIX_ONE;

	for(ii=0; ii<nclrs; ii++)
	{
		clr= *src++;

		rr= ia*((clr    )&31);
		gg= ia*((clr>>5 )&31);
		bb= ia*((clr>>10)&31);

		// Shift here, because it'd be unsigned-shift otherwise
		*dst++= RGB15(bf_clamp(rr>>8,5), bf_clamp(gg>>8,5), bf_clamp(bb>>8,5));
	}
}


// EOF
