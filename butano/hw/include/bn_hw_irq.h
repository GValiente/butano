/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_IRQ_H
#define BN_HW_IRQ_H

#include "bn_common.h"

extern "C"
{
    #include "../3rd_party/libugba/include/ugba/interrupts.h"
}

namespace bn::hw::irq
{
    enum class id
    {
        VBLANK,
        HBLANK,
        VCOUNT,
        TIMER0,
        TIMER1,
        TIMER2,
        TIMER3,
        SERIAL,
        DMA0,
        DMA1,
        DMA2,
        DMA3,
        KEYPAD,
        GAMEPAK
    };

    inline void init()
    {
        IRQ_Init();
    }

    inline void set_isr(id irq_id, void(*isr)())
    {
        IRQ_SetHandler(irq_index(irq_id), isr);
    }

    inline void enable(id irq_id)
    {
        IRQ_Enable(irq_index(irq_id));
    }

    inline void disable(id irq_id)
    {
        IRQ_Disable(irq_index(irq_id));
    }
}

#endif
