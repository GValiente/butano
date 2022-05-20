// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef INTERRUPTS_H__
#define INTERRUPTS_H__

#include <stddef.h>

#include "definitions.h"

typedef enum {
    IRQ_VBLANK  = 0,
    IRQ_HBLANK  = 1,
    IRQ_VCOUNT  = 2,
    IRQ_TIMER0  = 3,
    IRQ_TIMER1  = 4,
    IRQ_TIMER2  = 5,
    IRQ_TIMER3  = 6,
    IRQ_SERIAL  = 7,
    IRQ_DMA0    = 8,
    IRQ_DMA1    = 9,
    IRQ_DMA2    = 10,
    IRQ_DMA3    = 11,
    IRQ_KEYPAD  = 12,
    IRQ_GAMEPAK = 13,
    IRQ_NUMBER
} irq_index;

typedef void (*irq_vector)(void);

// Initialize global interrupt handling. This is called before reaching
// GBA_main(), so it isn't normally needed to call it.
EXPORT_API void IRQ_Init(void);

// Set interrupt handler for the specified interrupt.
EXPORT_API void IRQ_SetHandler(irq_index index, irq_vector function);

// Enable the specified interrupt. For VBLANK, HBLANK and VCOUNT, this function
// also sets the corresponding flags in DISPSTAT automatically.
EXPORT_API void IRQ_Enable(irq_index index);

// Disable the specified interrupt.
EXPORT_API void IRQ_Disable(irq_index index);

// Set the reference VCOUNT that triggers the VCOUNT interrupt.
EXPORT_API void IRQ_SetReferenceVCOUNT(uint32_t y);

#endif // INTERRUPTS_H__
