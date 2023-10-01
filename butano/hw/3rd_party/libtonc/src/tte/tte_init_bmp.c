
//
// Initializer for bitmap text renderers
//
//! \file tte_init_bmp.c
//! \author J Vijn
//! \date 20070517 - 20080229
//
// === NOTES ===

#include <string.h>

#include "tonc_memdef.h"
#include "tonc_core.h"
#include "tonc_video.h"
#include "tonc_tte.h"

#include "tonc_surface.h"


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

//! Initialize text system for bitmap fonts.
/*!
	\param vmode	Video mode (3,4 or 5).
	\param font		Font to initialize with.
	\param proc		Glyph renderer.
*/
void tte_init_bmp(int vmode, const TFont *font, fnDrawg proc)
{
	if(font==NULL)	font= &vwf_default;
	tte_init_base(font, proc, NULL);
	
	TTC *tc= tte_get_context();

	//# PONDER: page flip ?
	switch(vmode)
	{
    /*case 4:
        tc->dst = m4_surface;

		tc->cattr[TTE_INK]= 0xF1;
		tc->cattr[TTE_SHADOW]= 0xF2;
		//tc->cattr[TTE_PAPER]= 0;

		tc->marginRight= M4_WIDTH;
		tc->marginBottom= M4_HEIGHT;

		if(proc == NULL)
			proc= bmp8_drawg_default;
		tc->eraseProc= bmp8_erase;

		pal_bg_mem[0xF1]= CLR_YELLOW;
		pal_bg_mem[0xF2]= CLR_ORANGE;
        pal_bg_mem[0]= CLR_BLACK;
		break;

	case 5:
		tc->dst = m5_surface;

		tc->cattr[TTE_INK]= CLR_YELLOW;
		tc->cattr[TTE_SHADOW]= CLR_ORANGE;
		tc->cattr[TTE_PAPER]= CLR_BLACK;

		tc->marginRight= M5_WIDTH;
		tc->marginBottom= M5_HEIGHT;

		if(proc == NULL)
			proc= bmp16_drawg_default;
		tc->eraseProc= bmp16_erase;
        break;*/

	// Default mode is 3. This covers for not having set 
	// The video mode yet.
	// case 3:
	default:
		tc->dst = m3_surface;

		tc->cattr[TTE_INK]= CLR_YELLOW;
		tc->cattr[TTE_SHADOW]= CLR_ORANGE;
		tc->cattr[TTE_PAPER]= CLR_BLACK;

		tc->marginRight= M3_WIDTH;
		tc->marginBottom= M3_HEIGHT;

		if(proc == NULL)
			proc= bmp16_drawg_default;
		tc->eraseProc= bmp16_erase;
		break;
	}
	
	tc->drawgProc= proc;
}


//! Erase part of the 16bpp text canvas.
void bmp16_erase(int left, int top, int right, int bottom)
{	
	TTC *tc= tte_get_context();

	//# Check for single sequence
	sbmp16_rect(&tc->dst, left, top, right, bottom,	tc->cattr[TTE_PAPER]);
}


// EOF
