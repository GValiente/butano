/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sram.h"

#include "../hw/include/bn_hw_sram.h"

namespace _bn::sram
{

void unsafe_read(void* destination, int size, int offset)
{
    bn::hw::sram::read(destination, size, offset);
}

void unsafe_write(const void* source, int size, int offset)
{
    bn::hw::sram::write(source, size, offset);
}

}


namespace bn::sram
{

void clear(int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(bytes <= size(), "Bytes is too high: ", bytes);

    hw::sram::set_bytes(0, bytes, 0);
}

void clear(int bytes, int offset)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(bytes + offset <= size(), "Bytes and offset are too high: ", bytes, " - ", offset);

    hw::sram::set_bytes(0, bytes, offset);
}

void set_bytes(uint8_t value, int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(bytes <= size(), "Bytes is too high: ", bytes);

    hw::sram::set_bytes(value, bytes, 0);
}

void set_bytes(uint8_t value, int bytes, int offset)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(bytes + offset <= size(), "Bytes and offset are too high: ", bytes, " - ", offset);

    hw::sram::set_bytes(value, bytes, offset);
}

}
