/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_game_pak.h"

#include "../include/bn_hw_tonc.h"

namespace bn::hw::game_pak
{

namespace
{
    const unsigned rom_data[] = { 0x12345678, 0x00042069, 0xdeadbeef };
}

int _slow_game_pak()
{
    unsigned a[3];
    unsigned b[3];
    unsigned c[3];

    uint16_t old_waitcnt = REG_WAITCNT;
    REG_WAITCNT = 0;

    if(REG_WAITCNT != 0)
    {
        return 2;
    }

    for(int index = 0; index < 3; ++index)
    {
        a[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = WS_STANDARD;

    if(REG_WAITCNT != WS_STANDARD)
    {
        return 2;
    }

    for(int index = 0; index < 3; ++index)
    {
        b[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = WS_STANDARD;

    if(REG_WAITCNT != WS_STANDARD)
    {
        return 2;
    }

    for (int index = 2; index >= 0; --index)
    {
        c[index] = *(volatile unsigned*)(&rom_data[index]);
    }

    REG_WAITCNT = old_waitcnt;

    bool slow = a[0] != b[0] || a[1] != b[1] || a[2] != b[2] || a[0] != c[0] || a[1] != c[1] || a[2] != c[2];
    return slow ? 1 : 0;
}

}
