//
// Bitmap 16bpp render
//	* vwf and fwf
//	* any width, height
//	* 1->16bpp
//	* recolored
//	* transparency
//
//! \file bmp16_drawg_b1cs.c
//! \author J Vijn
//! \date 20070605 - 20070704
//
// === NOTES ===

#include "tonc_memdef.h"

#include "tonc_tte.h"

#include "tonc_warn.h"

// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------


//! Linear bitmap, 16bpp transparent character plotter.
/*	Works on a 16 bpp bitmap (mode 3 or 5).
	\param gid	Character to plot.
	\note	Font req: Any width/height. 1bpp font, 8px stips.
*/
void bmp16_drawg_b1cts(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellH;
	dstD += x0;

	u32 ink= tc->cattr[TTE_INK], raw;

	//# Fixme (src, dst, nx)
	uint ix, iy, iw;
	for(iw=0; iw<charW; iw += 8)
	{
		dstL= &dstD[iw];
		for(iy=0; iy<charH; iy++)
		{
			raw= srcL[iy];
			for(ix=0; raw>0; raw>>=1, ix++)
				if(raw&1)
					dstL[ix]= ink;

			dstL += dstP/2;
		}
		srcL += srcP;
	}
}

//! Linear bitmap, 16bpp opaque character plotter.
/*	Works on a 16 bpp bitmap (mode 3 or 5).
	\param gid	Character to plot.
	\note	Font req: Any width/height. 1bpp font, 8px stips.
*/
void bmp16_drawg_b1cos(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	TTE_DST_VARS(tc, u16, dstD, dstL, dstP, x0, y0);
	uint srcP= font->cellH;

	dstD += x0;

	u32 ink= tc->cattr[TTE_INK];
	u32 paper= tc->cattr[TTE_PAPER];
	u32 raw;

	uint ix, iy, iw;
	for(iw=0; iw<charW; iw += 8)	// For each strip
	{
		dstL= &dstD[iw];
		
		for(iy=0; iy<charH; iy++)	// For each line in strip
		{			
			raw= srcL[iy]<<1;
			for(ix=0; ix<8; ix++)
				dstL[ix]= ((raw>>=1)&1) ? ink : paper;

			dstL += dstP;
		}
		srcL += srcP;
	}
}

// EOF
