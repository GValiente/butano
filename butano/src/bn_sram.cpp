/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sram.h"

#include "../hw/include/bn_hw_sram.h"

namespace _bn::sram
{

void unsafe_write(const void* source, int size, int offset)
{
    bn::hw::sram::write(source, size, offset);
}

void unsafe_read(void* destination, int size, int offset)
{
    bn::hw::sram::read(destination, size, offset);
}

}
