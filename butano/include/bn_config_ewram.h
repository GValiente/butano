/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_EWRAM_H
#define BN_CONFIG_EWRAM_H

/**
 * @file
 * EWRAM configuration header file.
 *
 * @ingroup memory
 */

#include "bn_ewram_wait_state.h"

/**
 * @def BN_CFG_EWRAM_WAIT_STATE
 *
 * Specifies EWRAM access time in clock cycles.
 *
 * Values not specified in BN_EWRAM_WAIT_STATE_* macros are not allowed.
 *
 * @ingroup memory
 */
#ifndef BN_CFG_EWRAM_WAIT_STATE
    #define BN_CFG_EWRAM_WAIT_STATE BN_EWRAM_WAIT_STATE_2
#endif

#endif
