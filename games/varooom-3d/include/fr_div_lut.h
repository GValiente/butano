/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_DIV_LUT_H
#define FR_DIV_LUT_H

#include "bn_fixed.h"

namespace fr
{

constexpr int div_lut_precision = 24;
extern const uint32_t* div_lut_ptr;

[[nodiscard]] constexpr uint32_t calculate_div_lut_value(int denominator)
{
    if(denominator < 2)
    {
        return 1 << div_lut_precision;
    }

    return uint32_t((1 << div_lut_precision) / denominator);
}

template<int Precision>
[[nodiscard]] constexpr bn::fixed_t<Precision> unsafe_unsigned_lut_division(int numerator, int denominator)
{
    static_assert(Precision > 0 && Precision <= div_lut_precision, "Invalid precision");

    uint32_t div_lut_value = 0;

    if consteval
    {
        div_lut_value = calculate_div_lut_value(denominator);
    }
    else
    {
        div_lut_value = div_lut_ptr[denominator];
    }

    return bn::fixed_t<Precision>::from_data(numerator * int(div_lut_value >> (div_lut_precision - Precision)));
}

}

#endif
