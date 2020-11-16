/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_SPRITE_TILES_H
#define BTN_HW_SPRITE_TILES_H

#include "btn_tile.h"
#include "btn_memory.h"
#include "btn_hw_tonc.h"

namespace btn::hw::sprite_tiles
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
        memory::copy(*source_tiles_ptr, count, *destination_tiles_ptr);
    }

    inline void clear_tiles(int count, tile* tiles_ptr)
    {
        memory::clear(count, *tiles_ptr);
    }

    [[nodiscard]] inline tile* vram(int index)
    {
        return tile_vram(index);
    }

    inline void commit(const tile* source_tiles_ptr, int index, int count)
    {
        memory::copy(*source_tiles_ptr, count, *tile_vram(index));
    }

    BTN_CODE_IWRAM void plot_tiles(int width, const tile* source_tiles_ptr, int source_height, int source_y,
                                   int destination_y, tile* destination_tiles_ptr);
}

#endif
