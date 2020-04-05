#ifndef BTN_HW_TIMER_CONSTANTS_H
#define BTN_HW_TIMER_CONSTANTS_H

#include "btn_common.h"

namespace btn::timers
{
    [[nodiscard]] constexpr int ticks_per_frame()
    {
        return 280896 / 64;
    }

    [[nodiscard]] constexpr int ticks_per_second()
    {
        return ticks_per_frame() * 60;
    }
}

#endif
