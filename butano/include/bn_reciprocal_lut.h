/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * @brief LUT used to calculate the reciprocal of the given value (1 / value).
 *
 * @ingroup math
 */
constexpr const array<fixed_t<20>, 1025> reciprocal_lut = []{
    array<fixed_t<20>, 1025> result;
    int one = fixed_t<20>(1).data();

    for(int index = 1; index < 1025; ++index)
    {
        result[index] = fixed_t<20>::from_data(one / index);
    }

    return result;
}();

}

#endif
