#ifndef BTN_HW_BG_BLOCKS_CONSTANTS_H
#define BTN_HW_BG_BLOCKS_CONSTANTS_H

#include "btn_tile.h"

namespace btn::bg_maps
{
    [[nodiscard]] constexpr int blocks_count()
    {
        return 32;
    }

    [[nodiscard]] constexpr int cells_count()
    {
        return 32 * 1024;
    }
}

namespace btn::bg_tiles
{
    [[nodiscard]] constexpr int blocks_count()
    {
        return bg_maps::blocks_count();
    }

    [[nodiscard]] constexpr int tiles_count()
    {
        return bg_maps::cells_count() / int(sizeof(tile) / 2);
    }
}

#endif
