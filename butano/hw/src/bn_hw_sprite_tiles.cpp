/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sprite_tiles.h"

#include "bn_tile.h"
#include "../include/bn_hw_memory.h"

namespace bn::hw::sprite_tiles
{

void plot_tiles(int width, const tile* source_tiles_ptr, int source_height, int source_y, int destination_y,
                tile* destination_tiles_ptr)
{
    // From TONC:

    auto srcD = reinterpret_cast<const unsigned*>(source_tiles_ptr + source_y / 8);
    auto dstD = reinterpret_cast<unsigned*>(destination_tiles_ptr + destination_y / 8);
    int dstX0 = destination_y & 7;

    if(! dstX0)
    {
        // Simple cases : aligned pixels:

        for(int ix = 0; ix < width; ix += 8)
        {
            hw::memory::copy_words(srcD, 8, dstD);
            srcD += source_height;
            dstD += 8;
        }
    }
    else
    {
        // Hideous cases : srcX0 != dstX0:

        _plot_hideous_tiles(width, source_height, srcD, dstX0, dstD);
    }
}

}
