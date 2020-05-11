#ifndef BTN_HW_CORE_H
#define BTN_HW_CORE_H

#include "btn_hw_tonc.h"

extern "C"
{
    void btn_hw_soft_reset(unsigned reset_flags);
}

namespace btn::hw::core
{
    inline void wait_for_vblank()
    {
        VBlankIntrWait();
    }

    inline void sleep()
    {
        Stop();
    }

    [[noreturn]] inline void reset()
    {
        btn_hw_soft_reset(0xFF);

        while(true)
        {
        }
    }
}

#endif
