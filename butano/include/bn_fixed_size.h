/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_SIZE_H
#define BN_FIXED_SIZE_H

/**
 * @file
 * bn::fixed_size header file.
 *
 * @ingroup math
 */

#include "bn_size.h"
#include "bn_fixed.h"

namespace bn
{

/**
 * @brief Defines the size of a two-dimensional object using fixed point precision.
 *
 * @ingroup math
 */
class fixed_size
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr fixed_size() = default;

    /**
     * @brief Constructor.
     * @param width Valid width (>= 0).
     * @param height Valid height (>= 0).
     */
    constexpr fixed_size(fixed width, fixed height) :
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
    constexpr fixed_size(const size& size) :
        _width(size.width()),
        _height(size.height())
    {
    }

    /**
     * @brief Returns the width.
     */
    [[nodiscard]] constexpr fixed width() const
    {
        return _width;
    }

    /**
     * @brief Sets the width.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(fixed width)
    {
        BN_ASSERT(width >= 0, "Invalid width: ", width);

        _width = width;
    }

    /**
     * @brief Returns the height.
     */
    [[nodiscard]] constexpr fixed height() const
    {
        return _height;
    }

    /**
     * @brief Sets the height.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(fixed height)
    {
        BN_ASSERT(height >= 0, "Invalid height: ", height);

        _height = height;
    }

    /**
     * @brief Adds the given fixed_size to this one.
     * @param other fixed_size to add.
     * @return Reference to this.
     */
    constexpr fixed_size& operator+=(const fixed_size& other)
    {
        _width += other._width;
        _height += other._height;
        return *this;
    }

    /**
     * @brief Subtracts the given fixed_size to this one.
     * @param other fixed_size to subtract.
     * @return Reference to this.
     */
    constexpr fixed_size& operator-=(const fixed_size& other)
    {
        _width -= other._width;
        BN_ASSERT(_width >= 0, "Invalid width: ", _width);

        _height -= other._height;
        BN_ASSERT(_height >= 0, "Invalid height: ", _height);

        return *this;
    }

    /**
     * @brief Multiplies both width and height by the given factor.
     * @param value Valid integer multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator*=(int value)
    {
        BN_ASSERT(value >= 0, "Invalid value: ", value);

        _width *= value;
        _height *= value;
        return *this;
    }

    /**
     * @brief Multiplies both width and height by the given factor.
     * @param value Valid unsigned integer multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator*=(unsigned value)
    {
        _width *= value;
        _height *= value;
        return *this;
    }

    /**
     * @brief Multiplies both width and height by the given factor.
     * @param value Valid fixed point multiplication factor (>= 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator*=(fixed value)
    {
        BN_ASSERT(value >= 0, "Invalid value: ", value);

        _width *= value;
        _height *= value;
        return *this;
    }

    /**
     * @brief Divides both width and height by the given divisor.
     * @param value Valid integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator/=(int value)
    {
        BN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    /**
     * @brief Divides both width and height by the given divisor.
     * @param value Valid unsigned integer divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator/=(unsigned value)
    {
        BN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    /**
     * @brief Divides both width and height by the given divisor.
     * @param value Valid fixed point divisor (> 0).
     * @return Reference to this.
     */
    constexpr fixed_size& operator/=(fixed value)
    {
        BN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend fixed_size operator+(const fixed_size& a, const fixed_size& b)
    {
        return fixed_size(a._width + b._width, a._height + b._height);
    }

    /**
     * @brief Returns b subtracted from a.
     */
    [[nodiscard]] constexpr friend fixed_size operator-(const fixed_size& a, const fixed_size& b)
    {
        return fixed_size(a._width - b._width, a._height - b._height);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator*(const fixed_size& a, int b)
    {
        BN_ASSERT(b >= 0, "Invalid value: ", b);

        return fixed_size(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator*(const fixed_size& a, unsigned b)
    {
        return fixed_size(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator*(const fixed_size& a, fixed b)
    {
        BN_ASSERT(b >= 0, "Invalid value: ", b);

        return fixed_size(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator/(const fixed_size& a, int b)
    {
        BN_ASSERT(b > 0, "Invalid value: ", b);

        return fixed_size(a._width / b, a._height / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator/(const fixed_size& a, unsigned b)
    {
        BN_ASSERT(b > 0, "Invalid value: ", b);

        return fixed_size(a._width / b, a._height / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_size operator/(const fixed_size& a, fixed b)
    {
        BN_ASSERT(b > 0, "Invalid value: ", b);

        return fixed_size(a._width / b, a._height / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const fixed_size& a, const fixed_size& b) = default;

private:
    fixed _width;
    fixed _height;
};


/**
 * @brief Hash support for fixed_size.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<fixed_size>
{
    /**
     * @brief Returns the hash of the given fixed_size.
     */
    [[nodiscard]] constexpr unsigned operator()(const fixed_size& value) const
    {
        unsigned result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
