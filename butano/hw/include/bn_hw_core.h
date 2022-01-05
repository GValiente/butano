/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_CORE_H
#define BN_HW_CORE_H

#include "bn_hw_tonc.h"

extern "C"
{
    void bn_hw_soft_reset(unsigned reset_flags);
}

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
        bn_hw_soft_reset(0xFF);

        while(true)
        {
        }
    }
}

#endif
