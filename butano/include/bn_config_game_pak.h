/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_GAME_PAK_H
#define BN_CONFIG_GAME_PAK_H

/**
 * @file
 * Game Pak configuration header file.
 *
 * @ingroup game_pak
 */

#include "bn_game_pak_wait_state.h"

/**
 * @def BN_CFG_GAME_PAK_PREFETCH_ENABLED
 *
 * Specifies if Game Pak prefetch is enabled or not.
 *
 * When Game Pak prefetch is enabled, the GBA attempts to read opcodes from the Game Pak
 * when the CPU is not using the bus (if any).
 *
 * @ingroup game_pak
 */
#ifndef BN_CFG_GAME_PAK_PREFETCH_ENABLED
    #define BN_CFG_GAME_PAK_PREFETCH_ENABLED true
#endif

/**
 * @def BN_CFG_GAME_PAK_WAIT_STATE_FIRST
 *
 * Specifies Game Pak access time in clock cycles for non-sequential accesses.
 *
 * Values not specified in BN_GAME_PAK_WAIT_STATE_FIRST_* macros are not allowed.
 *
 * @ingroup game_pak
 */
#ifndef BN_CFG_GAME_PAK_WAIT_STATE_FIRST
    #define BN_CFG_GAME_PAK_WAIT_STATE_FIRST BN_GAME_PAK_WAIT_STATE_FIRST_AUTO
#endif

/**
 * @def BN_CFG_GAME_PAK_WAIT_STATE_SECOND
 *
 * Specifies Game Pak access time in clock cycles for sequential accesses.
 *
 * Values not specified in BN_GAME_PAK_WAIT_STATE_SECOND_* macros are not allowed.
 *
 * @ingroup game_pak
 */
#ifndef BN_CFG_GAME_PAK_WAIT_STATE_SECOND
    #define BN_CFG_GAME_PAK_WAIT_STATE_SECOND BN_GAME_PAK_WAIT_STATE_SECOND_AUTO
#endif

#endif
