//
// Affine Screen-entry glyph drawers
//
//! \file ase_drawg.c
//! \author J Vijn
//! \date 20070701 - 20080516
//
/* === NOTES ===
	* Not exactly pretty implementations here, but they work.
*/


#include "tonc_types.h"
#include "tonc_surface.h"

#include "tonc_tte.h"

#include "tonc_warn.h"

//! Erase part of the affine tilemap canvas.
void ase_erase(int left, int top, int right, int bottom)
{	
	TTC *tc= tte_get_context();

	//# Check for single sequence
	sbmp8_rect(&tc->dst, left>>3, top>>3, right>>3, bottom>>3, 
		tc->cattr[TTE_PAPER]);
}

// --------------------------------------------------------------------
// drawg routines
// --------------------------------------------------------------------


//! Character-plot for  affine BGs using an 8x8 font.
void ase_drawg_w8h8(uint gid)
{
	TTC *tc= tte_get_context();
	u8 se= tc->cattr[TTE_SPECIAL] + gid;

	_sbmp8_plot(&tc->dst, tc->cursorX/8, tc->cursorY/8, se);
}

//! Character-plot for affine BGs using an 8x16 font.
void ase_drawg_w8h16(uint gid)
{
	TTC *tc= tte_get_context();
	uint x0= tc->cursorX/8, y0= tc->cursorY/8;
	u8 se= tc->cattr[TTE_SPECIAL] + gid*2;

	_sbmp8_plot(&tc->dst, x0, y0  , se);
	_sbmp8_plot(&tc->dst, x0, y0+1, se+1);
}

//! Character-plot for affine Bgs, any size.
void ase_drawg(uint gid)
{
	TTE_BASE_VARS(tc, font);
	uint charW= (font->cellW+7)/8, charH= (font->cellH+7)/8;
	uint x0= tc->cursorX/8, y0= tc->cursorY/8;

	u8 se= tc->cattr[TTE_SPECIAL] + gid*charW*charH;

	int ix, iy;
	for(iy=0; iy<charH; iy++)
		for(ix=0; ix<charW; ix++, se++)
			_sbmp8_plot(&tc->dst, ix+x0, iy+y0, se);
}

//! Character-plot for affine BGs, any sized,vertically oriented font.
void ase_drawg_s(uint gid)
{
	TTE_BASE_VARS(tc, font);
	uint charW= (font->cellW+7)/8, charH= (font->cellH+7)/8;
	uint x0= tc->cursorX/8, y0= tc->cursorY/8;

	u8 se= tc->cattr[TTE_SPECIAL] + gid*charW*charH;

	int ix, iy;
	for(ix=0; ix<charW; ix++)
		for(iy=0; iy<charH; iy++, se++)
			_sbmp8_plot(&tc->dst, ix+x0, iy+y0, se);
}

// EOF
