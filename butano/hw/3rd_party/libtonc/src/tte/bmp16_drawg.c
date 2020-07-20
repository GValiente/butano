//
// Bitmap 16bpp glyph renderers
//
//! \file bmp16_drawg.c
//! \author J Vijn
//! \date 20080311 - 20080311
//
// === NOTES ===

#include "tonc_memdef.h"

#include "tonc_tte.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------


//! Linear 16bpp bitmap glyph renderer, opaque.
/*	Works on a 16 bpp bitmap
	\param gid	Character to plot.
	\note	Font params: bitmapped, 16bpp.
*/
void bmp16_drawg(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u16, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellW;
	dstL= &dstD[x0];

	// The actual rendering
	uint ix, iy;
	for(iy=0; iy<charH; iy++)
	{
		for(ix=0; ix<charW; ix++)
			dstL[ix]= srcL[ix];

		srcL += srcP;
		dstL += dstP/2;	
	}
}

//! Linear 16bpp bitmap glyph renderer, transparent.
/*	Works on a 16 bpp bitmap
	\param gid	Character to plot.
	\note	Font params: bitmapped, 16bpp. special cattr is transparent.
*/
void bmp16_drawg_t(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u16, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellW;
	dstL= &dstD[x0];

	u32 trans= tc->cattr[TTE_SPECIAL];

	// The actual rendering
	uint ix, iy;
	for(iy=0; iy<charH; iy++)
	{
		for(ix=0; ix<charW; ix++)
			if(srcL[ix] != trans)
				dstL[ix]= srcL[ix];

		srcL += srcP;
		dstL += dstP/2;	
	}
}

// EOF
