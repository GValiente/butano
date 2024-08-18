/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MATH_H
#define BN_HW_MATH_H

#include "bn_common.h"
#include "../3rd_party/agbabi/include/agbabi.h"
#include "../3rd_party/gba-modern/include/gba-modern.h"

namespace bn::hw::math
{
    [[nodiscard]] inline int sqrt(int value)
    {
        return int(isqrt32(unsigned(value)));
    }

    [[nodiscard]] inline int atan2(int y, int x)
    {
        return int(__agbabi_atan2(x, y));
    }
}

#endif
