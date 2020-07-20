
//
// Initializer for 4bpp tiled surfaces, row-major.
//
//! \file tte_init_chr4r.c
//! \author J Vijn
//! \date 20070517 - 20080515
//
/* === NOTES ===
  * 20080315,jv: separated clrids into se0 and cattrs fields.
	se0: screen-entry offset for map
	cattrs: packed BYTES (not nybbles) for color attributes.
*/

#include <string.h>
#include <tonc.h>
#include "tonc_tte.h"

#include "tonc_warn.h"


// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------


//! Initialize text system for 4bpp tiled, column-major surfaces.
/*!	
	\param bgnr		Background number.
	\param bgcnt	Background control flags.
	\param se0		Base offset for screen-entries.
	\param cattrs	Color attributes; one byte per attr.
	\param clrs		ink(/shadow) colors.
	\param font		Font to initialize with.
	\param proc		Glyph renderer
*/
void tte_init_chr4r(int bgnr, u16 bgcnt, u16 se0, u32 cattrs, u32 clrs, 
	const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &vwf_default;
	if(proc==NULL)	proc= chr4r_drawg_default;

	tte_init_base(font, proc, chr4r_erase);
	REG_BGCNT[bgnr]= bgcnt;

	TTC *tc= tte_get_context();
	TSurface *srf= &tc->dst;

	srf_init(srf, SRF_CHR4R, 
		&tile_mem[BFN_GET(bgcnt, BG_CBB)][se0 & SE_ID_MASK], 
		SCREEN_WIDTH, SCREEN_HEIGHT, 4, pal_bg_mem);
	schr4r_prep_map(srf, se_mem[BFN_GET(bgcnt, BG_SBB)], se0);

	tc->flags0= bgnr;
	tc->ctrl= bgcnt;
	REG_BGCNT[bgnr]= bgcnt;

	// --- Init color attribute ---
	u32 ink, shadow, paper, spec;
	ink=	 cattrs     & 15;
	shadow=	(cattrs>> 8)& 15;
	paper=	(cattrs>>16)& 15;
	spec=   (cattrs>>24)&255;

	tc->cattr[TTE_INK]= ink;
	tc->cattr[TTE_SHADOW]= shadow;
	tc->cattr[TTE_PAPER]= paper;
	//tc->cattr[TTE_SPECIAL]= 0;

	srf->palData= pal_bg_bank[se0>>12];
	srf->palData[ink]= clrs&0xFFFF;
	srf->palData[shadow]= clrs>>16;
}


//! Erase part of the 4bpp text canvas.
void chr4r_erase(int left, int top, int right, int bottom)
{	
	TTC *tc= tte_get_context();

	//# Check for single sequence
	schr4r_rect(&tc->dst, left, top, right, bottom, tc->cattr[TTE_PAPER]);
}

// EOF
