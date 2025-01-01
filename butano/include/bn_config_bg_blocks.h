/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_BG_BLOCKS_H
#define BN_CONFIG_BG_BLOCKS_H

/**
 * @file
 * Background memory blocks configuration header file.
 *
 * @ingroup bg
 */

#include "bn_common.h"

/**
 * @def BN_CFG_BG_BLOCKS_MAX_ITEMS
 *
 * Specifies the maximum number of background tile sets and maps that can be created.
 *
 * @ingroup bg
 */
#ifndef BN_CFG_BG_BLOCKS_MAX_ITEMS
    #define BN_CFG_BG_BLOCKS_MAX_ITEMS 16
#endif

/**
 * @def BN_CFG_BG_BLOCKS_LOG_ENABLED
 *
 * Specifies if the background blocks manager must be logged in detail or not.
 *
 * Keep in mind that it is very slow, it is useful only for debugging.
 *
 * @ingroup bg
 */
#ifndef BN_CFG_BG_BLOCKS_LOG_ENABLED
    #define BN_CFG_BG_BLOCKS_LOG_ENABLED false
#endif

/**
 * @def BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED
 *
 * Specifies if the background blocks manager sanity check is enabled or not.
 *
 * Sanity check asserts if the internal state of the manager is valid.
 *
 * @ingroup bg
 */
#ifndef BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED
    #define BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED false
#endif

#endif
