#ifndef BTN_HW_SPRITES_CONSTANTS_H
#define BTN_HW_SPRITES_CONSTANTS_H

#include "btn_limits.h"
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
        return btn::numeric_limits<int16_t>::max() - 1;
    }

    [[nodiscard]] constexpr int min_z_order()
    {
        return -max_z_order();
    }

    [[nodiscard]] constexpr int z_orders()
    {
        return btn::numeric_limits<uint16_t>::max();
    }
}

#endif
