//
//  Basic video functionality
//
//! \file tonc_video.c
//! \author J Vijn
//! \date 20060604 - 20070805
//
// === NOTES ===

#include "tonc_memmap.h"
#include "tonc_core.h"
#include "tonc_video.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Flip the display page.
/*! Toggles the display page in REG_DISPCNT and sets \a vid_page
	to point to the back buffer.
	\return	Current back buffer pointer.
*/
COLOR *vid_flip(void)
{
	vid_page= (COLOR*)((u32)vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPCNT ^= DCNT_PAGE;	// update control register	

	return vid_page;
}

// EOF
