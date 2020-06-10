#ifndef BTN_HW_SPRITE_TILES_CONSTANTS_H
#define BTN_HW_SPRITE_TILES_CONSTANTS_H

#include "btn_config_sprite_tiles.h"

namespace btn::sprite_tiles
{
    [[nodiscard]] constexpr int tiles_count()
    {
        return 1024;
    }

    [[nodiscard]] constexpr int items_count()
    {
        return BTN_CFG_SPRITE_TILES_MAX_ITEMS;
    }
}

#endif
