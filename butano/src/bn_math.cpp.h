/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_math.h"

#include "../hw/include/bn_hw_math.h"

namespace _bn
{

int sqrt_impl(int value)
{
    return bn::hw::math::sqrt(value);
}

}

namespace bn
{

fixed_t<16> atan2(int y, int x)
{
    BN_ASSERT(y >= -32767 && y <= 32767, "Invalid y: ", y);
    BN_ASSERT(x >= -32767 && x <= 32767, "Invalid x: ", x);

    int result = hw::math::atan2(y, x);

    if(result > 32768)
    {
        result -= 65536;
    }

    return fixed_t<16>::from_data(result);
}

}
