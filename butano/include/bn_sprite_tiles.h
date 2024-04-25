/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_TILES_H
#define BN_SPRITE_TILES_H

/**
 * @file
 * bn::sprite_tiles header file.
 *
 * @ingroup sprite
 * @ingroup tile
 */

#include "bn_common.h"

/**
 * @brief Sprite tiles related functions.
 *
 * @ingroup sprite
 * @ingroup tile
 */
namespace bn::sprite_tiles
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

    /**
     * @brief Logs the current status of the sprite tiles manager.
     */
    void log_status();
}

#endif
