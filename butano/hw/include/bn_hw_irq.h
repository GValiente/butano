/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_IRQ_H
#define BN_HW_IRQ_H

#include "bn_hw_tonc.h"

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
        irq_init(nullptr);
    }

    inline void replace_or_push_back(id irq_id, void(*isr)())
    {
        irq_add(eIrqIndex(irq_id), isr);
    }

    inline void remove(id irq_id)
    {
        irq_delete(eIrqIndex(irq_id));
    }

    inline void enable(id irq_id)
    {
        irq_enable(eIrqIndex(irq_id));
    }

    inline void disable(id irq_id)
    {
        irq_disable(eIrqIndex(irq_id));
    }
}

#endif
