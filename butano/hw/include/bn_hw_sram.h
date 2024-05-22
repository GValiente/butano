/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SRAM_H
#define BN_HW_SRAM_H

#include "bn_hw_tonc.h"
#include "../3rd_party/agbabi/include/agbabi.h"

namespace bn::hw::sram
{
    [[nodiscard]] const char* init();

    BN_CODE_EWRAM void _fill(uint8_t value, int size, uint8_t* destination);

    inline void write(const void* source, int size, int offset)
    {
        auto source_ptr = reinterpret_cast<const uint8_t*>(source);
        auto destination_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + offset;
        __agbabi_memcpy1(destination_ptr, source_ptr, unsigned(size));
    }

    inline void read(void* destination, int size, int offset)
    {
        auto source_ptr = reinterpret_cast<const uint8_t*>(MEM_SRAM) + offset;
        auto destination_ptr = reinterpret_cast<uint8_t*>(destination);
        __agbabi_memcpy1(destination_ptr, source_ptr, unsigned(size));
    }

    inline void set_bytes(uint8_t value, int size, int offset)
    {
        auto destination_ptr = reinterpret_cast<uint8_t*>(MEM_SRAM) + offset;
        _fill(value, size, destination_ptr);
    }
}

#endif
