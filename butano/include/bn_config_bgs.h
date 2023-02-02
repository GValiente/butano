/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_BGS_H
#define BN_CONFIG_BGS_H

/**
 * @file
 * Backgrounds configuration header file.
 *
 * @ingroup bg
 */

#include "bn_common.h"

/**
 * @def BN_CFG_BGS_MAX_ITEMS
 *
 * Specifies the maximum number of background items that can be created.
 *
 * A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.
 *
 * @ingroup bg
 */
#ifndef BN_CFG_BGS_MAX_ITEMS
    #define BN_CFG_BGS_MAX_ITEMS 4
#endif

#endif
