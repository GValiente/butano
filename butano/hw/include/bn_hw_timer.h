/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TIMER_H
#define BN_HW_TIMER_H

#include "bn_hw_tonc.h"
#include "bn_hw_timer_constants.h"

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
        BN_BARRIER;

        return (unsigned(REG_TM3D) << 16) | REG_TM2D;
    }
}

#endif
