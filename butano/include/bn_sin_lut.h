/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * Copyright (c) 2011-2015, 2019-2020 Antonio Niño Díaz <antonio_nd@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
[[nodiscard]] constexpr int calculate_sin_lut_value(int lut_angle)
{
    // https://github.com/AntonioND/ugba/blob/master/libugba/source/fp_math.c

    constexpr const int FP_PI = 0x8000;
    constexpr const int FP_PI_2 = FP_PI / 2;
    constexpr const int FP_2_PI = 2 * FP_PI;

    // First, use symmetry to clamp to -pi/2 to +pi/2 (-0x4000 to 0x4000)

    int x = lut_angle;
    x &= FP_2_PI - 1;

    if ((x > FP_PI_2) && (x < (3 * FP_PI_2))) // pi/2 to 3*pi/2
        x = FP_PI - x;
    else if (x >= (3 * FP_PI_2)) // 3*pi/2 to 2*pi
        x = x - FP_2_PI;

    // Calculate result

    constexpr const double pi = 3.1415926535897932384626433832795;

    constexpr const auto A = int64_t(((2.0 * pi) * (1 << 24))); // 8.24
    constexpr const auto B = int64_t(((12.0 / pi - 1.0 - pi) * (1 << 24))); // 0.24
    constexpr const auto C = int64_t((3.0 * (2.0 + pi - 16.0 / pi) * (1 << 24))); // 0.24

    int64_t X2 = x; // 16.0
    X2 = X2 * X2; // 32.0

    int64_t T1 = X2 * C; // 32.24
    T1 >>= 30; // 2.24

    int64_t T2 = B + T1; // 2.24
    T2 *= X2; // 34.24
    T2 >>= 25; // 9.24

    int64_t result = A + T2; // 9.24
    result *= x; // 25.24
    // Add 0.5 to round up before the final shift
    result += (1 << 24) / 2; // 25.24
    result >>= 24; // 25.0

    return int(result >> 4);
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
        result[index] = int16_t(calculate_sin_lut_value(lut_angle));
        lut_angle += 65536 / 2048;
    }

    return result;
}();

}

#endif
