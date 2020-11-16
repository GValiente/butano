/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_SPRITE_TILES_H
#define BTN_CONFIG_SPRITE_TILES_H

/**
 * @file
 * Sprite tiles configuration header file.
 *
 * @ingroup sprite
 */

#include "btn_common.h"

/**
 * @def BTN_CFG_SPRITE_TILES_MAX_ITEMS
 *
 * Specifies the maximum number of sprite tile sets that can be created with
 * btn::sprite_tiles_ptr static constructors.
 *
 * @ingroup sprite
 */
#ifndef BTN_CFG_SPRITE_TILES_MAX_ITEMS
    #define BTN_CFG_SPRITE_TILES_MAX_ITEMS 128
#endif

/**
 * @def BTN_CFG_SPRITE_TILES_LOG_ENABLED
 *
 * Specifies if the sprite tiles manager must be logged in detail or not.
 *
 * Keep in mind that it is very slow, it is useful only for debugging.
 *
 * @ingroup sprite
 */
#ifndef BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #define BTN_CFG_SPRITE_TILES_LOG_ENABLED false
#endif

#endif
