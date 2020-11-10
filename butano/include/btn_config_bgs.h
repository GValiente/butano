/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_BGS_H
#define BTN_CONFIG_BGS_H

/**
 * @file
 * Backgrounds configuration header file.
 *
 * @ingroup bg
 */

#include "btn_common.h"

/**
 * @def BTN_CFG_BGS_MAX_ITEMS
 *
 * Specifies the maximum number of background items that can be created.
 *
 * A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.
 *
 * @ingroup bg
 */
#ifndef BTN_CFG_BGS_MAX_ITEMS
    #define BTN_CFG_BGS_MAX_ITEMS 4
#endif

#endif
