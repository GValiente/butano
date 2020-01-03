#ifndef BTN_HW_TIMER_H
#define BTN_HW_TIMER_H

#include "btn_common.h"

namespace btn::hw::timer
{
    [[nodiscard]] constexpr int ticks_per_frame()
    {
        return 280896;
    }

    [[nodiscard]] constexpr int ticks_per_second()
    {
        return ticks_per_frame() * 60;
    }

    void init();

    [[nodiscard]] unsigned ticks();
}

#endif
