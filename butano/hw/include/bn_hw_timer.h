/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TIMER_H
#define BN_HW_TIMER_H

#include "bn_hw_tonc.h"

namespace bn::hw::timer
{
    inline void init()
    {
        REG_TM3CNT = 0;
        REG_TM2CNT = 0;

        REG_TM3D = 0;
        REG_TM2D = 0;

        REG_TM3CNT = TM_ENABLE | TM_CASCADE;
        REG_TM2CNT = TM_ENABLE | TM_FREQ_64;
    }

    [[nodiscard]] inline unsigned ticks()
    {
        uint16_t high;
        uint16_t low;

        do
        {
            high = REG_TM3D;
            low = REG_TM2D;
        }
        while(high != REG_TM3D); [[unlikely]]

        return (unsigned(high) << 16) | unsigned(low);
    }
}

#endif
