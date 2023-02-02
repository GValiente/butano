/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TIMER_CONSTANTS_H
#define BN_HW_TIMER_CONSTANTS_H

#include "bn_common.h"

namespace bn::hw::timers
{
    [[nodiscard]] constexpr int divisor()
    {
        return 64;
    }

    [[nodiscard]] constexpr int ticks_per_frame()
    {
        // http://problemkaputt.de/gbatek.htm#lcddimensionsandtimings

        return 280896 / divisor();
    }

    [[nodiscard]] constexpr int ticks_per_vblank()
    {
        // http://problemkaputt.de/gbatek.htm#lcddimensionsandtimings

        return 83776 / divisor();
    }
}

#endif
