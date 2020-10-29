/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_SRAM_CONSTANTS_H
#define BTN_HW_SRAM_CONSTANTS_H

#include "btn_common.h"

namespace btn::hw::sram
{
    [[nodiscard]] constexpr int size()
    {
        return 32 * 1024;
    }
}

#endif
