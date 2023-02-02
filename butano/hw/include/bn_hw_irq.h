/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_IRQ_H
#define BN_HW_IRQ_H

#include "bn_common.h"

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

    void init();

    void set_isr(id irq_id, void(*isr)());

    void enable(id irq_id);

    void disable(id irq_id);
}

#endif
