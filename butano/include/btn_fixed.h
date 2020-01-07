#ifndef BTN_FIXED_H
#define BTN_FIXED_H

#include "btn_assert.h"
#include "btn_limits.h"
#include "btn_functional.h"

namespace btn
{

template<size_t Precision>
class fixed_t
{
    static_assert(Precision > 0 && Precision < 31, "Invalid precision");

public:
    [[nodiscard]] static constexpr fixed_t create(int value)
    {
        return fixed_t(value, false);
    }

    [[nodiscard]] static constexpr int precision()
    {
        return Precision;
    }

    [[nodiscard]] static constexpr int scale()
    {
        return 1 << Precision;
    }

    constexpr fixed_t() = default;

    constexpr fixed_t(int integer) :
        _value(integer * scale())
    {
    }

    constexpr fixed_t(int integer, int fraction) :
        _value((integer * scale()) + fraction)
    {
        BTN_CONSTEXPR_ASSERT(fraction >= 0, "Fraction is negative");
    }

    constexpr fixed_t(float value) :
        _value(int(value * scale()))
    {
    }

    constexpr fixed_t(double value) :
        _value(int(value * scale()))
    {
    }

    template<size_t OtherPrecision>
    constexpr fixed_t(fixed_t<OtherPrecision> other) :
        _value(Precision < OtherPrecision ?
                   other.value() >> (OtherPrecision - Precision) :
                   other.value() << (Precision - OtherPrecision))
    {
    }

    [[nodiscard]] constexpr int value() const
    {
        return _value;
    }

    [[nodiscard]] constexpr int integer() const
    {
        return _value / scale();
    }

    [[nodiscard]] constexpr int fraction() const
    {
        return _value & (scale() - 1);
    }

    [[nodiscard]] constexpr float to_float() const
    {
        return float(_value) / scale();
    }

    [[nodiscard]] constexpr double to_double() const
    {
        return double(_value) / scale();
    }

    [[nodiscard]] constexpr fixed_t multiplication(fixed_t other) const
    {
        return fixed_t::create(int((int64_t(_value) * other._value) / scale()));
    }

    [[nodiscard]] constexpr fixed_t multiplication(int integer) const
    {
        return fixed_t::create(_value * integer);
    }

    [[nodiscard]] constexpr fixed_t unsafe_multiplication(fixed_t other) const
    {
        return fixed_t::create((_value * other._value) / scale());
    }

    [[nodiscard]] constexpr fixed_t unsafe_multiplication(int integer) const
    {
        return multiplication(integer);
    }

    [[nodiscard]] constexpr fixed_t reciprocal() const
    {
        BTN_CONSTEXPR_ASSERT(integer(), "Integer is zero");

        return fixed_t::create((scale() + integer() - 1) / integer());
    }

    [[nodiscard]] constexpr fixed_t reciprocal_division(fixed_t other) const
    {
        BTN_CONSTEXPR_ASSERT(reciprocal_division_threshold(other) <= 0 ||
                             integer() < scale() / reciprocal_division_threshold(other), "Numerator is too high");
        BTN_CONSTEXPR_ASSERT(scale() > integer() * (other.integer() - 1), "Precision is too low");

        return *this * other.reciprocal();
    }

    [[nodiscard]] constexpr fixed_t division(fixed_t other) const
    {
        BTN_CONSTEXPR_ASSERT(other._value, "Other's internal value is zero");

        return fixed_t::create(int((int64_t(_value) * scale()) / other._value));
    }

    [[nodiscard]] constexpr fixed_t division(int integer) const
    {
        BTN_CONSTEXPR_ASSERT(integer, "Integer is zero");

        return fixed_t::create(_value / integer);
    }

    [[nodiscard]] constexpr fixed_t unsafe_division(fixed_t other) const
    {
        BTN_CONSTEXPR_ASSERT(other._value, "Other's internal value is zero");

        return fixed_t::create((_value * scale()) / other._value);
    }

    [[nodiscard]] constexpr fixed_t unsafe_division(int integer) const
    {
        BTN_CONSTEXPR_ASSERT(integer, "Integer is zero");

        return division(integer);
    }

