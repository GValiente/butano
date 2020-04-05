#ifndef BTN_HW_SRAM_CONSTANTS_H
#define BTN_HW_SRAM_CONSTANTS_H

#include "btn_common.h"

namespace btn::sram
{
    [[nodiscard]] constexpr int unused_bytes()
    {
        return sizeof(int);
    }

    [[nodiscard]] constexpr int max_size()
    {
        return (32 * 1024) - (unused_bytes() * 2);
    }
}

#endif
