/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_UNCOMPRESS_H
#define BN_HW_UNCOMPRESS_H

#include "bn_hw_tonc.h"

namespace bn::hw::uncompress
{
    inline void lz77_wram(const void* src, void* dst)
    {
        LZ77UnCompWram(src, dst);
    }

    inline void lz77_vram(const void* src, void* dst)
    {
        LZ77UnCompVram(src, dst);
    }

    inline void rl_wram(const void* src, void* dst)
    {
        RLUnCompWram(src, dst);
    }

    inline void rl_vram(const void* src, void* dst)
    {
        RLUnCompVram(src, dst);
    }
}

#endif
