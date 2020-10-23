/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_GAME_PAK_H
#define BTN_HW_GAME_PAK_H

#include "btn_config_game_pak.h"
#include "btn_hw_tonc.h"

namespace btn::hw::game_pak
{
    static_assert(BTN_CFG_GAME_PAK_WAIT_STATE_FIRST == BTN_GAME_PAK_WAIT_STATE_FIRST_4 ||
            BTN_CFG_GAME_PAK_WAIT_STATE_FIRST == BTN_GAME_PAK_WAIT_STATE_FIRST_3 ||
            BTN_CFG_GAME_PAK_WAIT_STATE_FIRST == BTN_GAME_PAK_WAIT_STATE_FIRST_2 ||
            BTN_CFG_GAME_PAK_WAIT_STATE_FIRST == BTN_GAME_PAK_WAIT_STATE_FIRST_8);

    static_assert(BTN_CFG_GAME_PAK_WAIT_STATE_SECOND == BTN_GAME_PAK_WAIT_STATE_SECOND_2 ||
            BTN_CFG_GAME_PAK_WAIT_STATE_SECOND == BTN_GAME_PAK_WAIT_STATE_SECOND_1);

    inline void init()
    {
        BIT_SET(REG_WAITCNT_NV, BTN_CFG_GAME_PAK_WAIT_STATE_FIRST);
        BIT_SET(REG_WAITCNT_NV, BTN_CFG_GAME_PAK_WAIT_STATE_SECOND);

        if(BTN_CFG_GAME_PAK_PREFETCH_ENABLED)
        {
            BIT_SET(REG_WAITCNT_NV, WS_PREFETCH);
        }
        else
        {
            BIT_CLEAR(REG_WAITCNT_NV, WS_PREFETCH);
        }
    }
}

#endif
