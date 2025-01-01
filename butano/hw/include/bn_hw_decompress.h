/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DECOMPRESS_H
#define BN_HW_DECOMPRESS_H

#include "bn_common.h"
#include "../3rd_party/cult-of-gba-bios/include/cult-of-gba-bios.h"

namespace bn::hw::decompress
{
    inline void lz77(const void* src, void* dst)
    {
        swi_LZ77UnCompWrite16bit(src, dst);
    }

    inline void rl_wram(const void* src, void* dst)
    {
        swi_RLUnCompReadNormalWrite8bit(src, dst);
    }

    inline void rl_vram(const void* src, void* dst)
    {
        swi_RLUnCompReadNormalWrite16bit(src, dst);
    }

    inline void huff(const void* src, void* dst)
    {
        swi_HuffUnCompReadNormal(src, dst);
    }
}

#endif
