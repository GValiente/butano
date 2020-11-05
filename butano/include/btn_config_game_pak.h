/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_GAME_PAK_H
#define BTN_CONFIG_GAME_PAK_H

#include "btn_game_pak_wait_state.h"

/**
 * @def BTN_CFG_GAME_PAK_PREFETCH_ENABLED
 *
 * Specifies if Game Pak prefetch is enabled or not.
 *
 * When Game Pak prefetch is enabled, the GBA attempts to read opcodes from the Game Pak
 * when the CPU is not using the bus (if any).
 *
 * @ingroup game_pak
 */
#ifndef BTN_CFG_GAME_PAK_PREFETCH_ENABLED
    #define BTN_CFG_GAME_PAK_PREFETCH_ENABLED true
#endif

/**
 * @def BTN_CFG_GAME_PAK_WAIT_STATE_FIRST
 *
 * Specifies Game Pak access time in clock cycles for non-sequential accesses.
 *
 * Values not specified in BTN_GAME_PAK_WAIT_STATE_FIRST_* macros are not allowed.
 *
 * @ingroup game_pak
 */
#ifndef BTN_CFG_GAME_PAK_WAIT_STATE_FIRST
    #define BTN_CFG_GAME_PAK_WAIT_STATE_FIRST BTN_GAME_PAK_WAIT_STATE_FIRST_4
#endif

/**
 * @def BTN_CFG_GAME_PAK_WAIT_STATE_SECOND
 *
 * Specifies Game Pak access time in clock cycles for sequential accesses.
 *
 * Values not specified in BTN_GAME_PAK_WAIT_STATE_SECOND_* macros are not allowed.
 *
 * @ingroup game_pak
 */
#ifndef BTN_CFG_GAME_PAK_WAIT_STATE_SECOND
    #define BTN_CFG_GAME_PAK_WAIT_STATE_SECOND BTN_GAME_PAK_WAIT_STATE_SECOND_2
#endif

#endif
