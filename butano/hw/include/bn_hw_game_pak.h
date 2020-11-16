/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_GAME_PAK_H
#define BN_HW_GAME_PAK_H

#include "bn_config_game_pak.h"
#include "bn_hw_tonc.h"

namespace bn::hw::game_pak
{
    static_assert(BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_4 ||
            BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_3 ||
            BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_2 ||
            BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_8);

    static_assert(BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_2 ||
            BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_1);

    inline void init()
    {
        BIT_SET(REG_WAITCNT_NV, BN_CFG_GAME_PAK_WAIT_STATE_FIRST);
        BIT_SET(REG_WAITCNT_NV, BN_CFG_GAME_PAK_WAIT_STATE_SECOND);

        if(BN_CFG_GAME_PAK_PREFETCH_ENABLED)
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
