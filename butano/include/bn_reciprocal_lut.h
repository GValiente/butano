/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECIPROCAL_LUT_H
#define BN_RECIPROCAL_LUT_H

/**
 * @file
 * bn::reciprocal_lut header file.
 *
 * @ingroup math
 */

#include "bn_array.h"
#include "bn_fixed.h"

namespace bn
{

/**
 * @brief Returns the LUT used to calculate the reciprocal of the given value (1 / value).
 *
 * @ingroup math
 */
[[nodiscard]] constexpr array<fixed, 1025> create_reciprocal_lut()
{
    array<fixed, 1025> result;
    int one = fixed(1).data() * fixed::scale();

    for(int index = 1; index < 1025; ++index)
    {
        result[index] = fixed::from_data(one / index);
    }

    return result;
}

/**
 * @brief LUT used to invert scale values in an affine transformation matrix.
 *
 * @ingroup math
 */
constexpr const array<fixed, 1025> reciprocal_lut = create_reciprocal_lut();

}

#endif
