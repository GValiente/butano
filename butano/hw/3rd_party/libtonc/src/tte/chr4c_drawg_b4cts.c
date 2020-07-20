//
// Tile renderer, var width/height, 4bpp tiles, recolored with transparency
//
//! \file chr4c_drawg_b4cts.c
//! \author J Vijn
//! \date 20080427 - 20080427
//
// === NOTES ===

#include "tonc_memdef.h"
#include "tonc_tte.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

void chr4c_drawg_b4cts(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u32, srcD, srcL, charW, charH);
	uint x= tc->cursorX, y= tc->cursorY;
	uint srcP= font->cellH, dstP= tc->dst.pitch/4;

	u32 *dstD= (u32*)(tc->dst.data + y*4 + x/8*dstP*4), *dstL;
	x %= 8;
	u32 lsl= 4*x, lsr= 32-4*x, right= x+charW;

	// Inner loop vars
	u32 amask= 0x11111111;
	u32 px, pxmask, raw;
	u32 ink=   tc->cattr[TTE_INK];
	u32 shade= tc->cattr[TTE_SHADOW];

	uint iy, iw;
	for(iw=0; iw<charW; iw += 8)	// Loop over strips
	{
		dstL= dstD;		dstD += dstP;
		srcL= srcD;		srcD += srcP;

		iy= charH;
		while(iy--)					// Loop over scanlines
		{
			raw= *srcL++;

			px	  = (raw    & amask);
			raw	  = (raw>>1 & amask);
			pxmask= px | raw;
			if(pxmask)
			{
				px *= ink;
				px += raw*shade;
				pxmask *= 15;

				// Write left tile:
				dstL[0] = (dstL[0] &~ (pxmask<<lsl) ) | (px<<lsl);

				// Write right tile (if any)
				if(right > 8)
					dstL[dstP]= (dstL[dstP] &~ (pxmask>>lsr) ) | (px>>lsr);
			}
			dstL++;
		}
	}
}

// EOF
