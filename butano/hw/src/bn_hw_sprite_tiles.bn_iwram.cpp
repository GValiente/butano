/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sprite_tiles.h"

namespace bn::hw::sprite_tiles
{

void _plot_hideous_tiles(int width, int source_height, const unsigned* srcD, int dstX0, unsigned* dstD)
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
    unsigned lsr = 32 - lsl;
    unsigned mask = lmask(0);

    for(int ix = width; ix > 0; ix -= 8)
    {
        if(ix < 8)
        {
            mask &= rmask(ix);
        }

        unsigned* dstL = dstD;
        dstD += 8;

        const unsigned* srcL = srcD;
        srcD += source_height;

        for(int iy = 0; iy < 8; ++iy)
        {
            unsigned px = *srcL++ & mask;

            // Write to left tile:
            dstL[0] = (dstL[0] &~ (mask << lsl)) | (px << lsl);

            // Write to right tile:
            dstL[8] = (dstL[8] &~ (mask >> lsr)) | (px >> lsr);

            ++dstL;
        }

        mask = 0xFFFFFFFF;
    }
}

}
