/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_array.h"
#include "bn_fixed.h"
#include "bn_sin_lut.h"
#include "bn_reciprocal_lut.h"
#include "bn_rule_of_three_approximation.h"

/// @cond DO_NOT_DOCUMENT

namespace _bn
{
    [[nodiscard]] BN_CONST int sqrt_impl(int value);
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
     * @brief Returns the absolute value of the given value.
     *
     * @ingroup math
     */
    template<typename Type>
    [[nodiscard]] constexpr Type abs(Type value)
    requires(is_unsigned_v<Type>)
    {
        return value;
    }

    /**
     * @brief Returns the integer square root of the given value.
     *
     * @ingroup math
     */
    [[nodiscard]] BN_CONST constexpr int sqrt(int value)
    {
        BN_ASSERT(value >= 0, "Invalid value: ", value);

        if consteval
        {
            int x0 = value / 2;

            if(x0 == 0)
            {
                return value;
            }

            int x1 = (x0 + (value / x0)) / 2;

            while(x1 < x0)
            {
                x0 = x1;
                x1 = (x0 + (value / x0)) / 2;
            }

            return x0;
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
    [[nodiscard]] BN_CONST constexpr fixed_t<(Precision + 1) / 2> sqrt(fixed_t<Precision> value)
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
     * @brief Returns the fixed point square root of the given value
     * without trying to avoid overflow.
     *
     * See https://github.com/JoaoBaptMG/gba-modern/blob/master/source/math/fixedmath.hpp
     *
     * @ingroup math
     */
    template<int Precision>
    [[nodiscard]] BN_CONST constexpr fixed_t<Precision> unsafe_sqrt(fixed_t<Precision> value)
    {
        int double_precision_data = fixed_t<Precision * 2>(value).data();

        return fixed_t<Precision>::from_data(sqrt(double_precision_data));
    }

    /**
     * @brief Returns an angle in degrees in the range [0..360) from the given angle in degrees in any range.
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed safe_degrees_angle(fixed degrees_angle)
    {
        unsigned n = (degrees_angle / 360).unsigned_integer();
        return degrees_angle - (n * 360);
    }

    /**
     * @brief Calculates the sine value of an angle.
     * @param angle Angle (2π = 1).
     * @return Sine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed sin(fixed_t<16> angle)
    {
        int lut_angle = angle.data();
        return fixed::from_data(calculate_sin_lut_value(lut_angle));
    }

    /**
     * @brief Calculates the sine value of an angle in degrees.
     * @param degrees_angle Angle in degrees.
     * @return Sine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_sin(fixed degrees_angle)
    {
        constexpr rule_of_three_approximation rule_of_three(fixed(360).data(), 65536);
        int lut_angle = rule_of_three.calculate(degrees_angle.data());
        return fixed::from_data(calculate_sin_lut_value(lut_angle));
    }

    /**
     * @brief Calculates the sine value of an angle using a LUT.
     * @param lut_angle Angle in the range [0..2048].
     * @return Sine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed lut_sin(int lut_angle)
    {
        BN_ASSERT(lut_angle >= 0 && lut_angle < sin_lut_size,
                  "Angle must be in the range [0..", sin_lut_size - 1, "]: ", lut_angle);

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the sine value of an angle in degrees using a LUT.
     * @param degrees_angle Angle in degrees in the range [0..360].
     * @return Sine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_lut_sin(fixed degrees_angle)
    {
        BN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
                  "Angle must be in the range [0..360]: ", degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the sine value of an angle in degrees in any range using a LUT.
     * @param degrees_angle Angle in degrees in any range.
     * @return Sine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_lut_sin_safe(fixed degrees_angle)
    {
        degrees_angle = safe_degrees_angle(degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the cosine value of an angle.
     * @param angle Angle (2π = 1).
     * @return Cosine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed cos(fixed_t<16> angle)
    {
        int lut_angle = angle.data() + 16384;
        return fixed::from_data(calculate_sin_lut_value(lut_angle));
    }

    /**
     * @brief Calculates the cosine value of an angle in degrees.
     * @param degrees_angle Angle in degrees.
     * @return Cosine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_cos(fixed degrees_angle)
    {
        constexpr rule_of_three_approximation rule_of_three(fixed(360).data(), 65536);
        int lut_angle = rule_of_three.calculate(degrees_angle.data()) + 16384;
        return fixed::from_data(calculate_sin_lut_value(lut_angle));
    }

    /**
     * @brief Calculates the cosine value of an angle using a LUT.
     * @param lut_angle Angle in the range [0..2048].
     * @return Cosine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed lut_cos(int lut_angle)
    {
        BN_ASSERT(lut_angle >= 0 && lut_angle < sin_lut_size,
                  "Angle must be in the range [0..", sin_lut_size - 1, "]: ", lut_angle);

        lut_angle = (lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the cosine value of an angle in degrees using a LUT.
     * @param degrees_angle Angle in degrees in the range [0..360].
     * @return Cosine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_lut_cos(fixed degrees_angle)
    {
        BN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
                  "Angle must be in the range [0..360]: ", degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());
        lut_angle = (lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the cosine value of an angle in degrees in any range using a LUT.
     * @param degrees_angle Angle in degrees in any range.
     * @return Cosine value in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_lut_cos_safe(fixed degrees_angle)
    {
        degrees_angle = safe_degrees_angle(degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());
        lut_angle = (lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);

        if consteval
        {
            int sin_lut_value = calculate_sin_lut_value((lut_angle * 65536) / (sin_lut_size - 1));
            return fixed::from_data(sin_lut_value);
        }
        else
        {
            return fixed::from_data(sin_lut._data[lut_angle]);
        }
    }

    /**
     * @brief Calculates the sine and the cosine values of an angle.
     * @param angle Angle (2π = 1).
     * @return Sine and cosine values in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr pair<fixed, fixed> sin_and_cos(fixed_t<16> angle)
    {
        int sin_lut_angle = angle.data();
        int cos_lut_angle = sin_lut_angle + 16384;
        return { fixed::from_data(calculate_sin_lut_value(sin_lut_angle)),
                 fixed::from_data(calculate_sin_lut_value(cos_lut_angle)) };
    }

    /**
     * @brief Calculates the sine and the cosine values of an angle in degrees.
     * @param degrees_angle Angle in degrees.
     * @return Sine and cosine values in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr pair<fixed, fixed> degrees_sin_and_cos(fixed degrees_angle)
    {
        constexpr rule_of_three_approximation rule_of_three(fixed(360).data(), 65536);
        int sin_lut_angle = rule_of_three.calculate(degrees_angle.data());
        int cos_lut_angle = sin_lut_angle + 16384;
        return { fixed::from_data(calculate_sin_lut_value(sin_lut_angle)),
                 fixed::from_data(calculate_sin_lut_value(cos_lut_angle)) };
    }

    /**
     * @brief Calculates the sine and the cosine values value of an angle using a LUT.
     * @param lut_angle Angle in the range [0..2048].
     * @return Sine and cosine values in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr pair<fixed, fixed> lut_sin_and_cos(int lut_angle)
    {
        BN_ASSERT(lut_angle >= 0 && lut_angle < sin_lut_size,
                  "Angle must be in the range [0..", sin_lut_size - 1, "]: ", lut_angle);

        int sin_lut_angle = lut_angle;
        int cos_lut_angle = (sin_lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);
        int sin_lut_value;
        int cos_lut_value;

        if consteval
        {
            sin_lut_value = calculate_sin_lut_value((sin_lut_angle * 65536) / (sin_lut_size - 1));
            cos_lut_value = calculate_sin_lut_value((cos_lut_angle * 65536) / (sin_lut_size - 1));
        }
        else
        {
            sin_lut_value = sin_lut._data[sin_lut_angle];
            cos_lut_value = sin_lut._data[cos_lut_angle];
        }

        return { fixed::from_data(sin_lut_value), fixed::from_data(cos_lut_value) };
    }

    /**
     * @brief Calculates the sine and the cosine values of an angle in degrees using a LUT.
     * @param degrees_angle Angle in degrees in the range [0..360].
     * @return Sine and cosine values in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr pair<fixed, fixed> degrees_lut_sin_and_cos(fixed degrees_angle)
    {
        BN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
                  "Angle must be in the range [0..360]: ", degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int sin_lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());
        int cos_lut_angle = (sin_lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);
        int sin_lut_value;
        int cos_lut_value;

        if consteval
        {
            sin_lut_value = calculate_sin_lut_value((sin_lut_angle * 65536) / (sin_lut_size - 1));
            cos_lut_value = calculate_sin_lut_value((cos_lut_angle * 65536) / (sin_lut_size - 1));
        }
        else
        {
            sin_lut_value = sin_lut._data[sin_lut_angle];
            cos_lut_value = sin_lut._data[cos_lut_angle];
        }

        return { fixed::from_data(sin_lut_value), fixed::from_data(cos_lut_value) };
    }

    /**
     * @brief Calculates the sine and the cosine values of an angle in degrees in any range using a LUT.
     * @param degrees_angle Angle in degrees in any range.
     * @return Sine and cosine values in the range [-1..1].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr pair<fixed, fixed> degrees_lut_sin_and_cos_safe(fixed degrees_angle)
    {
        degrees_angle = safe_degrees_angle(degrees_angle);

        using rule_of_three_fixed = bn::fixed_t<10>;
        constexpr rule_of_three_approximation rule_of_three(rule_of_three_fixed(360).data(), sin_lut_size - 1);
        int sin_lut_angle = rule_of_three.unsafe_calculate(rule_of_three_fixed(degrees_angle).data());
        int cos_lut_angle = (sin_lut_angle + ((sin_lut_size - 1) / 4)) & (sin_lut_size - 2);
        int sin_lut_value;
        int cos_lut_value;

        if consteval
        {
            sin_lut_value = calculate_sin_lut_value((sin_lut_angle * 65536) / (sin_lut_size - 1));
            cos_lut_value = calculate_sin_lut_value((cos_lut_angle * 65536) / (sin_lut_size - 1));
        }
        else
        {
            sin_lut_value = sin_lut._data[sin_lut_angle];
            cos_lut_value = sin_lut._data[cos_lut_angle];
        }

        return { fixed::from_data(sin_lut_value), fixed::from_data(cos_lut_value) };
    }

    /**
     * @brief Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
     * @param y Vertical value in the range [-32767..32767].
     * @param x Horizontal value in the range [-32767..32767].
     * @return Arc tangent of y/x in the range [-0.5..0.5] (2π = 1).
     *
     * @ingroup math
     */
    [[nodiscard]] BN_CONST fixed_t<16> atan2(int y, int x);

    /**
     * @brief Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.
     * @param y Vertical value in the range [-32767..32767].
     * @param x Horizontal value in the range [-32767..32767].
     * @return Arc tangent of y/x in degrees in the range [-180..180].
     *
     * @ingroup math
     */
    [[nodiscard]] BN_CONST inline fixed degrees_atan2(int y, int x)
    {
        return fixed::from_data((atan2(y, x).data() * 360) / (1 << 4));
    }

    /**
     * @brief Computes an approximation of the arc tangent of y/x
     * using the signs of arguments to determine the correct quadrant.
     *
     * https://www.freesteel.co.uk/wpblog/2009/06/05/encoding-2d-angles-without-trigonometry/
     *
     * @param y Vertical value in the range [-131072..131071].
     * @param x Horizontal value in the range [-131072..131071].
     * @return Arc tangent of y/x in the range [-0.5..0.5] (2π = 1).
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed_t<16> diamond_angle(int y, int x)
    {
        BN_ASSERT(y >= -131072 && y <= 131071, "Invalid y: ", y);
        BN_ASSERT(x >= -131072 && x <= 131071, "Invalid x: ", x);

        int data;

        if(y >= 0)
        {
            if(x >= 0)
            {
                if(y == 0 && x == 0)
                {
                    return 0;
                }

                data = (y * 16384) / (x + y);
            }
            else
            {
                data = 16384 - ((x * 16384) / (y - x));
            }
        }
        else
        {
            if(x < 0)
            {
                data = -32768 - ((y * 16384) / (-x - y));
            }
            else
            {
                data = ((x * 16384) / (x - y)) - 16384;
            }
        }

        return fixed_t<16>::from_data(data);
    }

    /**
     * @brief Computes an approximation of the arc tangent of y/x
     * using the signs of arguments to determine the correct quadrant.
     *
     * https://www.freesteel.co.uk/wpblog/2009/06/05/encoding-2d-angles-without-trigonometry/
     *
     * @param y Vertical value in the range [-131072..131071].
     * @param x Horizontal value in the range [-131072..131071].
     * @return Arc tangent of y/x in degrees in the range [-180..180].
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed degrees_diamond_angle(int y, int x)
    {
        return fixed::from_data((diamond_angle(y, x).data() * 360) / (1 << 4));
    }

    /**
     * @brief Calculates the reciprocal of a value using a LUT.
     * @param lut_value Value in the range [1..1024].
     * @return Reciprocal of the given value (1 / value).
     *
     * @ingroup math
     */
    [[nodiscard]] constexpr fixed_t<20> lut_reciprocal(int lut_value)
    {
        BN_ASSERT(lut_value >= 1 && lut_value < reciprocal_lut_size,
                  "Value must be in the range [1..", reciprocal_lut_size - 1, "]: ", lut_value);

        if consteval
        {
            return calculate_reciprocal_lut_value(lut_value);
        }
        else
        {
            return reciprocal_lut._data[lut_value];
        }
    }
}

#endif
