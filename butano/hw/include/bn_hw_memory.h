/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MEMORY_H
#define BN_HW_MEMORY_H

#include "bn_hw_tonc.h"

namespace bn::hw::memory
{
    void init();

    [[nodiscard]] inline int stack_address()
    {
        unsigned result = 0;
        asm volatile("mov %0, sp" : "=r" (result));
        return int(result);
    }

    [[nodiscard]] int used_stack_iwram(int current_stack_address);

    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] char* ewram_heap_start();

    [[nodiscard]] char* ewram_heap_end();

    inline void copy_bytes(const void* source, int bytes, void* destination)
    {
        tonccpy(destination, source, unsigned(bytes));
    }

    inline void copy_half_words(const void* source, int half_words, void* destination)
    {
        memcpy16(destination, source, unsigned(half_words));
    }

    inline void copy_words(const void* source, int words, void* destination)
    {
        memcpy32(destination, source, unsigned(words));
    }

    inline void set_bytes(uint8_t value, int bytes, void* destination)
    {
        toncset(destination, value, unsigned(bytes));
    }

    inline void set_half_words(uint16_t value, int half_words, void* destination)
    {
        memset16(destination, value, unsigned(half_words));
    }

    inline void set_words(unsigned value, int words, void* destination)
    {
        memset32(destination, value, unsigned(words));
    }
}

#endif
