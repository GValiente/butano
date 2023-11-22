/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_CORE_H
#define BN_HW_CORE_H

#include "bn_hw_tonc.h"

extern "C"
{
    void bn_hw_soft_reset(unsigned reset_flags);

    void bn_hw_hard_reset();
}

namespace bn::hw::core
{
    inline void init()
    {
        while(REG_VCOUNT != 160)
        {
        }

        while(REG_VCOUNT != 161)
        {
        }
    }

    inline void wait_for_vblank()
    {
        if(REG_VCOUNT == 159)
        {
            while(REG_VCOUNT == 159)
            {
            }
        }
        else
        {
            VBlankIntrWait();
        }
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

    [[noreturn]] inline void hard_reset()
    {
        bn_hw_hard_reset();

        while(true)
        {
        }
    }
}

#endif
