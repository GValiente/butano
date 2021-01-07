/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MATH_H
#define BN_HW_MATH_H

#include "../3rd_party/gba-modern/include/gba-modern.h"
#include "bn_common.h"

namespace bn::hw
{
    [[nodiscard]] inline int sqrt(int value)
    {
        return int(isqrt32(unsigned(value)));
    }
}

#endif
