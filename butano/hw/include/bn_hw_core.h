/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_CORE_H
#define BN_HW_CORE_H

#include "bn_hw_tonc.h"

namespace bn::hw::core
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
        RegisterRamReset(0xFF);
        SoftReset();

        while(true)
        {
        }
    }
}

#endif
