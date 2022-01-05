/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MATH_H
#define BN_HW_MATH_H

#include "../3rd_party/gba-modern/include/gba-modern.h"
#include "bn_hw_tonc.h"

namespace bn::hw::math
{
    [[nodiscard]] inline int sqrt(int value)
    {
        return int(isqrt32(unsigned(value)));
    }

    [[nodiscard]] inline int atan2(int y, int x)
    {
        return int(uint16_t(ArcTan2(int16_t(x), int16_t(y))));
    }
}

#endif
