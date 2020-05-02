#ifndef BTN_HW_SPRITES_CONSTANTS_H
#define BTN_HW_SPRITES_CONSTANTS_H

#include "btn_sort_key.h"
#include "btn_config_sprites.h"

namespace btn::sprites
{
    [[nodiscard]] constexpr int sprites_count()
    {
        return 128;
    }

    [[nodiscard]] constexpr int items_count()
    {
        return BTN_CFG_SPRITES_MAX_ITEMS;
    }

    [[nodiscard]] constexpr int max_bg_priority()
    {
        return 3;
    }

    [[nodiscard]] constexpr int max_z_order()
    {
        return sort_key::max_z_order();
    }

    [[nodiscard]] constexpr int min_z_order()
    {
        return sort_key::min_z_order();
    }
}

#endif
