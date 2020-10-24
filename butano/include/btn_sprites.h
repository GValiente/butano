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
     * A sprite item which is outside of the screen is not committed to the GBA,
     * so there can be more than 128 sprite items.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available sprite items that can be created with sprite_ptr static constructors.
     *
     * A sprite item which is outside of the screen is not committed to the GBA,
     * so there can be more than 128 sprite items.
     */
    [[nodiscard]] int available_items_count();
}

#endif
