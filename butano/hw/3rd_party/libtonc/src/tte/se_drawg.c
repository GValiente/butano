//
// Screen-entry plotter
//
//! \file se_drawg.c
//! \author J Vijn
//! \date 20070628 - 20070628
//
// === NOTES ===


#include "tonc_types.h"
#include "tonc_tte.h"
#include "tonc_surface.h"

#include "tonc_warn.h"


typedef u16 pixel_t;
#define PXSIZE	sizeof(pixel_t)
#define PXPTR(psrf, x, y)	\
	(pixel_t*)((psrf)->data + (y)*(psrf)->pitch + (x)*sizeof(pixel_t) )


//! Erase part of the regular tilemap canvas.
void se_erase(int left, int top, int right, int bottom)
{	
	TTC *tc= tte_get_context();

	//# Check for single sequence
	sbmp16_rect(&tc->dst, left>>3, top>>3, right>>3, bottom>>3, 
		tc->cattr[TTE_PAPER]);
}


// --------------------------------------------------------------------
// drawg routines
// --------------------------------------------------------------------


//! Character-plot for reg BGs using an 8x8 font.
void se_drawg_w8h8(uint gid)
{
	TTC *tc= tte_get_context();

	uint x0= tc->cursorX, y0= tc->cursorY;
	u16 *dstD= PXPTR(&tc->dst, x0/8, y0/8);

	dstD[0]= tc->cattr[TTE_SPECIAL] + gid;
}

//! Character-plot for reg BGs using an 8x16 font.
void se_drawg_w8h16(uint gid)
{
	TTC *tc= tte_get_context();

	uint x0= tc->cursorX, y0= tc->cursorY;
	uint dstP= tc->dst.pitch/PXSIZE;
	u16 *dstD= PXPTR(&tc->dst, x0/8, y0/8);

	u32 se= tc->cattr[TTE_SPECIAL] + gid*2;

	dstD[0]= se;
	dstD[dstP]= se+1;
}

//! Character-plot for reg BGs, any sized font.
void se_drawg(uint gid)
{
	int ix, iy;

	TTE_BASE_VARS(tc, font);
	u32 charW= (font->cellW+7)/8, charH= (font->cellH+7)/8;

	uint x0= tc->cursorX, y0= tc->cursorY;
	uint dstP= tc->dst.pitch/PXSIZE;
	u16 *dstD= PXPTR(&tc->dst, x0/8, y0/8);

	u32 se= tc->cattr[TTE_SPECIAL] + gid*charW*charH;

	for(iy=0; iy<charH; iy++)
	{
		for(ix=0; ix<charW; ix++)
			dstD[ix]= se++;
		
		dstD += dstP;
	}
}

//! Character-plot for reg BGs, any sized, vertically tiled font.
void se_drawg_s(uint gid)
{
	int ix, iy;

	TTE_BASE_VARS(tc, font);
	u32 charW= (font->cellW+7)/8, charH= (font->cellH+7)/8;

	uint x0= tc->cursorX, y0= tc->cursorY;
	uint dstP= tc->dst.pitch/PXSIZE;
	u16 *dstD= PXPTR(&tc->dst, x0/8, y0/8);

	u32 se= tc->cattr[TTE_SPECIAL] + gid*charW*charH;

	for(ix=0; ix<charW; ix++)
	{
		for(iy=0; iy<charH; iy++)
			dstD[iy*dstP]= se++;
		
		dstD++;
	}
}

// EOF
