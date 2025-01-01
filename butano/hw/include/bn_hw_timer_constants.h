/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TIMER_CONSTANTS_H
#define BN_HW_TIMER_CONSTANTS_H

#include "bn_config_timer.h"

namespace bn::hw::timers
{
    [[nodiscard]] constexpr int divisor()
    {
        return BN_CFG_TIMER_FREQUENCY;
    }

    [[nodiscard]] constexpr int ticks_per_frame()
    {
        // http://problemkaputt.de/gbatek.htm#lcddimensionsandtimings

        return 280896 / divisor();
    }

    [[nodiscard]] constexpr int ticks_per_second()
    {
        // http://problemkaputt.de/gbatek.htm#lcddimensionsandtimings

        return (16 * 1024 * 1024) / divisor();
    }

    [[nodiscard]] constexpr int ticks_per_vblank()
    {
        // http://problemkaputt.de/gbatek.htm#lcddimensionsandtimings

        return 83776 / divisor();
    }
}

#endif
