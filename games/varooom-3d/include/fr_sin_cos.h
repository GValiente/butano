/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SIN_COS_H
#define FR_SIN_COS_H

#include "bn_fixed.h"
#include "bn_sin_lut.h"

namespace fr
{

extern const int16_t* sin_lut_ptr;

[[nodiscard]] constexpr bn::fixed sin(int angle)
{
    if consteval
    {
        return bn::fixed::from_data(bn::calculate_sin_lut_value(angle));
    }
    else
    {
        angle = (angle % 65536);

        return bn::fixed::from_data(sin_lut_ptr[angle]);
    }
}

[[nodiscard]] constexpr bn::fixed cos(int angle)
{
    return sin(angle + 16384);
}

}

#endif
