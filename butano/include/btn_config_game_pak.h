/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_GAME_PAK_H
#define BTN_CONFIG_GAME_PAK_H

#include "btn_game_pak_wait_state.h"

#ifndef BTN_CFG_GAME_PAK_PREFETCH_ENABLED
    #define BTN_CFG_GAME_PAK_PREFETCH_ENABLED true
#endif

#ifndef BTN_CFG_GAME_PAK_WAIT_STATE_FIRST
    #define BTN_CFG_GAME_PAK_WAIT_STATE_FIRST BTN_GAME_PAK_WAIT_STATE_FIRST_4
#endif

#ifndef BTN_CFG_GAME_PAK_WAIT_STATE_SECOND
    #define BTN_CFG_GAME_PAK_WAIT_STATE_SECOND BTN_GAME_PAK_WAIT_STATE_SECOND_2
#endif

#endif
