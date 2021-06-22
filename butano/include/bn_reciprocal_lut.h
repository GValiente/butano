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

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
{

/**
 * @brief Calculates the value to store in reciprocal_lut for the given value.
 * @param lut_value reciprocal_lut value (>= 1).
 * @return Reciprocal of the given value (1 / value).
 *
 * @ingroup math
 */
[[nodiscard]] constexpr fixed_t<20> calculate_reciprocal_lut_value(int lut_value)
{
    BN_ASSERT(lut_value > 0, "Value must be greater than 0: ", lut_value);

    return fixed_t<20>::from_data(fixed_t<20>(1).data() / lut_value);
}

/**
 * @brief Reciprocal LUT size.
 *
 * @ingroup math
 */
constexpr int reciprocal_lut_size = 1025;

/**
 * @brief Pointer to the reciprocal LUT.
 *
 * @ingroup math
 */
extern const fixed_t<20>* reciprocal_lut;

}

#endif
