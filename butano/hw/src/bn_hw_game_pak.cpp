/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_game_pak.h"

#include "bn_config_game_pak.h"
#include "../include/bn_hw_tonc.h"

namespace bn::hw::game_pak
{

static_assert(BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_4 ||
        BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_3 ||
        BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_2 ||
        BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_8 ||
        BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_AUTO);

static_assert(BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_2 ||
        BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_1 ||
        BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_AUTO);

[[nodiscard]] extern BN_CODE_EWRAM bool _slow_game_pak();

bool init()
{
    bool first_auto = BN_CFG_GAME_PAK_WAIT_STATE_FIRST == BN_GAME_PAK_WAIT_STATE_FIRST_AUTO;
    bool second_auto = BN_CFG_GAME_PAK_WAIT_STATE_SECOND == BN_GAME_PAK_WAIT_STATE_SECOND_AUTO;
    bool slow_game_pak = false;

    if(first_auto || second_auto)
    {
        slow_game_pak = _slow_game_pak();
    }

    if(first_auto)
    {
        if(slow_game_pak)
        {
            BIT_SET(REG_WAITCNT_NV, BN_GAME_PAK_WAIT_STATE_FIRST_4);
        }
        else
        {
            BIT_SET(REG_WAITCNT_NV, BN_GAME_PAK_WAIT_STATE_FIRST_3);
        }
    }
    else
    {
        BIT_SET(REG_WAITCNT_NV, BN_CFG_GAME_PAK_WAIT_STATE_FIRST);
    }

    if(second_auto)
    {
        if(slow_game_pak)
        {
            BIT_SET(REG_WAITCNT_NV, BN_GAME_PAK_WAIT_STATE_SECOND_2);
        }
        else
        {
            BIT_SET(REG_WAITCNT_NV, BN_GAME_PAK_WAIT_STATE_SECOND_1);
        }
    }
    else
    {
        BIT_SET(REG_WAITCNT_NV, BN_CFG_GAME_PAK_WAIT_STATE_SECOND);
    }

    if(BN_CFG_GAME_PAK_PREFETCH_ENABLED)
    {
        BIT_SET(REG_WAITCNT_NV, WS_PREFETCH);
    }
    else
    {
        BIT_CLEAR(REG_WAITCNT_NV, WS_PREFETCH);
    }

    return slow_game_pak;
}

}
