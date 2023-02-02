/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
    #define BN_CFG_HBES_MAX_ITEMS 6
#endif

#endif
