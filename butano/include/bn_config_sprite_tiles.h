/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_SPRITE_TILES_H
#define BN_CONFIG_SPRITE_TILES_H

/**
 * @file
 * Sprite tiles configuration header file.
 *
 * @ingroup sprite
 */

#include "bn_common.h"

/**
 * @def BN_CFG_SPRITE_TILES_MAX_ITEMS
 *
 * Specifies the maximum number of sprite tile sets that can be created with
 * bn::sprite_tiles_ptr static constructors.
 *
 * @ingroup sprite
 */
#ifndef BN_CFG_SPRITE_TILES_MAX_ITEMS
    #define BN_CFG_SPRITE_TILES_MAX_ITEMS 128
#endif

/**
 * @def BN_CFG_SPRITE_TILES_LOG_ENABLED
 *
 * Specifies if the sprite tiles manager must be logged in detail or not.
 *
 * Keep in mind that it is very slow, it is useful only for debugging.
 *
 * @ingroup sprite
 */
#ifndef BN_CFG_SPRITE_TILES_LOG_ENABLED
    #define BN_CFG_SPRITE_TILES_LOG_ENABLED false
#endif

#endif
