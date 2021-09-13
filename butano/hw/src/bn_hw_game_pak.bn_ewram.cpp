/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_game_pak.h"

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

namespace
{
    const unsigned rom_data[] = { 0x12345678, 0x00042069, 0xdeadbeef };
}

bool _slow_game_pak()
{
    unsigned a[3];
    unsigned b[3];
    unsigned c[3];

    for(int index = 0; index < 3; ++index)
    {
        a[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = WS_STANDARD;

    for(int index = 0; index < 3; ++index)
    {
        b[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = WS_STANDARD;

    for (int index = 2; index >= 0; --index)
    {
        c[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = 0;

    return a[0] != b[0] || a[1] != b[1] || a[2] != b[2] || a[0] != c[0] || a[1] != c[1] || a[2] != c[2];
}

}
