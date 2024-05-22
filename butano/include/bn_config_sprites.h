/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_SPRITES_H
#define BN_CONFIG_SPRITES_H

/**
 * @file
 * Sprites configuration header file.
 *
 * @ingroup sprite
 */

#include "bn_common.h"

/**
 * @def BN_CFG_SPRITES_MAX_ITEMS
 *
 * Specifies the maximum number of sprite items that can be created with bn::sprite_ptr static constructors.
 *
 * A sprite item which is outside of the screen or hidden is not committed to the GBA,
 * so there can be more than 128 sprite items.
 *
 * @ingroup sprite
 */
#ifndef BN_CFG_SPRITES_MAX_ITEMS
    #define BN_CFG_SPRITES_MAX_ITEMS 128
#endif

/**
 * @def BN_CFG_SPRITES_MAX_SORT_LAYERS
 *
 * Specifies the maximum number of used sprite sort layers.
 *
 * Sprites are grouped in layers depending of their background priority and z order,
 * so to reduce memory usage and improve performance, please use as less unique z orders as possible.
 *
 * @ingroup sprite
 */
#ifndef BN_CFG_SPRITES_MAX_SORT_LAYERS
    #define BN_CFG_SPRITES_MAX_SORT_LAYERS 16
#endif

#endif
