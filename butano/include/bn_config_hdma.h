/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_HDMA_H
#define BN_CONFIG_HDMA_H

/**
 * @file
 * H-Blank direct memory access configuration header file.
 *
 * @ingroup hdma
 */

// Assembler guard
#ifndef __ASSEMBLER__
    #include "bn_common.h"
#endif

/**
 * @def BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED
 *
 * Specifies if the high priority HDMA IRQ is enabled or not.
 *
 * @ingroup hdma
 */
#ifndef BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED
    #define BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED 0
#endif

#endif
