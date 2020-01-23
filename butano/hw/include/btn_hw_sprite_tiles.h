#ifndef BTN_HW_SPRITE_TILES_H
#define BTN_HW_SPRITE_TILES_H

#include "tonc.h"
#include "btn_tile.h"
#include "btn_memory.h"

namespace btn::hw::sprite_tiles
{
    [[nodiscard]] constexpr int count_per_bank()
    {
        return 512;
    }

    namespace
    {
        static_assert(sizeof(TILE) == sizeof(tile));
        static_assert(alignof(TILE) == alignof(tile));

        tile& tile_vram(int index)
        {
            return reinterpret_cast<tile*>(MEM_VRAM_OBJ)[index];
        }
    }

    inline void copy_tiles(const tile& source_tiles_ref, int count, tile& destination_tiles_ref)
    {
        memory::copy(source_tiles_ref, count, destination_tiles_ref);
    }

    inline void clear_tiles(int count, tile& tiles_ref)
    {
        memory::clear(count, tiles_ref);
    }

    [[nodiscard]] inline tile& vram(int index)
    {
        return tile_vram(index);
    }

    inline void commit(const tile& source_tiles_ref, int index, int count)
    {
        memory::copy(source_tiles_ref, count, tile_vram(index));
    }

    BTN_CODE_IWRAM void plot_tiles(int width, const tile& source_tiles_ref, int source_height, int source_y,
                                   int destination_y, tile& destination_tiles_ref);
}

#endif