    /**
     * @brief Taylor series square root aproximation. It gives usable results for values between 0.5 and 2.
     *
     * Thanks to ax6.
     * Source: https://pastebin.com/raw/PsDBJXDV
     *
     * @param iterations Aproximation iterations [1, ...)
     * @return Aproximated square root.
     */
    [[nodiscard]] constexpr fixed_t ts_sqrt(int iterations) const
    {
        BTN_CONSTEXPR_ASSERT(iterations >= 1, "Invalid iterations");

        int result = (_value + (1 << Precision)) >> 1;
        int factor = ((1 << Precision) - _value) >> 1;
        int power = factor;
        int coefficient = 1 << Precision;

        auto multiply = [](int first, int second)
        {
            int64_t result = (int64_t(first) * int64_t(second) + (int64_t(1) << (Precision - 1))) >> Precision;

            if(result > integral_limits<int>::max)
            {
                return integral_limits<int>::max;
            }

            if(result < integral_limits<int>::min)
            {
                return integral_limits<int>::min;
            }

            return int(result);
        };

        auto integer_inverse = [](unsigned value)
        {
            return ((2 << Precision) / value + 1) >> 1;
        };

        for(unsigned iteration = 2; iteration <= unsigned(iterations); ++iteration)
        {
            coefficient = multiply(coefficient, (2 * iteration - 3) << Precision);
            coefficient = multiply(coefficient, integer_inverse(iteration));
            power = multiply(power, factor);
            result -= multiply(power, coefficient);
        }

        return create(result);
    }

    constexpr fixed_t& operator+=(fixed_t other)
    {
        _value += other._value;
        return *this;
    }

    constexpr fixed_t& operator-=(fixed_t other)
    {
        _value -= other._value;
        return *this;
    }

    constexpr fixed_t& operator*=(fixed_t other)
    {
        *this = multiplication(other);
        return *this;
    }

    constexpr fixed_t& operator*=(int integer)
    {
        *this = multiplication(integer);
        return *this;
    }

    constexpr fixed_t& operator/=(fixed_t other)
    {
        BTN_CONSTEXPR_ASSERT(other._value, "Other's internal value is zero");

        *this = division(other);
        return *this;
    }

    constexpr fixed_t& operator/=(int integer)
    {
        BTN_CONSTEXPR_ASSERT(integer, "Integer is zero");

        *this = division(integer);
        return *this;
    }

    [[nodiscard]] constexpr fixed_t operator+(fixed_t other) const
    {
        return fixed_t::create(_value + other._value);
    }

    [[nodiscard]] constexpr fixed_t operator-(fixed_t other) const
    {
        return fixed_t::create(_value - other._value);
    }

    [[nodiscard]] constexpr fixed_t operator-() const
    {
        return fixed_t() - *this;
    }

    [[nodiscard]] constexpr fixed_t operator*(fixed_t other) const
    {
        return multiplication(other);
    }

    [[nodiscard]] constexpr fixed_t operator*(int integer) const
    {
        return multiplication(integer);
    }

    [[nodiscard]] constexpr fixed_t operator/(fixed_t other) const
    {
        BTN_CONSTEXPR_ASSERT(other._value, "Other's internal value is zero");

        return division(other);
    }

    [[nodiscard]] constexpr fixed_t operator/(int integer) const
    {
        BTN_CONSTEXPR_ASSERT(integer, "Integer is zero");

        return division(integer);
    }

    [[nodiscard]] constexpr bool operator==(fixed_t other) const
    {
        return _value == other._value;
    }

    [[nodiscard]] constexpr bool operator!=(fixed_t other) const
    {
        return _value != other._value;
    }

    [[nodiscard]] constexpr bool operator<(fixed_t other) const
    {
        return _value < other._value;
    }

    [[nodiscard]] constexpr bool operator<=(fixed_t other) const
    {
        return _value <= other._value;
    }

    [[nodiscard]] constexpr bool operator>(fixed_t other) const
    {
        return _value > other._value;
    }

    [[nodiscard]] constexpr bool operator>=(fixed_t other) const
    {
        return _value >= other._value;
    }

private:
    int _value = 0;

    constexpr fixed_t(int value, bool) :
        _value(value)
    {
    }

    [[nodiscard]] constexpr int reciprocal_division_threshold(fixed_t other) const
    {
        return other.reciprocal()._value * other.integer() - scale();
    }
};

using fixed = fixed_t<12>;

template<size_t Precision>
struct hash<fixed_t<Precision>>
{
    [[nodiscard]] constexpr size_t operator()(fixed_t<Precision> fixed) const
    {
        return make_hash(fixed.value());
    }
};

}

#endif
