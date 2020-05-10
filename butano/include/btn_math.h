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
}

namespace btn
{
    template<typename Type>
    [[nodiscard]] constexpr Type abs(Type value)
    {
        return (value >= 0) ? value : -value;
    }

    template<int Precision = 12>
    [[nodiscard]] constexpr fixed_t<Precision> reciprocal_division(int numerator, int denominator)
    {
        static_assert(Precision > 0 && Precision < 31, "Invalid precision");
        BTN_CONSTEXPR_ASSERT(denominator, "Denominator is zero");

        return fixed_t<Precision>::from_data(numerator * fixed_t<Precision>(denominator).reciprocal().data());
    }

    /**
     * @brief Integer square root.
     */
    [[nodiscard]] int sqrt(int value);

    /**
     * @brief Fixed point square root.
     *
     * https://github.com/JoaoBaptMG/gba-modern/blob/master/source/math/fixedmath.hpp
     */
    template<int Precision = 12>
    [[nodiscard]] fixed_t<(Precision + 1) / 2> sqrt(fixed_t<Precision> value)
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
        BTN_CONSTEXPR_ASSERT(value >= 0, "Value is negative");

        return _btn::newton_raphson_sqrt_impl(value, value, Type(0));
    }

    /**
     * @brief Sine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Sine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed degrees_sin(fixed degrees_angle)
    {
        BTN_CONSTEXPR_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]");

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
        BTN_CONSTEXPR_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]");

        return fixed::from_data(sin_lut[lut_angle]);
    }

    /**
     * @brief Cosine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] constexpr fixed degrees_cos(fixed degrees_angle)
    {
        BTN_CONSTEXPR_ASSERT(degrees_angle >= 0 && degrees_angle <= 360, "Angle must be in the range [0, 360]");

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
        BTN_CONSTEXPR_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]");

        return fixed::from_data(sin_lut[(lut_angle + 128) & 0x1FF]);
    }
}

#endif
