/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_memory.h"

extern "C"
{
    void* memset(void* dest, int ch, std::size_t count)
    {
        bn::hw::memory::set_bytes(uint8_t(ch), int(count), dest);
        return dest;
    }
}
