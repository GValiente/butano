#ifndef BTN_HW_TIMER_H
#define BTN_HW_TIMER_H

#include "tonc.h"
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

    inline void init()
    {
        REG_TM3CNT = 0;
        REG_TM2CNT = 0;

        REG_TM3D = 0;
        REG_TM2D = 0;

        REG_TM3CNT = TM_ENABLE | TM_CASCADE;
        REG_TM2CNT = TM_ENABLE;
    }

    [[nodiscard]] inline unsigned ticks()
    {
        return (unsigned(REG_TM3D) << 16) | REG_TM2D;
    }
}

#endif
