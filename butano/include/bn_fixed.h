/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_H
#define BN_FIXED_H

/**
 * @file
 * bn::fixed_t and bn::fixed implementation header file.
 *
 * @ingroup math
 */

#include "bn_compare.h"
#include "bn_fixed_fwd.h"
#include "bn_algorithm.h"
#include "bn_functional.h"

namespace bn
{

template<int Precision>
class fixed_t
{
    static_assert(Precision > 0 && Precision < 31, "Invalid precision");

public:
    /**
     * @brief Returns a fixed_t with the given internal data.
     *
     * If you want to convert an integer value to fixed_t, you should use its constructor.
     */
    [[nodiscard]] static constexpr fixed_t from_data(int data)
    {
        fixed_t result;
        result._data = data;
        return result;
    }

    /**
     * @brief Returns the number of bits used for the fractional part.
     */
    [[nodiscard]] static constexpr int precision()
    {
        return Precision;
    }

    /**
     * @brief Returns the internal data of fixed_t<Precision>(1);
     */
    [[nodiscard]] static constexpr int scale()
    {
        return 1 << Precision;
    }

    /**
     * @brief Returns the internal data of fixed_t<Precision / 2>(1);
     */
    [[nodiscard]] static constexpr int half_scale()
    {
        static_assert(Precision % 2 == 0, "Invalid precision");

        return 1 << (Precision / 2);
    }

    /**
     * @brief Default constructor.
     */
    constexpr fixed_t() = default;

    /**
     * @brief Constructor.
     * @param value Integer value.
     */
    constexpr fixed_t(int value) :
        _data(value * scale())
    {
    }

    /**
     * @brief Constructor.
     * @param value Unsigned integer value.
     */
    constexpr fixed_t(unsigned value) :
        _data(value << Precision)
    {
    }

    /**
     * @brief Constructor.
     * @param value Single precision floating point value.
     */
    constexpr fixed_t(float value) :
        _data(int(value * scale()))
    {
    }

    /**
     * @brief Constructor.
     * @param value Double precision floating point value.
     */
    constexpr fixed_t(double value) :
        _data(int(value * scale()))
    {
    }

    /**
     * @brief Constructor.
     * @param other Fixed point value.
     */
    template<int OtherPrecision>
    constexpr fixed_t(fixed_t<OtherPrecision> other) :
        _data(Precision < OtherPrecision ?
                   other.data() / (other.scale() / scale()) :
                   other.data() * (scale() / other.scale()))
    {
    }

    /**
     * @brief Returns the internal data.
     */
    [[nodiscard]] constexpr int data() const
    {
        return _data;
    }

    /**
     * @brief Returns the integer part using a division.
     */
    [[nodiscard]] constexpr int integer() const
    {
        return _data / scale();
    }

    /**
     * @brief Returns the integer part using a division.
     */
    [[nodiscard]] constexpr explicit operator int() const
    {
        return integer();
    }

    /**
     * @brief Returns the integer part using a right shift.
     */
    [[nodiscard]] constexpr int right_shift_integer() const
    {
        return _data >> Precision;
    }

    /**
     * @brief Returns the integer part, assuming that it is >= 0.
     */
    [[nodiscard]] constexpr unsigned unsigned_integer() const
    {
        return _data >> Precision;
    }

    /**
     * @brief Returns the nearest integer value.
     */
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

    /**
     * @brief Returns the largest integer value not greater than this value.
     */
    [[nodiscard]] constexpr int floor_integer() const
    {
        int data = _data;
        int result = data / scale();

        if(data < 0 && data % scale())
        {
            --result;
        }

        return result;
    }

    /**
     * @brief Returns the smallest integer value not less than this value.
     */
    [[nodiscard]] constexpr int ceil_integer() const
    {
        int data = _data;
        int result = data / scale();

        if(data >= 0 && data % scale())
        {
            ++result;
        }

        return result;
    }

    /**
     * @brief Returns the fractional part.
     */
    [[nodiscard]] constexpr int fraction() const
    {
        return _data & (scale() - 1);
    }

    /**
     * @brief Returns the nearest single precision floating point value.
     */
    [[nodiscard]] constexpr float to_float() const
    {
        return float(_data) / scale();
    }

    /**
     * @brief Returns the nearest single precision floating point value.
     */
    [[nodiscard]] constexpr explicit operator float() const
    {
        return to_float();
    }

    /**
     * @brief Returns the nearest double precision floating point value.
     */
    [[nodiscard]] constexpr double to_double() const
    {
        return double(_data) / scale();
    }

    /**
     * @brief Returns the nearest double precision floating point value.
     */
    [[nodiscard]] constexpr explicit operator double() const
    {
        return to_double();
    }

    /**
     * @brief Returns the multiplication of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t multiplication(int value) const
    {
        return from_data(_data * value);
    }

    /**
     * @brief Returns the multiplication of this value by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
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

    /**
     * @brief Returns the multiplication of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t safe_multiplication(int value) const
    {
        return multiplication(value);
    }

    /**
     * @brief Returns the multiplication of this value by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_t safe_multiplication(fixed_t other) const
    {
        return from_data(int((int64_t(_data) * other._data) / scale()));
    }

    /**
     * @brief Returns the multiplication of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t unsafe_multiplication(int value) const
    {
        return multiplication(value);
    }

    /**
     * @brief Returns the multiplication of this value by the given fixed point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_t unsafe_multiplication(fixed_t other) const
    {
        return from_data((_data * other._data) / scale());
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t division(int value) const
    {
        return from_data(_data / value);
    }

    /**
     * @brief Returns the division of this value by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_t division(fixed_t other) const
    {
        if constexpr(Precision % 2 == 0)
        {
            int data = _data * half_scale();
            int other_data = other._data / half_scale();
            return from_data(data / other_data);
        }

        return safe_division(other);
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t safe_division(int value) const
    {
        return division(value);
    }

    /**
     * @brief Returns the division of this value by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_t safe_division(fixed_t other) const
    {
        return from_data(int((int64_t(_data) * scale()) / other._data));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t unsafe_division(int value) const
    {
        return division(value);
    }

    /**
     * @brief Returns the division of this value by the given fixed point value without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_t unsafe_division(fixed_t other) const
    {
        return from_data((_data * scale()) / other._data);
    }

    /**
     * @brief Returns the remainder of the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_t modulo(int value) const
    {
        int n = division(value).integer();
        return *this - (n * value);
    }

    /**
     * @brief Returns a fixed_t that is formed by changing the sign of this one.
     */
    [[nodiscard]] constexpr fixed_t operator-() const
    {
        return from_data(-_data);
    }

