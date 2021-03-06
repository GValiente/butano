/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sram.h"

#include "../include/bn_hw_tonc.h"

namespace bn::hw::sram
{

void _copy(const uint8_t* source, int size, uint8_t* destination)
{
    // This code should go in WRAM:
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram (Reading and Writing section)

    volatile const uint8_t* source_ptr = source;
    volatile uint8_t* destination_ptr = destination;

    for(int i = 0; i < size; i++)
    {
        destination_ptr[i] = source_ptr[i];
    }
}

}
