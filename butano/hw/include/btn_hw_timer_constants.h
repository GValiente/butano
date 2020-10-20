/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_TIMER_CONSTANTS_H
#define BTN_HW_TIMER_CONSTANTS_H

#include "btn_common.h"

namespace btn::timers
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
