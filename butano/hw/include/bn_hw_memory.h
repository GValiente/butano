/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MEMORY_H
#define BN_HW_MEMORY_H

#include "bn_hw_tonc.h"
#include "../3rd_party/agbabi/include/aeabi.h"
#include "../3rd_party/agbabi/include/agbabi.h"

namespace bn::hw::memory
{
    void init();

    [[nodiscard]] inline int stack_address()
    {
        unsigned result = 0;
        asm volatile("mov %0, sp" : "=r" (result));
        return int(result);
    }

    [[nodiscard]] inline bool in_vram(void* ptr)
    {
        return int(ptr) >= MEM_PAL && int(ptr) < MEM_ROM;
    }

    [[nodiscard]] int used_stack_iwram(int current_stack_address);

    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] char* ewram_heap_start();

    [[nodiscard]] char* ewram_heap_end();

    inline void copy_bytes(const void* source, int bytes, void* destination)
    {
        __aeabi_memcpy(destination, source, size_t(bytes));
    }

    inline void copy_half_words(const void* source, int half_words, void* destination)
    {
        __agbabi_memcpy2(destination, source, size_t(half_words) * 2);
    }

    inline void copy_words(const void* source, int words, void* destination)
    {
        __aeabi_memcpy4(destination, source, size_t(words) * 4);
    }

    inline void set_bytes(uint8_t value, int bytes, void* destination)
    {
        __aeabi_memset(destination, size_t(bytes), int(value));
    }

    inline void set_half_words(uint16_t value, int half_words, void* destination)
    {
        memset16(destination, value, unsigned(half_words));
    }

    inline void set_words(unsigned value, int words, void* destination)
    {
        __agbabi_wordset4(destination, size_t(words) * 4, int(value));
    }
}

#endif
