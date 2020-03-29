#ifndef BTN_MATH_H
#define BTN_MATH_H

#include "btn_assert.h"
#include "btn_fixed_fwd.h"

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

        return fixed_t<Precision>::create(numerator * fixed_t<Precision>(denominator).reciprocal().value());
    }

    /**
     * @brief Integer square root.
     */
    [[nodiscard]] int sqrt(int value);

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
    [[nodiscard]] fixed degrees_sin(fixed degrees_angle);

    /**
     * @brief Sine value of a s16 angle.
     * @param lut_angle Angle in the range [0, 512].
     * @return Sine value in the range [-1, 1].
     */
    [[nodiscard]] fixed lut_sin(int lut_angle);

    /**
     * @brief Cosine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] fixed degrees_cos(fixed degrees_angle);

    /**
     * @brief Cosine value of a s16 angle.
     * @param lut_angle Angle in the range [0, 512].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] fixed lut_cos(int lut_angle);
}

#endif
