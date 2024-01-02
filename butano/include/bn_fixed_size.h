/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_SIZE_H
#define BN_FIXED_SIZE_H

/**
 * @file
 * bn::fixed_size_t and bn::fixed_size implementation header file.
 *
 * @ingroup math
 */

#include "bn_size.h"
#include "bn_fixed.h"
#include "bn_fixed_size_fwd.h"

namespace bn
{

template<int Precision>
class fixed_size_t
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr fixed_size_t() = default;

    /**
     * @brief Constructor.
     * @param width Valid width (>= 0).
     * @param height Valid height (>= 0).
     */
    constexpr fixed_size_t(fixed_t<Precision> width, fixed_t<Precision> height) :
        _width(width),
        _height(height)
    {
        BN_ASSERT(width >= 0, "Invalid width: ", width);
        BN_ASSERT(height >= 0, "Invalid height: ", height);
    }

    /**
     * @brief Constructor.
     * @param size Integer size.
     */
    constexpr fixed_size_t(const size& size) :
        _width(size.width()),
        _height(size.height())
    {
        BN_ASSERT(_width >= 0, "Invalid width: ", _width);
        BN_ASSERT(_height >= 0, "Invalid height: ", _height);
    }

    /**
     * @brief Returns the width.
     */
    [[nodiscard]] constexpr fixed_t<Precision> width() const
    {
        return _width;
    }

    /**
     * @brief Sets the width.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(fixed_t<Precision> width)
    {
        BN_ASSERT(width >= 0, "Invalid width: ", width);

        _width = width;
    }

    /**
     * @brief Returns the height.
     */
    [[nodiscard]] constexpr fixed_t<Precision> height() const
    {
        return _height;
    }

    /**
     * @brief Sets the height.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(fixed_t<Precision> height)
    {
        BN_ASSERT(height >= 0, "Invalid height: ", height);

        _height = height;
    }

    /**
     * @brief Returns the multiplication of this size by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t multiplication(int value) const
    {
        return fixed_size_t(_width.multiplication(value), _height.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this size by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t multiplication(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.multiplication(value), _height.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this size by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t safe_multiplication(int value) const
    {
        return fixed_size_t(_width.safe_multiplication(value), _height.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this size by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t safe_multiplication(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.safe_multiplication(value), _height.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this size by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t unsafe_multiplication(int value) const
    {
        return fixed_size_t(_width.unsafe_multiplication(value), _height.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this size by the given fixed point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t unsafe_multiplication(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.unsafe_multiplication(value), _height.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the division of this size by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t division(int value) const
    {
        return fixed_size_t(_width.division(value), _height.division(value));
    }

    /**
     * @brief Returns the division of this size by the given fixed point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t division(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.division(value), _height.division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t safe_division(int value) const
    {
        return fixed_size_t(_width.safe_division(value), _height.safe_division(value));
    }

    /**
     * @brief Returns the division of this size by the given fixed point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t safe_division(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.safe_division(value), _height.safe_division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_size_t unsafe_division(int value) const
    {
        return fixed_size_t(_width.unsafe_division(value), _height.unsafe_division(value));
    }

    /**
     * @brief Returns the division of this size by the given fixed point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_size_t unsafe_division(fixed_t<Precision> value) const
    {
        return fixed_size_t(_width.unsafe_division(value), _height.unsafe_division(value));
    }

    /**
     * @brief Adds the given fixed_size_t to this one.
     * @param other fixed_size_t to add.
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator+=(const fixed_size_t& other)
    {
        set_width(_width + other._width);
        set_height(_height + other._height);
        return *this;
    }

    /**
     * @brief Subtracts the given fixed_size_t to this one.
     * @param other fixed_size_t to subtract.
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator-=(const fixed_size_t& other)
    {
        set_width(_width - other._width);
        set_height(_height - other._height);
        return *this;
    }

    /**
     * @brief Multiplies both width and height by the given factor.
     * @param value Valid integer multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator*=(int value)
    {
        set_width(_width * value);
        set_height(_height * value);
        return *this;
    }

    /**
     * @brief Multiplies both width and height by the given factor.
     * @param value Valid fixed point multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator*=(fixed_t<Precision> value)
    {
        set_width(_width * value);
        set_height(_height * value);
        return *this;
    }

    /**
     * @brief Divides both width and height by the given divisor.
     * @param value Valid integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator/=(int value)
    {
        set_width(_width / value);
        set_height(_height / value);
        return *this;
    }

    /**
     * @brief Divides both width and height by the given divisor.
     * @param value Valid fixed point divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_size_t& operator/=(fixed_t<Precision> value)
    {
        set_width(_width / value);
        set_height(_height / value);
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator+(const fixed_size_t& a, const fixed_size_t& b)
    {
        return fixed_size_t(a._width + b._width, a._height + b._height);
    }

    /**
     * @brief Returns b subtracted from a.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator-(const fixed_size_t& a, const fixed_size_t& b)
    {
        return fixed_size_t(a._width - b._width, a._height - b._height);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator*(const fixed_size_t& a, int b)
    {
        return fixed_size_t(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator*(const fixed_size_t& a, fixed_t<Precision> b)
    {
        return fixed_size_t(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator/(const fixed_size_t& a, int b)
    {
        return fixed_size_t(a._width / b, a._height / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_size_t operator/(const fixed_size_t& a, fixed_t<Precision> b)
    {
        return fixed_size_t(a._width / b, a._height / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const fixed_size_t& a, const fixed_size_t& b) = default;

private:
    fixed_t<Precision> _width;
    fixed_t<Precision> _height;
};


/**
 * @brief Hash support for fixed_size_t.
 *
 * @ingroup math
 * @ingroup functional
 */
template<int Precision>
struct hash<fixed_size_t<Precision>>
{
    /**
     * @brief Returns the hash of the given fixed_size_t.
     */
    [[nodiscard]] constexpr unsigned operator()(const fixed_size_t<Precision>& value) const
    {
        unsigned result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
