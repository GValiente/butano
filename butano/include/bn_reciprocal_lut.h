/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECIPROCAL_LUT_H
#define BN_RECIPROCAL_LUT_H

/**
 * @file
 * bn::reciprocal_lut and bn::reciprocal_16_lut header file.
 *
 * @ingroup math
 */

#include "bn_fixed.h"
#include "bn_assert.h"
#include "bn_array_fwd.h"

namespace bn
{

/**
 * @brief Calculates the value to store in a reciprocal LUT for the given value.
 * @tparam Precision Number of bits used for the fractional part of the reciprocal value.
 * @param lut_value Value in the range (>= 1).
 * @return Reciprocal of the given value (1 / value).
 *
 * @ingroup math
 */
template<int Precision = 20>
[[nodiscard]] constexpr fixed_t<Precision> calculate_reciprocal_lut_value(int lut_value)
{
    BN_ASSERT(lut_value > 0, "Invalid lut value: ", lut_value);

    return fixed_t<Precision>::from_data(fixed_t<Precision>(1).data() / lut_value);
}

/**
 * @brief Reciprocal 20bit LUT size.
 *
 * @ingroup math
 */
constexpr int reciprocal_lut_size = 1025;

/**
 * @brief Reciprocal 20bit LUT.
 *
 * @ingroup math
 */
extern const array<fixed_t<20>, reciprocal_lut_size>& reciprocal_lut;

/**
 * @brief Reciprocal 16bit LUT size.
 *
 * @ingroup math
 */
constexpr int reciprocal_16_lut_size = 1025;

/**
 * @brief Reciprocal 16bit LUT.
 *
 * @ingroup math
 */
alignas(int) extern const array<uint16_t, reciprocal_16_lut_size>& reciprocal_16_lut;

}

#endif
