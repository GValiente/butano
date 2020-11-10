/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_TILES_H
#define BTN_SPRITE_TILES_H

/**
 * @file
 * btn::sprite_tiles header file.
 *
 * @ingroup sprite
 * @ingroup tile
 */

#include "btn_common.h"

/**
 * @brief Sprite tiles related functions.
 *
 * @ingroup sprite
 * @ingroup tile
 */
namespace btn::sprite_tiles
{
    /**
     * @brief Returns the number of used sprite tiles managed with sprite_tiles_ptr objects.
     */
    [[nodiscard]] int used_tiles_count();

    /**
     * @brief Returns the number of available sprite tiles that can be managed with sprite_tiles_ptr objects.
     */
    [[nodiscard]] int available_tiles_count();

    /**
     * @brief Returns the number of used sprite tile sets created with sprite_tiles_ptr static constructors.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available sprite tile sets
     * that can be created with sprite_tiles_ptr static constructors.
     */
    [[nodiscard]] int available_items_count();
}

#endif
