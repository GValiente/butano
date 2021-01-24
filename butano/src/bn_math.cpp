/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_math.h"

#include "../hw/include/bn_hw_math.h"

#include "bn_log.h"

namespace _bn
{

int sqrt_impl(int value)
{
    return bn::hw::math::sqrt(value);
}

}

namespace bn
{

fixed_t<16> atan2(fixed y, fixed x)
{
    int result = hw::math::atan2(y.data(), x.data());

    if(result > 32768)
    {
        result -= 65536;
    }

    return fixed_t<16>::from_data(result);
}

}
