#ifndef BTN_HW_MEMORY_H
#define BTN_HW_MEMORY_H

#include "btn_common.h"

namespace btn::hw::memory
{
    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] int used_malloc_ewram();

    void copy(const void* source, int bytes, void* destination);

    void copy16(const void* source, int half_words, void* destination);

    void copy32(const void* source, int words, void* destination);

    void set(uint8_t value, int bytes, void* destination);

    void set16(uint16_t value, int half_words, void* destination);

    void set32(unsigned value, int words, void* destination);
}

#endif
