//
// Bitmap glyph rendering
//	* vwf and fwf
//	* any width, height
//	* 1->8bpp / strips
//	* recolored
//	* transparency or opaque
//
//! \file bmp8_drawg_b1cs.c
//! \author J Vijn
//! \date 20070613 - 20070613
//
/* === NOTES ===
  * 20070725: Using words here seems to have only minor effects. 
	Could still be worth it in ARM though.
*/

#include "tonc_memdef.h"
#include "tonc_surface.h"

#include "tonc_tte.h"

#include "tonc_warn.h"

void bmp8_drawg_b1cts(uint gid);
void bmp8_drawg_b1cos(uint gid);


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

void bmp8_drawg_b1cts(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellH;
	uint odd= x0&1;

	dstD += x0/2;

	u32 ink= tc->cattr[TTE_INK], raw, px;

	uint ix, iy, iw;
	for(iw=0; iw<charW; iw += 8)			// Loop over strips
	{
		dstL= &dstD[iw/2];
		for(iy=0; iy<charH; iy++)			// Loop over lines
		{
			raw= srcL[iy]<<odd;
			for(ix=0; raw>0; raw>>=2, ix++)	// Loop over pixels
			{
				// 2-bit -> 2-byte unpack, then used as masks.
				// Yes, of course I know how it looks
				px= ( (raw&3)<<7 | (raw&3) ) &~ 0xFE;
				dstL[ix]= (dstL[ix]&~(px*255)) + ink*px;
			}
			dstL += dstP/2;
		}
		srcL += srcP;
	}
}


void bmp8_drawg_b1cos(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellH;
	uint odd= x0&1;

	dstD += x0/2;

	u32 ink= tc->cattr[TTE_INK];
	u32 raw, px, mask;

	// Bugger this; I'm doing it the easy way: pre-clear
	sbmp8_rect(&tc->dst, x0, y0, charW, charH, tc->cattr[TTE_PAPER]);
	
	// and then write as normal.
	// NOTE: this is probably not as fast as it should be.

	uint ix, iy, iw;
	for(iw=0; iw<charW; iw += 8)
	{
		dstL= &dstD[iw/2];	
		for(iy=0; iy<charH; iy++)
		{
			raw= srcL[iy]<<odd;
			for(ix=0; raw>0; raw>>=2, ix++)	// Loop over pixels
			{
				// 2-fold 1->8 bitunpack
				px= ( (raw&3)<<7 | (raw&3) ) &~ 0xFE;
				mask= px*255;
				dstL[ix]= (dstL[ix]&~mask) + ink*px;
			}

			dstL += dstP/2;
		}
		srcL += srcP;
	}
}

// EOF
