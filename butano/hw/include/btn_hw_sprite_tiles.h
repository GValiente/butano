#ifndef BTN_HW_SPRITE_TILES_H
#define BTN_HW_SPRITE_TILES_H

#include "btn_common.h"

namespace btn
{

class tile;

namespace hw::sprite_tiles
{
    [[nodiscard]] constexpr int count_per_bank()
    {
        return 512;
    }

    void copy_tiles(const tile* source_tiles_ptr, int tiles_count, tile* destination_tiles_ptr);

    void clear_tiles(int tiles_count, tile* output_tiles_ptr);

    [[nodiscard]] tile* vram(int tile_index);

    void commit(const tile* source_tiles_ptr, int tile_index, int tiles_count);
}

}

#endif
