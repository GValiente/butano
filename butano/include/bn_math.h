/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MATH_H
#define BN_MATH_H

/**
 * @file
 * Math functions header file.
 *
 * @ingroup math
 */

#include "bn_fixed.h"
#include "bn_sin_lut.h"
#include "bn_reciprocal_lut.h"
#include "bn_rule_of_three_approximation.h"

/// @cond DO_NOT_DOCUMENT

namespace _bn
{
    template<typename Type>
    [[nodiscard]] constexpr Type newton_raphson_sqrt_impl(Type value, Type current_result, Type previous_result)
    {
        if(current_result == previous_result)
        {
            return current_result;
        }

        Type new_result = (current_result + (value / current_result)) / 2;
        return newton_raphson_sqrt_impl(value, new_result, current_result);
    }

    template<>
    [[nodiscard]] constexpr bn::fixed newton_raphson_sqrt_impl(bn::fixed value, bn::fixed current_result,
                                                                bn::fixed previous_result)
    {
        if(current_result == previous_result)
        {
            return current_result;
        }

        bn::fixed new_result = (current_result + (value.safe_division(current_result))) / 2;
        return newton_raphson_sqrt_impl(value, new_result, current_result);
    }

    [[nodiscard]] int sqrt_impl(int value);
}

/// @endcond


namespace bn
{
    /**
     * @brief Returns the absolute value of the given value.
     *
     * @ingroup math
     */
    template<typename Type>
    [[nodiscard]] constexpr Type abs(Type value)
    {
        return (value >= 0) ? value : -value;
    }

    /**
     * @brief Returns the integer square root of the given value.
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr int sqrt(int value)
    {
        BN_ASSERT(value >= 0, "Invalid value: ", value);

        if(is_constant_evaluated())
        {
            return _bn::newton_raphson_sqrt_impl(value, value, 0);
        }
        else
        {
            return _bn::sqrt_impl(value);
        }
    }

    /**
     * @brief Returns the fixed point square root of the given value.
     *
     * See https://github.com/JoaoBaptMG/gba-modern/blob/master/source/math/fixedmath.hpp
     *
     * @ingroup math
     */
    template<int Precision>
    [[nodiscard]] constexpr fixed_t<(Precision + 1) / 2> sqrt(fixed_t<Precision> value)
    {
        if constexpr(Precision % 2)
        {
            return fixed_t<(Precision + 1) / 2>::from_data(sqrt(value.data() << 1));
        }
        else
        {
            return fixed_t<(Precision + 1) / 2>::from_data(sqrt(value.data()));
        }
    }

    /**
     * @brief Returns the square root of the given value using the Newton–Raphson method.
     *
     * See https://en.wikipedia.org/wiki/Newton%27s_method#Square_root
     *
     * @ingroup math
     */
    template<typename Type>
    [[nodiscard]] constexpr Type newton_raphson_sqrt(Type value)
    {
        BN_ASSERT(value >= 0, "Invalid value: ", value);

        return _bn::newton_raphson_sqrt_impl(value, value, Type(0));
    }

    /**
     * @brief Calculates the sine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Sine value in the range [-1, 1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_sin(fixed degrees_angle)
    {
        BN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]: ", degrees_angle);

        constexpr rule_of_three_approximation rule_of_three(360, 512);
        fixed lut_angle = rule_of_three.calculate(degrees_angle);
        return fixed::from_data(sin_lut[lut_angle.unsigned_integer()]);
    }

    /**
     * @brief Calculates the sine value of an angle using a LUT.
     * @param lut_angle Angle in the range [0, 512].
     * @return Sine value in the range [-1, 1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed lut_sin(int lut_angle)
    {
        BN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

        return fixed::from_data(sin_lut[lut_angle]);
    }

    /**
     * @brief Calculates the cosine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Cosine value in the range [-1, 1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_cos(fixed degrees_angle)
    {
        BN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]: ", degrees_angle);

        constexpr rule_of_three_approximation rule_of_three(360, 512);
        fixed lut_angle = rule_of_three.calculate(degrees_angle);
        return fixed::from_data(sin_lut[(lut_angle.unsigned_integer() + 128) & 0x1FF]);
    }

    /**
     * @brief Calculates the cosine value of an angle using a LUT.
     * @param lut_angle Angle in the range [0, 512].
     * @return Cosine value in the range [-1, 1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed lut_cos(int lut_angle)
    {
        BN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

        return fixed::from_data(sin_lut[(lut_angle + 128) & 0x1FF]);
    }

    /**
     * @brief Calculates the reciprocal of a value using a LUT.
     * @param lut_value Value in the range [1, 1024].
     * @return Reciprocal of the given value (1 / value).
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed lut_reciprocal(int lut_value)
    {
        BN_ASSERT(lut_value >= 1 && lut_value <= 1024, "Value must be in the range [1, 1024]: ", lut_value);

        return reciprocal_lut[lut_value];
    }
}

#endif
