#ifndef BTN_HW_MEMORY_H
#define BTN_HW_MEMORY_H

#include "tonc.h"
#include "btn_common.h"

namespace btn::hw::memory
{
    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] int used_malloc_ewram();

    inline void copy(const void* source, int bytes, void* destination)
    {
        tonccpy(destination, source, unsigned(bytes));
    }

    inline void copy16(const void* source, int half_words, void* destination)
    {
        memcpy16(destination, source, unsigned(half_words));
    }

    inline void copy32(const void* source, int words, void* destination)
    {
        memcpy32(destination, source, unsigned(words));
    }

    inline void set(uint8_t value, int bytes, void* destination)
    {
        toncset(destination, value, unsigned(bytes));
    }

    inline void set16(uint16_t value, int half_words, void* destination)
    {
        memset16(destination, value, unsigned(half_words));
    }

    inline void set32(unsigned value, int words, void* destination)
    {
        memset32(destination, value, unsigned(words));
    }
}

#endif
