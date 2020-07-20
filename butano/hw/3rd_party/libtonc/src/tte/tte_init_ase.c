//
// Affine Screen-entry plotter
//
//! \file tte_init_ase.c
//! \author J Vijn
//! \date 20070701 - 20080515
//
/* === NOTES ===
	* Not exactly pretty implementations here, but they work.
*/


#include <string.h>

#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_bios.h"
#include "tonc_surface.h"

#include "tonc_tte.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

//! Initialize text system for affine screen-entry fonts.
/*!
	\param bgnr		Number of background to be used for text.
	\param bgcnt	Background control flags.
	\param ase0		Base screen entry. This allows a greater range in
	  capabilities, like offset tile-starts.
	\param clrs		colors to use for the text. The palette entries
	  used depends on \a ase0 and \a bupofs.
	\param bupofs	Flags for font bit-unpacking. Basically indicates
	  pixel values (and hence palette use).
	\param font		Font to initialize with.
	\param proc		Character plotting procedure.
*/
void tte_init_ase(int bgnr, u16 bgcnt, u8 ase0, u32 clrs, u32 bupofs, 
	const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &fwf_default;
	if(proc==NULL)	proc= ase_drawg_default;

	tte_init_base(font, proc, ase_erase);

	TTC *tc= tte_get_context();
	uint size= 16<<BFN_GET(bgcnt, BG_SIZE);

	srf_init(&tc->dst, SRF_BMP8, se_mem[BFN_GET(bgcnt, BG_SBB)],
		size, size, 8, pal_bg_mem);

	tc->marginRight= size*8;
	tc->marginBottom= size*8;

	tc->flags0= bgnr;
	tc->ctrl= bgcnt;
	REG_BGCNT[bgnr]= bgcnt;

	// --- Prep color-lut and palette ---
	u32 ink, shadow=0, paper;

	ink= (bupofs+1)&255;
	if( (bupofs&~BUP_ALL_OFS) > 255)
		shadow= (bupofs>>8)&255;

	// Set paper entry (PONDER: color too somehow?)
	paper= (bupofs&BUP_ALL_OFS) ? bupofs : 0;

	tc->cattr[TTE_INK]= ink;
	tc->cattr[TTE_SHADOW]= shadow;	
	tc->cattr[TTE_PAPER]= paper;
	tc->cattr[TTE_SPECIAL]= ase0;

	pal_bg_mem[ink]= clrs&0xFFFF;	
	pal_bg_mem[shadow]= clrs>>16;
				
	// --- Bup font ---
	void *dstD= &tile_mem[BFN_GET(bgcnt, BG_CBB)][2*ase0];
	u32 dstS= font->charCount*font->cellSize;

	BUP bup= { dstS, font->bpp, 8, bupofs };
	BitUnPack(font->data, dstD, &bup);
}

// EOF
