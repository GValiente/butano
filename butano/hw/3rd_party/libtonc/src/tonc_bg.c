//
//  Tiled background stuff
//
//! \file tonc_bg.c
//! \author J Vijn
//! \date 20061112 - 20061117
//
// === NOTES ===
// * Empty, isn't it :P


#include "tonc_memmap.h"
#include "tonc_video.h"


// --------------------------------------------------------------------
// FUNCTIONS 
// --------------------------------------------------------------------


//! Create a framed rectangle.
/*! In contrast to se_frame(), se_frame_ex() uses nine tiles starting at 
	\a se0 for the frame, which indicate the borders and center for the
	window.
	\note	Rectangle is nor normalized.
*/
void se_window(SCR_ENTRY *sbb, int left, int top, int right, int bottom, 
	SCR_ENTRY se0)
{
	int ix, iy;
	right -= (left+1);
	bottom -= (top+1);

	sbb += top*32+left;

	sbb[              0]= se0;
	sbb[          right]= se0+2;
	sbb[bottom*32      ]= se0+6;
	sbb[bottom*32+right]= se0+8;

	// horizontal
	for(ix=1; ix<right; ix++)
	{
		sbb[          ix]= se0+1;
		sbb[bottom*32+ix]= se0+7;
	}

	// vertical + inside
	sbb += 32;
	for(iy=1; iy<bottom; iy++)
	{
		sbb[ 0]= se0+3;
		for(ix=1; ix<right; ix++)
			sbb[ix]= se0+4;
		sbb[right]= se0+5;
		sbb += 32;
	}
}


// EOF
