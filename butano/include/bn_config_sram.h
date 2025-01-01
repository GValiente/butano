/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_SRAM_H
#define BN_CONFIG_SRAM_H

/**
 * @file
 * SRAM configuration header file.
 *
 * @ingroup sram
 */

#include "bn_sram_wait_state.h"

/**
 * @def BN_CFG_SRAM_WAIT_STATE
 *
 * Specifies SRAM access time in clock cycles.
 *
 * Values not specified in BN_SRAM_WAIT_STATE_* macros are not allowed.
 *
 * @ingroup sram
 */
#ifndef BN_CFG_SRAM_WAIT_STATE
    #define BN_CFG_SRAM_WAIT_STATE BN_SRAM_WAIT_STATE_8
#endif

#endif
