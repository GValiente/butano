//
// Screen-entry glyph renderer initializer
//
//! \file tte_init_se.c
//! \author J Vijn
//! \date 20070628 - 20080229
//
// === NOTES ===

#include <string.h>

#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_bios.h"
#include "tonc_tte.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------


//! Initialize text system for screen-entry fonts.
/*!
	\param bgnr	Number of background to be used for text.
	\param bgcnt	Background control flags.
	\param se0		Base screen entry. This allows a greater range in
	  capabilities, like offset tile-starts and palettes.
	\param clrs		colors to use for the text. The palette entries
	  used depends on \a se0 and \a bupofs.
	\param bupofs	Flags for font bit-unpacking. Basically indicates
	  pixel values (and hence palette use).
	\param font		Font to initialize with.
	\param proc		Glyph renderer.
*/
void tte_init_se(int bgnr, u16 bgcnt, SCR_ENTRY se0, u32 clrs, u32 bupofs, 
	const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &fwf_default;
	if(proc==NULL)	proc= se_drawg_default;

	tte_init_base(font, proc, se_erase);

	TTC *tc= tte_get_context();
	TSurface *srf= &tc->dst;

	srf_init(srf, SRF_BMP16, se_mem[BFN_GET(bgcnt, BG_SBB)],
		32, 32, 16, pal_bg_mem);
	srf_set_pal(srf, pal_bg_mem, 256);

	tc->flags0= bgnr;
	tc->ctrl= bgcnt;
	REG_BGCNT[bgnr]= bgcnt;

	// --- Prep color attributes and palette ---
	uint dstB= (bgcnt & BG_8BPP) ? 8 : 4;
	uint ink, shadow=0, paper;
	uint se= BFN_GET(se0, SE_ID);

	if(dstB == 4)
	{
		u32 pb= BFN_GET(se0, SE_PALBANK)*16;

		ink= pb + ((bupofs+1)&15);
		if( (bupofs&~BUP_ALL_OFS) > 15)
			shadow= pb + ((bupofs>>4)&15);
	}
	else
	{
		se *= 2;	// Correct for tile stride

		ink= (bupofs+1)&255;
		if( (bupofs&~BUP_ALL_OFS) > 255)
			shadow= (bupofs>>8)&255;
	}
	// Set paper entry (PONDER: color too somehow?)
	paper= (bupofs&BUP_ALL_OFS) ? bupofs : 0;
	
	tc->cattr[TTE_INK]= ink;
	tc->cattr[TTE_SHADOW]= shadow;
	tc->cattr[TTE_PAPER]= paper;
	tc->cattr[TTE_SPECIAL]= se0;

	srf->palData[ink]= clrs&0xFFFF;
	if(shadow)
		srf->palData[shadow]= clrs>>16;

	// --- Bitunpack font ---
	void *dstD= &tile_mem[BFN_GET(bgcnt, BG_CBB)][se];
	u32 dstS= font->charCount*font->cellSize;

	BUP bup= { dstS, font->bpp, dstB, bupofs };
	BitUnPack(font->data, dstD, &bup);
}

// EOF
