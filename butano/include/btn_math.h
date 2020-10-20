/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MATH_H
#define BTN_MATH_H

#include "btn_fixed.h"
#include "btn_sin_lut.h"
#include "btn_rule_of_three_approximation.h"

namespace _btn
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
    [[nodiscard]] constexpr btn::fixed newton_raphson_sqrt_impl(btn::fixed value, btn::fixed current_result,
                                                                btn::fixed previous_result)
    {
        if(current_result == previous_result)
        {
            return current_result;
        }

        btn::fixed new_result = (current_result + (value.safe_division(current_result))) / 2;
        return newton_raphson_sqrt_impl(value, new_result, current_result);
    }

    [[nodiscard]] int sqrt_impl(int value);
}

namespace btn
{
    template<typename Type>
    [[nodiscard]] constexpr Type abs(Type value)
    {
        return (value >= 0) ? value : -value;
    }

    /**
     * @brief Integer square root.
     */
    [[nodiscard]] constexpr int sqrt(int value)
    {
        BTN_ASSERT(value >= 0, "Invalid value: ", value);

        if(is_constant_evaluated())
        {
            return _btn::newton_raphson_sqrt_impl(value, value, 0);
        }
        else
        {
            return _btn::sqrt_impl(value);
        }
    }

    /**
     * @brief Fixed point square root.
     *
     * https://github.com/JoaoBaptMG/gba-modern/blob/master/source/math/fixedmath.hpp
     */
    template<int Precision = 12>
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

    template<typename Type>
    [[nodiscard]] constexpr Type newton_raphson_sqrt(Type value)
    {
        BTN_ASSERT(value >= 0, "Invalid value: ", value);

        return _btn::newton_raphson_sqrt_impl(value, value, Type(0));
    }

    /**
     * @brief Sine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Sine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed degrees_sin(fixed degrees_angle)
    {
        BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]: ", degrees_angle);

        constexpr rule_of_three_approximation rule_of_three(360, 512);
        fixed lut_angle = rule_of_three.calculate(degrees_angle);
        return fixed::from_data(sin_lut[lut_angle.unsigned_integer()]);
    }

    /**
     * @brief Sine value of a s16 angle.
     * @param lut_angle Angle in the range [0, 512].
     * @return Sine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed lut_sin(int lut_angle)
    {
        BTN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

        return fixed::from_data(sin_lut[lut_angle]);
    }

    /**
     * @brief Cosine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed degrees_cos(fixed degrees_angle)
    {
        BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]: ", degrees_angle);

        constexpr rule_of_three_approximation rule_of_three(360, 512);
        fixed lut_angle = rule_of_three.calculate(degrees_angle);
        return fixed::from_data(sin_lut[(lut_angle.unsigned_integer() + 128) & 0x1FF]);
    }

    /**
     * @brief Cosine value of a s16 angle.
     * @param lut_angle Angle in the range [0, 512].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed lut_cos(int lut_angle)
    {
        BTN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

        return fixed::from_data(sin_lut[(lut_angle + 128) & 0x1FF]);
    }
}

#endif
