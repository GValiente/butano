//
// Tile renderer, var width/height, 1->4bpp tiles,
// recolored with transparency
//
//! \file chr4r_drawg_b1cts.c
//! \author J Vijn
//! \date 20070621 - 20070725
//
/* === NOTES ===
  * 20070725: Skipping rendering if raw == 0 helps. A lot. Also, there 
    is more than one way to bitunpack and split between tiles. Which 
	method is faster is very platform dependent.
  * 20070723: Prepping dst stuff inside the drawg and passing along to 
	renc does NOT help (prolly due to Thumb). Try combining in 
	asm manually.
*/

#include "tonc_memdef.h"
#include "tonc_tte.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

//! Render 1bpp fonts to 4bpp tiles
void chr4r_drawg_b1cts(uint gid)
{
	TTE_BASE_VARS(tc, font);
	TTE_CHAR_VARS(font, gid, u8, srcD, srcL, charW, charH);
	uint x= tc->cursorX, y= tc->cursorY;
	uint srcP= font->cellH, dstP= tc->dst.pitch/4;

	u32 *dstD= (u32*)(tc->dst.data + y/8*dstP + (y%8)*4 + x/8*32), *dstL;
	dstP= dstP/4 - 8;
	x %= 8;
	u32 lsl= 4*x, lsr= 32-4*x, right= x+charW;

	// Inner loop vars
	u32 px, pxmask, raw;
	u32 ink= tc->cattr[TTE_INK];
	const u32 mask= 0x01010101;

	uint iy, iw;
	for(iw=0; iw<charW; iw += 8)	// Loop over strips
	{
		dstL= dstD;		dstD += 8;
		srcL= srcD;		srcD += srcP;

		iy= charH;
		while(iy--)					// Loop over scanlines
		{
			raw= *srcL++;
			if(raw)
			{
				raw |= raw<<12;
				raw |= raw<< 6;
				px   = raw & mask<<1;
				raw &= mask;
				px   = raw | px<<3;

				pxmask= px*15;
				px   *= ink;

				// Write left tile:
				dstL[0] = (dstL[0] &~ (pxmask<<lsl) ) | (px<<lsl);

				// Write right tile (if any)
				if(right > 8)
					dstL[8]= (dstL[8] &~ (pxmask>>lsr) ) | (px>>lsr);
			}
			dstL++;

			if( ((u32)dstL)%32 == 0 )
				dstL += dstP;
		}
	}
}

// EOF
