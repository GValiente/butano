/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SPRITE_TILES_H
#define BN_HW_SPRITE_TILES_H

#include "bn_tile.h"
#include "bn_hw_dma.h"
#include "bn_hw_memory.h"

namespace bn::hw::sprite_tiles
{
    namespace
    {
        static_assert(sizeof(TILE) == sizeof(tile));
        static_assert(alignof(TILE) == alignof(tile));

        [[nodiscard]] inline tile* tile_vram(int index)
        {
            return reinterpret_cast<tile*>(MEM_VRAM_OBJ) + index;
        }
    }

    inline void copy_tiles(const tile* source_tiles_ptr, int count, tile* destination_tiles_ptr)
    {
        hw::memory::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), destination_tiles_ptr);
    }

    inline void clear_tiles(int count, tile* tiles_ptr)
    {
        hw::memory::set_words(0, count * int(sizeof(tile) / 4), tiles_ptr);
    }

    [[nodiscard]] inline tile* vram(int index)
    {
        return tile_vram(index);
    }

    inline void commit_with_cpu(const tile* source_tiles_ptr, int index, int count)
    {
        hw::memory::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), tile_vram(index));
    }

    inline void commit_with_dma(const tile* source_tiles_ptr, int index, int count)
    {
        hw::dma::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), tile_vram(index));
    }

    BN_CODE_IWRAM void _plot_hideous_tiles(int width, const unsigned* srcD, int dstX0, unsigned* dstD);

    inline void plot_tiles(int width, const tile* source_tiles_ptr, int source_y, int destination_y,
                           tile* destination_tiles_ptr)
    {
        // From TONC:

        auto srcD = reinterpret_cast<const unsigned*>(source_tiles_ptr + source_y / 8);
        auto dstD = reinterpret_cast<unsigned*>(destination_tiles_ptr + destination_y / 8);

        if(int dstX0 = destination_y & 7)
        {
            // Hideous cases: srcX0 != dstX0:

            _plot_hideous_tiles(width, srcD, dstX0, dstD);
        }
        else
        {
            // Simple cases: aligned pixels:

            hw::memory::copy_words(srcD, 8, dstD);
        }
    }
}

#endif
