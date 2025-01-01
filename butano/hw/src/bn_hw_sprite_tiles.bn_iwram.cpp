/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sprite_tiles.h"

namespace bn::hw::sprite_tiles
{

void _plot_hideous_tiles(int width, const unsigned* srcD, int dstX0, unsigned* dstD)
{
    // From TONC:
    // Hideous cases : srcX0 != dstX0:

    auto lmask = [](int left) {
        return 0xFFFFFFFFU << (((left) & 7) * 4);
    };

    auto rmask = [](int right) {
        return 0xFFFFFFFFU >> ((-(right) & 7) * 4);
    };

    unsigned lsl = 4 * unsigned(dstX0);
    unsigned mask = lmask(0);

    if(width < 8)
    {
        mask &= rmask(width);
    }

    if(dstX0 + width <= 8)
    {
        for(int iy = 0; iy < 8; ++iy)
        {
            unsigned px = *srcD++ & mask;

            // Write to left tile only:
            dstD[0] = (dstD[0] &~ (mask << lsl)) | (px << lsl);

            ++dstD;
        }
    }
    else
    {
        unsigned lsr = 32 - lsl;

        for(int iy = 0; iy < 8; ++iy)
        {
            unsigned px = *srcD++ & mask;

            // Write to left tile:
            dstD[0] = (dstD[0] &~ (mask << lsl)) | (px << lsl);

            // Write to right tile:
            dstD[8] = (dstD[8] &~ (mask >> lsr)) | (px >> lsr);

            ++dstD;
        }
    }
}

}
