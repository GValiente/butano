//
// Bitmap 8 bpp glyph renderers
//
//! \file bmp8_drawg.c
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


//! Linear 8 bpp bitmap glyph renderer, opaque.
/*	Works on a 8 bpp bitmap
	\param gid	Character to plot.
	\note	Font params: bitmapped, 8bpp.
	\note UNTESTED
*/
void bmp8_drawg(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	uint x0= tc->cursorX, y0= tc->cursorY;
	uint srcP= font->cellW;

	// The actual rendering.
	uint ix, iy;
	for(iy=0; iy<charH; iy++)
	{
		// Aw fuckit, slow and easy then.
		for(ix=0; ix<charW; ix++)
			sbmp8_plot(&tc->dst, x0+ix, y0+iy, srcL[ix]);

		srcL += srcP;
	}
}

//! Linear 8 bpp bitmap glyph renderer, transparent.
/*	Works on a 8 bpp bitmap
	\param gid	Character to plot.
	\note	Font params: bitmapped, 8bpp. special cattr is transparent.
	\note UNTESTED
*/
void bmp8_drawg_t(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	uint x0= tc->cursorX, y0= tc->cursorY;
	uint srcP= font->cellW;

	u32 trans= tc->cattr[TTE_SPECIAL];

	// The actual rendering.
	uint ix, iy;
	for(iy=0; iy<charH; iy++)
	{
		for(ix=0; ix<charW; ix++)
			if(srcL[ix] != trans)
				sbmp8_plot(&tc->dst, x0+ix, y0+iy, srcL[ix]);

		srcL += srcP;
	}
}

// EOF
