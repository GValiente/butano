/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_MATH_H
#define BTN_HW_MATH_H

#include "../3rd_party/gba-modern/include/gba-modern.h"
#include "btn_common.h"

namespace btn::hw
{
    [[nodiscard]] inline int sqrt(int value)
    {
        return int(isqrt32(unsigned(value)));
    }
}

#endif
