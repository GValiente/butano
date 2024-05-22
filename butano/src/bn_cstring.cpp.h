/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cstring.h"

#include "bn_assert.h"
#include "../hw/include/bn_hw_memory.h"

namespace bn
{

void memcpy(void* destination, const void* source, int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_BASIC_ASSERT(destination, "Destination is null");
    BN_ASSERT(source, "Source is null");

    hw::memory::copy_bytes(source, bytes, destination);
}

void memset(void* destination, uint8_t value, int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_BASIC_ASSERT(destination, "Destination is null");

    hw::memory::set_bytes(value, bytes, destination);
}

void memclear(void* destination, int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_BASIC_ASSERT(destination, "Destination is null");

    hw::memory::set_bytes(0, bytes, destination);
}

}
