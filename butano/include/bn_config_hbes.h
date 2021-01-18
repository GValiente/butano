/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_HBES_H
#define BN_CONFIG_HBES_H

/**
 * @file
 * H-Blank effects configuration header file.
 *
 * @ingroup hblank_effect
 */

// Assembler guard
#ifndef __ASSEMBLER__
#include "bn_common.h"
#endif

/**
 * @def BN_CFG_HBES_MAX_ITEMS
 *
 * Specifies the maximum number of active H-Blank effects.
 *
 * @ingroup hblank_effect
 */
#ifndef BN_CFG_HBES_MAX_ITEMS
    #define BN_CFG_HBES_MAX_ITEMS 8
#endif

/**
 * @def BN_CFG_HBES_MAX_ITEMS_32
 *
 * Specifies the maximum number of active 32-bit H-Blank effects.
 *
 * @ingroup hblank_effect
 */
#ifndef BN_CFG_HBES_MAX_ITEMS_32
    #define BN_CFG_HBES_MAX_ITEMS_32 4
#endif

#if BN_CFG_HBES_MAX_ITEMS_32 > BN_CFG_HBES_MAX_ITEMS
#error BN_CFG_HBES_MAX_ITEMS_32 must not exceed BN_CFG_HBES_MAX_ITEMS!
#endif

#endif
