// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef INTERRUPTS_H__
#define INTERRUPTS_H__

#include <stddef.h>

// #include "definitions.h"
#define EXPORT_API

typedef enum {
    UGBA_IRQ_VBLANK  = 0,
    UGBA_IRQ_HBLANK  = 1,
    UGBA_IRQ_VCOUNT  = 2,
    UGBA_IRQ_TIMER0  = 3,
    UGBA_IRQ_TIMER1  = 4,
    UGBA_IRQ_TIMER2  = 5,
    UGBA_IRQ_TIMER3  = 6,
    UGBA_IRQ_SERIAL  = 7,
    UGBA_IRQ_DMA0    = 8,
    UGBA_IRQ_DMA1    = 9,
    UGBA_IRQ_DMA2    = 10,
    UGBA_IRQ_DMA3    = 11,
    UGBA_IRQ_KEYPAD  = 12,
    UGBA_IRQ_GAMEPAK = 13,
    UGBA_IRQ_NUMBER
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
