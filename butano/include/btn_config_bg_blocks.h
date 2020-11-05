/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_BG_BLOCKS_H
#define BTN_CONFIG_BG_BLOCKS_H

#include "btn_common.h"

/**
 * @def BTN_CFG_BG_BLOCKS_MAX_ITEMS
 *
 * Specifies the maximum number of background tile sets and maps that can be created.
 *
 * @ingroup bg
 */
#ifndef BTN_CFG_BG_BLOCKS_MAX_ITEMS
    #define BTN_CFG_BG_BLOCKS_MAX_ITEMS 16
#endif

/**
 * @def BTN_CFG_BG_BLOCKS_LOG_ENABLED
 *
 * Specifies if the background blocks manager must be logged in detail or not.
 *
 * Keep in mind that it is very slow, it is useful only for debugging.
 *
 * @ingroup bg
 */
#ifndef BTN_CFG_BG_BLOCKS_LOG_ENABLED
    #define BTN_CFG_BG_BLOCKS_LOG_ENABLED false
#endif

#endif
