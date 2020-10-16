#ifndef BTN_FIXED_H
#define BTN_FIXED_H

#include "btn_assert.h"
#include "btn_limits.h"
#include "btn_compare.h"
#include "btn_functional.h"

namespace btn
{

template<int Precision>
class fixed_t
{
    static_assert(Precision > 0 && Precision < 31, "Invalid precision");

public:
    [[nodiscard]] static constexpr fixed_t from_data(int data)
    {
        fixed_t result;
        result._data = data;
        return result;
    }

    [[nodiscard]] static constexpr int precision()
    {
        return Precision;
    }

    [[nodiscard]] static constexpr int scale()
    {
        return 1 << Precision;
    }

    [[nodiscard]] static constexpr int half_scale()
    {
        static_assert(Precision % 2 == 0, "Invalid precision");

        return 1 << (Precision / 2);
    }

    constexpr fixed_t() = default;

    constexpr fixed_t(int integer) :
        _data(integer * scale())
    {
    }

    constexpr fixed_t(int integer, int fraction) :
        _data((integer * scale()) + fraction)
    {
        BTN_ASSERT(fraction >= 0, "Fraction is negative");
    }

    constexpr fixed_t(float value) :
        _data(int(value * scale()))
    {
    }

    constexpr fixed_t(double value) :
        _data(int(value * scale()))
    {
    }

    template<int OtherPrecision>
    constexpr fixed_t(fixed_t<OtherPrecision> other) :
        _data(Precision < OtherPrecision ?
                   other.data() / (other.scale() / scale()) :
                   other.data() * (scale() / other.scale()))
    {
    }

    [[nodiscard]] constexpr int data() const
    {
        return _data;
    }

    [[nodiscard]] constexpr int integer() const
    {
        return _data / scale();
    }

    [[nodiscard]] constexpr int right_shift_integer() const
    {
        return _data >> Precision;
    }

    [[nodiscard]] constexpr unsigned unsigned_integer() const
    {
        return _data >> Precision;
    }

    [[nodiscard]] constexpr int round_integer() const
    {
        int data = _data;

        if(data >= 0)
        {
            return (data + (1 << (Precision - 1))) / scale();
        }
        else
        {
            return (data - (1 << (Precision - 1))) / scale();
        }
    }

    [[nodiscard]] constexpr int fraction() const
    {
        return _data & (scale() - 1);
    }

    [[nodiscard]] constexpr float to_float() const
    {
        return float(_data) / scale();
    }

    [[nodiscard]] constexpr double to_double() const
    {
        return double(_data) / scale();
    }

    [[nodiscard]] constexpr fixed_t multiplication(fixed_t other) const
    {
        if constexpr(Precision % 2 == 0)
        {
            int data = _data / half_scale();
            int other_data = other._data / half_scale();
            return from_data(data * other_data);
        }

        return safe_multiplication(other);
    }

    [[nodiscard]] constexpr fixed_t multiplication(int integer) const
    {
        return from_data(_data * integer);
    }

    [[nodiscard]] constexpr fixed_t safe_multiplication(fixed_t other) const
    {
        return from_data(int((int64_t(_data) * other._data) / scale()));
    }

    [[nodiscard]] constexpr fixed_t unsafe_multiplication(fixed_t other) const
    {
        return from_data((_data * other._data) / scale());
    }

    [[nodiscard]] constexpr fixed_t unsafe_multiplication(int integer) const
    {
        return multiplication(integer);
    }

    [[nodiscard]] constexpr fixed_t division(fixed_t other) const
    {
        if constexpr(Precision % 2 == 0)
        {
            int data = _data * half_scale();
            int other_data = other._data / half_scale();
            BTN_ASSERT(other_data, "Other's internal data is zero");

            return from_data(data / other_data);
        }

        return safe_division(other);
    }

    [[nodiscard]] constexpr fixed_t division(int integer) const
    {
        BTN_ASSERT(integer, "Integer is zero");

        return from_data(_data / integer);
    }

    [[nodiscard]] constexpr fixed_t safe_division(fixed_t other) const
    {
        BTN_ASSERT(other._data, "Other's internal data is zero");

        return from_data(int((int64_t(_data) * scale()) / other._data));
    }

    [[nodiscard]] constexpr fixed_t unsafe_division(fixed_t other) const
    {
        BTN_ASSERT(other._data, "Other's internal data is zero");

        return from_data((_data * scale()) / other._data);
    }

    [[nodiscard]] constexpr fixed_t unsafe_division(int integer) const
    {
        BTN_ASSERT(integer, "Integer is zero");

        return division(integer);
    }

    [[nodiscard]] constexpr fixed_t operator-() const
    {
        return from_data(-_data);
    }

    constexpr fixed_t& operator+=(fixed_t other)
    {
        _data += other._data;
        return *this;
    }

    constexpr fixed_t& operator-=(fixed_t other)
    {
        _data -= other._data;
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
        *this = division(other);
        return *this;
    }

    constexpr fixed_t& operator/=(int integer)
    {
        *this = division(integer);
        return *this;
    }

    [[nodiscard]] constexpr friend fixed_t operator+(fixed_t a, fixed_t b)
    {
        return from_data(a._data + b._data);
    }

    [[nodiscard]] constexpr friend fixed_t operator-(fixed_t a, fixed_t b)
    {
        return from_data(a._data - b._data);
    }

    [[nodiscard]] constexpr friend fixed_t operator*(fixed_t a, fixed_t b)
    {
        return a.multiplication(b);
    }

    [[nodiscard]] constexpr friend fixed_t operator*(fixed_t a, int b)
    {
        return a.multiplication(b);
    }

    [[nodiscard]] constexpr friend fixed_t operator/(fixed_t a, fixed_t b)
    {
        return a.division(b);
    }

    [[nodiscard]] constexpr friend fixed_t operator/(fixed_t a, int b)
    {
        return a.division(b);
    }

    [[nodiscard]] constexpr friend auto operator<=>(fixed_t a, fixed_t b) = default;

private:
    int _data = 0;
};

using fixed = fixed_t<12>;

template<int Precision>
struct hash<fixed_t<Precision>>
{
    [[nodiscard]] constexpr unsigned operator()(fixed_t<Precision> value) const
    {
        return make_hash(value.data());
    }
};

}

#endif
