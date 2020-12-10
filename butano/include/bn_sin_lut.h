/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SIN_LUT_H
#define BN_SIN_LUT_H

/**
 * @file
 * bn::sin_lut header file.
 *
 * @ingroup math
 */

#include "bn_array.h"

namespace bn
{

/**
 * @brief Calculates the value to store in sin_lut for the given angle.
 * @param lut_angle sin_lut angle (360 degrees = 65536).
 * @return Sine value in the range [-4096, 4096].
 *
 * @ingroup math
 */
[[nodiscard]] constexpr int16_t calculate_sin_lut_value(int lut_angle)
{
    // https://www.nullhardware.com/blog/fixed-point-sine-and-cosine-for-embedded-systems/

    auto i = int16_t(lut_angle);
    bool c = i < 0;

    if(i == (i | 0x4000))
    {
        i = (1 << 15) - i;
    }

    i = (i & 0x7FFF) >> 1;

    enum { A1 = 3370945099UL, B1 = 2746362156UL, C1 = 292421UL };
    enum { n = 13, p = 32, q = 31, r = 3, a = 12 };

    unsigned ui(i);
    unsigned y = (C1 * ui) >> n;
    y = B1 - ((ui * y) >> r);
    y = ui * (y >> n);
    y = ui * (y >> n);
    y = A1 - (y >> (p - q));
    y = ui * (y >> n);
    y = (y + (1UL << (q - a - 1))) >> (q - a);

    return c ? -y : y;
}

/**
 * @brief A sine LUT of 16bit values in 4.12 format.
 *
 * @ingroup math
 */
constexpr const array<int16_t, 2049> sin_lut = []{
    array<int16_t, 2049> result;
    int lut_angle = 0;

    for(int index = 0; index < 2049; ++index)
    {
        result[index] = calculate_sin_lut_value(lut_angle);
        lut_angle += 65536 / 2048;
    }

    return result;
}();

}

#endif
