#ifndef BTN_HW_CORE_H
#define BTN_HW_CORE_H

#include "tonc.h"
#include "btn_common.h"

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
        RegisterRamReset(RESET_GFX | RESET_REG_SOUND);
        SoftReset();

        while(true)
        {
        }
    }
}

#endif
