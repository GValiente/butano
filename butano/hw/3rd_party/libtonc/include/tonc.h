//
//  Main tonc header
//
//! \file tonc.h
//! \author J Vijn
//! \date 20060508 - 20080825
//
// === NOTES ===


#ifndef TONC_MAIN
#define TONC_MAIN

#ifdef __cplusplus
extern "C" {
#endif

#include "tonc_types.h"
#include "tonc_memmap.h"
#include "tonc_memdef.h"

#include "tonc_bios.h"
#include "tonc_core.h"
#include "tonc_input.h"
#include "tonc_irq.h"
#include "tonc_math.h"
#include "tonc_oam.h"
#include "tonc_tte.h"
#include "tonc_video.h"
#include "tonc_surface.h"

#include "tonc_nocash.h"

#ifdef __cplusplus
};
#endif

// --- Doxygen modules: ---

/*!	\defgroup grpBios	Bios Calls			*/
/*!	\defgroup grpCore	Core				*/
/*! \defgroup grpDma	DMA					*/
/*! \defgroup grpInput	Input				*/
/*! \defgroup grpIrq	Interrupt			*/
/*! \defgroup grpMath	Math				*/
/*!	\defgroup grpMemmap Memory Map			*/
/*! \defgroup grpAudio	Sound				*/
/*! \defgroup grpTTE	Tonc Text Engine	*/
/*! \defgroup grpText	Old Text			*/
/*! \defgroup grpTimer	Timer				*/
/*! \defgroup grpVideo	Video				*/



/*!	\mainpage	Tonclib 1.4 (20080825)
	<p>
	Tonclib is the library accompanying the set of GBA tutorials known 
	as <a href="http://www.coranac.com/tonc/">Tonc</a>  Initially, it 
	was just a handful of macros and functions for dealing with the 
	GBA hardware: the memory map and its bits, affine transformation 
	code and things like that. More recently, more general items 
	have been added like tonccpy() and toncset(), the TSurface system 
	and TTE. All these items should provide a firm basis on which to 
	build GBA software.
	</p>
*/

#endif // TONC_MAIN

