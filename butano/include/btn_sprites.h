/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITES_H
#define BTN_SPRITES_H

#include "../hw/include/btn_hw_sprites_constants.h"

/**
 * @brief Sprites related functions.
 *
 * @ingroup sprite
 */
namespace btn::sprites
{
    /**
     * @brief Returns the number of used sprite items created with sprite_ptr static constructors.
     *
     * A sprite item which is outside of the screen or hidden is not committed to the GBA,
     * so there can be more than 128 sprite items.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available sprite items that can be created with sprite_ptr static constructors.
     *
     * A sprite item which is outside of the screen or hidden is not committed to the GBA,
     * so there can be more than 128 sprite items.
     */
    [[nodiscard]] int available_items_count();

    /**
     * @return Returns the minimum priority of a sprite relative to backgrounds.
     */
    [[nodiscard]] constexpr int min_bg_priority()
    {
        return hw::sprites::min_bg_priority();
    }

    /**
     * @return Returns the maximum priority of a sprite relative to backgrounds.
     */
    [[nodiscard]] constexpr int max_bg_priority()
    {
        return hw::sprites::max_bg_priority();
    }

    /**
     * @return Returns the minimum priority of a sprite relative to other sprites.
     */
    [[nodiscard]] constexpr int min_z_order()
    {
        return -32767;
    }

    /**
     * @return Returns the maximum priority of a sprite relative to other sprites.
     */
    [[nodiscard]] constexpr int max_z_order()
    {
        return 32767;
    }
}

#endif
