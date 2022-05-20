// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef UGBA_H__
#define UGBA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "background.h"
#include "bios.h"
#include "bios_wrappers.h"
#include "console.h"
#include "dma.h"
#include "display.h"
#include "debug.h"
#include "definitions.h"
#include "fp_math.h"
#include "hardware.h"
#include "input.h"
#include "interrupts.h"
#include "obj.h"
#include "sound.h"
#include "sram.h"
#include "timer.h"
#include "version.h"
#include "vram.h"

// Initialize library. This function needs to be called at the start of main().
EXPORT_API void UGBA_Init(int *argc, char **argv[]);

#ifndef __GBA__
// Initialize library with no video output (for testing). This function needs to
// be called at the start of main(). Not implemented in GBA as it isn't usedul
// there.
EXPORT_API void UGBA_InitHeadless(int *argc, char **argv[]);
#endif

// This function tries to detect specific flashcarts with special needs and
// returns a value to be written to REG_WAITCNT based on the flashcart. The best
// possible returned value is WAITCNT_OPTIMIZED.
EXPORT_API uint16_t UGBA_FlashcartOptimizedWaitstates(void);

#ifdef __cplusplus
}
#endif

#endif // UGBA_H__
