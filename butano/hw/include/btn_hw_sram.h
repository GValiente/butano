#ifndef BTN_HW_SRAM_H
#define BTN_HW_SRAM_H

#include "btn_common.h"

namespace btn::hw::sram
{
    [[nodiscard]] constexpr int unused_bytes()
    {
        return sizeof(int);
    }

    [[nodiscard]] constexpr int max_size()
    {
        return (32 * 1024) - (unused_bytes() * 2);
    }

    void init();

    void write(const void* source, int size, int offset);

    void read(void* destination, int size, int offset);
}

#endif
