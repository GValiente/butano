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

    class rule_of_three_aproximation
    {

    public:
        constexpr rule_of_three_aproximation(unsigned multiplicator, unsigned divisor) :
            _divisor(_build_divisor(divisor)),
            _multiplicator(_build_multiplicator(multiplicator, divisor))
        {
        }

        [[nodiscard]] constexpr unsigned unsafe_calculate(unsigned value) const
        {
            return (value * _multiplicator) / _divisor;
        }

        [[nodiscard]] constexpr int unsafe_calculate(int value) const
        {
            return (value * int(_multiplicator)) / int(_divisor);
        }

        template<size_t Precision>
        [[nodiscard]] constexpr fixed_t<Precision> unsafe_calculate(fixed_t<Precision> fixed) const
        {
            return fixed_t<Precision>::create(unsafe_calculate(fixed.value()));
        }

        [[nodiscard]] constexpr unsigned calculate(unsigned value) const
        {
            return unsigned((uint64_t(value) * _multiplicator) / _divisor);
        }

        [[nodiscard]] constexpr int calculate(int value) const
        {
            return int((int64_t(value) * _multiplicator) / _divisor);
        }

        template<size_t Precision>
        [[nodiscard]] constexpr fixed_t<Precision> calculate(fixed_t<Precision> fixed) const
        {
            return fixed_t<Precision>::create(calculate(fixed.value()));
        }

    private:
        unsigned _divisor;
        unsigned _multiplicator;

        [[nodiscard]] static constexpr unsigned _build_divisor(unsigned divisor)
        {
            BTN_CONSTEXPR_ASSERT(divisor, "Divisor is zero");

            unsigned shift_bits = 0;

            while(true)
            {
                unsigned result = 1u << shift_bits;

                if(result >= divisor)
                {
                    return result;
                }

                ++shift_bits;
            }
        }

        [[nodiscard]] constexpr unsigned _build_multiplicator(unsigned multiplicator, unsigned divisor)
        {
            BTN_CONSTEXPR_ASSERT(divisor, "Divisor is zero");

            if(_divisor == divisor)
            {
                return multiplicator;
            }

            return ((multiplicator * _divisor) / divisor) + 1;
        }
    };

    template<size_t Precision = 12>
    [[nodiscard]] constexpr fixed_t<Precision> reciprocal_division(int numerator, int denominator)
    {
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

        return _btn::newton_raphson_sqrt_impl(value, value, 0);
    }

    /**
     * @brief Sine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Sine value in the range [-1, 1].
     */
    [[nodiscard]] fixed degrees_sin(fixed degrees_angle);

    /**
     * @brief Sine value of a s16 angle.
     * @param angle Angle in the range [0, 65536].
     * @return Sine value in the range [-4096, 4096].
     */
    [[nodiscard]] int lut_sin(int s16_angle);

    /**
     * @brief Cosine value of an angle in degrees.
     * @param degrees_angle Angle in the range [0, 360].
     * @return Cosine value in the range [-1, 1].
     */
    [[nodiscard]] fixed degrees_cos(fixed degrees_angle);

    /**
     * @brief Cosine value of a s16 angle.
     * @param angle Angle in the range [0, 65536].
     * @return Cosine value in the range [-4096, 4096].
     */
    [[nodiscard]] int lut_cos(int s16_angle);
}

#endif
