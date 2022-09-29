/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sprite_tiles.h"

namespace bn::hw::sprite_tiles
{

void plot_tiles(int width, const tile* source_tiles_ptr, int source_y, int destination_y,
                tile* destination_tiles_ptr)
{
    // From TONC:

    auto srcD = reinterpret_cast<const unsigned*>(source_tiles_ptr + source_y / 8);
    auto dstD = reinterpret_cast<unsigned*>(destination_tiles_ptr + destination_y / 8);

    if(int dstX0 = destination_y & 7)
    {
        // Hideous cases : srcX0 != dstX0:

        _plot_hideous_tiles(width, srcD, dstX0, dstD);
    }
    else
    {
        // Simple cases : aligned pixels:

        hw::memory::copy_words(srcD, 8, dstD);
    }
}

}
