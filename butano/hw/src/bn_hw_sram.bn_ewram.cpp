/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/btn_hw_sram.h"

#include "../include/btn_hw_tonc.h"

namespace btn::hw::sram
{

void write(const void* source, int size, int offset)
{
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram
    // (Reading and Writing section)

    volatile const uint8_t* source_ptr = static_cast<const uint8_t*>(source);
    volatile uint8_t* sram_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + offset;

    for(int i = 0; i < size; i++)
    {
        sram_ptr[i] = source_ptr[i];
    }
}

void read(void* destination, int size, int offset)
{
    // http://problemkaputt.de/gbatek.htm#gbacartbackupsramfram
    // (Reading and Writing section)

    volatile const uint8_t* sram_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM) + offset;
    volatile uint8_t* destination_ptr = static_cast<uint8_t*>(destination);

    for(int i = 0; i < size; i++)
    {
        destination_ptr[i] = sram_ptr[i];
    }
}

}
