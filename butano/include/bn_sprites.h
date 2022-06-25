/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_H
#define BN_SPRITES_H

/**
 * @file
 * bn::sprites header file.
 *
 * @ingroup sprite
 */

#include "../hw/include/bn_hw_sprites_constants.h"

/**
 * @brief Sprites related functions.
 *
 * @ingroup sprite
 */
namespace bn::sprites
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

    /**
     * @brief Returns the number of hardware sprite handles not used by Butano sprites manager.
     *
     * Normally you should not need to reserve handles, but it can be useful for messing with HDMA for example.
     */
    [[nodiscard]] int reserved_handles_count();

    /**
     * @brief Sets the number of hardware sprite handles not used by Butano sprites manager.
     *
     * Normally you should not need to reserve handles, but it can be useful for messing with HDMA for example.
     *
     * Changing reserved handles count reloads the internal attributes of all sprites (including the reserved ones).
     */
    void set_reserved_handles_count(int reserved_handles_count);

    /**
     * @brief Reloads the internal attributes of all sprites (including the reserved ones).
     *
     * Normally you should not need to call this function, but it can be useful after messing with HDMA for example.
     */
    void reload();
}

#endif