    /**
     * @brief Adds the given fixed_t to this one.
     * @param other fixed_t to add.
     * @return Reference to this.
     */
    constexpr fixed_t& operator+=(fixed_t other)
    {
        _data += other._data;
        return *this;
    }

    /**
     * @brief Subtracts the given fixed_t to this one.
     * @param other fixed_t to subtract.
     * @return Reference to this.
     */
    constexpr fixed_t& operator-=(fixed_t other)
    {
        _data -= other._data;
        return *this;
    }

    /**
     * @brief Multiplies this fixed_t by the given factor.
     * @param value Integer multiplication factor.
     * @return Reference to this.
     */
    constexpr fixed_t& operator*=(int value)
    {
        *this = multiplication(value);
        return *this;
    }

    /**
     * @brief Multiplies this fixed_t by the given factor.
     * @param other Fixed point multiplication factor.
     * @return Reference to this.
     */
    constexpr fixed_t& operator*=(fixed_t other)
    {
        *this = multiplication(other);
        return *this;
    }

    /**
     * @brief Divides this fixed_t by the given divisor.
     * @param value Valid integer divisor (!= 0).
     * @return Reference to this.
     */
    constexpr fixed_t& operator/=(int value)
    {
        *this = division(value);
        return *this;
    }

    /**
     * @brief Divides this fixed_t by the given divisor.
     * @param other Valid fixed point divisor (!= 0).
     * @return Reference to this.
     */
    constexpr fixed_t& operator/=(fixed_t other)
    {
        *this = division(other);
        return *this;
    }

    /**
     * @brief Assigns the remainder of the division of this fixed_t by the given divisor.
     * @param other Valid integer divisor (!= 0).
     * @return Reference to this.
     */
    constexpr fixed_t& operator%=(int other)
    {
        *this = modulo(other);
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend fixed_t operator+(fixed_t a, fixed_t b)
    {
        return from_data(a._data + b._data);
    }

    /**
     * @brief Returns the sum of a and b.
     */
    template<int OtherPrecision>
    [[nodiscard]] constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator+(
            fixed_t a, fixed_t<OtherPrecision> b)
    {
        using output_fixed_t = fixed_t<bn::max(Precision, OtherPrecision)>;

        return output_fixed_t(a) + output_fixed_t(b);
    }

    /**
     * @brief Returns the subtraction of a and b.
     */
    [[nodiscard]] constexpr friend fixed_t operator-(fixed_t a, fixed_t b)
    {
        return from_data(a._data - b._data);
    }

    /**
     * @brief Returns the subtraction of a and b.
     */
    template<int OtherPrecision>
    [[nodiscard]] constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator-(
        fixed_t a, fixed_t<OtherPrecision> b)
    {
        using output_fixed_t = fixed_t<bn::max(Precision, OtherPrecision)>;

        return output_fixed_t(a) - output_fixed_t(b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_t operator*(fixed_t a, int b)
    {
        return a.multiplication(b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_t operator*(fixed_t a, fixed_t b)
    {
        return a.multiplication(b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    template<int OtherPrecision>
    [[nodiscard]] constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator*(
        fixed_t a, fixed_t<OtherPrecision> b)
    {
        using output_fixed_t = fixed_t<bn::max(Precision, OtherPrecision)>;

        return output_fixed_t(a) * output_fixed_t(b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_t operator/(fixed_t a, int b)
    {
        return a.division(b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_t operator/(fixed_t a, fixed_t b)
    {
        return a.division(b);
    }

    /**
     * @brief Returns a divided by b.
     */
    template<int OtherPrecision>
    [[nodiscard]] constexpr friend fixed_t<bn::max(Precision, OtherPrecision)> operator/(
        fixed_t a, fixed_t<OtherPrecision> b)
    {
        using output_fixed_t = fixed_t<bn::max(Precision, OtherPrecision)>;

        return output_fixed_t(a) / output_fixed_t(b);
    }

    /**
     * @brief Returns the remainder of the division of a by b.
     */
    [[nodiscard]] constexpr friend fixed_t operator%(fixed_t a, int b)
    {
        return a.modulo(b);
    }

    /**
     * @brief Default three-way comparison operator.
     */
    [[nodiscard]] constexpr friend auto operator<=>(fixed_t a, fixed_t b) = default;

private:
    int _data = 0;
};


/**
 * @brief Hash support for fixed_t.
 *
 * @ingroup math
 * @ingroup functional
 */
template<int Precision>
struct hash<fixed_t<Precision>>
{
    /**
     * @brief Returns the hash of the given fixed_t.
     */
    [[nodiscard]] constexpr unsigned operator()(fixed_t<Precision> value) const
    {
        return make_hash(value.data());
    }
};

}

#endif
