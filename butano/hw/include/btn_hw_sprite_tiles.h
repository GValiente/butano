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

    void copy_tiles(const tile& source_tiles_ref, int tiles_count, tile& destination_tiles_ref);

    void clear_tiles(int tiles_count, tile& tiles_ref);

    BTN_CODE_IWRAM void plot_tiles(int width, const tile& source_tiles_ref, int source_height, int source_y,
                                   int destination_y, tile& destination_tiles_ref);

    [[nodiscard]] tile& vram(int tile_index);

    void commit(const tile& source_tiles_ref, int tile_index, int tiles_count);
}

}

#endif
