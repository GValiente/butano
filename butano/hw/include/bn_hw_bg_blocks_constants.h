/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BG_BLOCKS_CONSTANTS_H
#define BN_HW_BG_BLOCKS_CONSTANTS_H

#include "bn_tile.h"

namespace bn::hw::bg_maps
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

namespace bn::hw::bg_tiles
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
