/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sram.h"

#include "bn_config_sram.h"
#include "../include/bn_hw_tonc.h"

namespace bn::hw::sram
{

static_assert(BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_4 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_3 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_2 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_8);

namespace
{
    // https://forum.gbadev.org/viewtopic.php?f=4&t=2825
    alignas(int) __attribute__((used)) const char save_type[] = "SRAM_V113";
}

void init()
{
    BIT_SET(REG_WAITCNT_NV, BN_CFG_SRAM_WAIT_STATE);
}

void write(const void* source, int size, int offset)
{
    // Maybe it should go in WRAM, but doing that crashes No$gba:
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram (Reading and Writing section)

    volatile const uint8_t* source_ptr = static_cast<const uint8_t*>(source);
    volatile uint8_t* sram_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + offset;

    for(int i = 0; i < size; i++)
    {
        sram_ptr[i] = source_ptr[i];
    }
}

void read(void* destination, int size, int offset)
{
    // Maybe it should go in WRAM, but doing that crashes No$gba:
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram (Reading and Writing section)

    volatile const uint8_t* sram_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM) + offset;
    volatile uint8_t* destination_ptr = static_cast<uint8_t*>(destination);

    for(int i = 0; i < size; i++)
    {
        destination_ptr[i] = sram_ptr[i];
    }
}

}
