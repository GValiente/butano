/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_TILES_H
#define BTN_SPRITE_TILES_H

#include "btn_config_sprite_tiles.h"
#include "../hw/include/btn_hw_sprite_tiles_constants.h"

/**
 * @brief Sprite tiles related functions.
 *
 * @ingroup sprite
 * @ingroup tile
 */
namespace btn::sprite_tiles
{
    /**
     * @brief Returns the total number of sprite tiles that can be managed with sprite_tiles_ptr objects.
     */
    [[nodiscard]] constexpr int tiles_count()
    {
        return hw::sprite_tiles::tiles_count();
    }

    /**
     * @brief Returns the number of used sprite tiles managed with sprite_tiles_ptr objects.
     */
    [[nodiscard]] int used_tiles_count();

    /**
     * @brief Returns the number of available sprite tiles that can be managed with sprite_tiles_ptr objects.
     */
    [[nodiscard]] int available_tiles_count();

    /**
     * @brief Returns the total number of sprite tiles items
     * that can be created with sprite_tiles_ptr static constructors.
     */
    [[nodiscard]] constexpr int items_count()
    {
        return BTN_CFG_SPRITE_TILES_MAX_ITEMS;
    }

    /**
     * @brief Returns the number of used sprite tiles items created with sprite_tiles_ptr static constructors.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available sprite tiles items
     * that can be created with sprite_tiles_ptr static constructors.
     */
    [[nodiscard]] int available_items_count();
}

#endif
