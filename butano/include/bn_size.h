/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SIZE_H
#define BN_SIZE_H

/**
 * @file
 * bn::size header file.
 *
 * @ingroup math
 */

#include "bn_assert.h"
#include "bn_functional.h"

namespace bn
{

/**
 * @brief Defines the size of a two-dimensional object using integer precision.
 *
 * @ingroup math
 */
class size
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr size() = default;

    /**
     * @brief Constructor.
     * @param width Valid width (>= 0).
     * @param height Valid height (>= 0).
     */
    constexpr size(int width, int height) :
        _width(width),
        _height(height)
    {
        BN_ASSERT(width >= 0, "Invalid width: ", width);
        BN_ASSERT(height >= 0, "Invalid height: ", height);
    }

    /**
     * @brief Returns the width.
     */
    [[nodiscard]] constexpr int width() const
    {
        return _width;
    }

    /**
     * @brief Sets the width.
     * @param width Valid width (>= 0).
     */
    constexpr void set_width(int width)
    {
        BN_ASSERT(width >= 0, "Invalid width: ", width);

        _width = width;
    }

    /**
     * @brief Returns the height.
     */
    [[nodiscard]] constexpr int height() const
    {
        return _height;
    }

    /**
     * @brief Sets the height.
     * @param height Valid height (>= 0).
     */
    constexpr void set_height(int height)
    {
        BN_ASSERT(height >= 0, "Invalid height: ", height);

        _height = height;
    }

    /**
     * @brief Adds the given size to this one.
     * @param other size to add.
     * @return Reference to this.
     */
    constexpr size& operator+=(const size& other)
    {
        _width += other._width;
        _height += other._height;
        return *this;
    }

    /**
     * @brief Subtracts the given size to this one.
     * @param other size to subtract.
     * @return Reference to this.
     */
    constexpr size& operator-=(const size& other)
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
    constexpr size& operator*=(int value)
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
    constexpr size& operator/=(int value)
    {
        BN_ASSERT(value > 0, "Invalid value: ", value);

        _width /= value;
        _height /= value;
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend size operator+(const size& a, const size& b)
    {
        return size(a._width + b._width, a._height + b._height);
    }

    /**
     * @brief Returns b subtracted from a.
     */
    [[nodiscard]] constexpr friend size operator-(const size& a, const size& b)
    {
        return size(a._width - b._width, a._height - b._height);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend size operator*(const size& a, int b)
    {
        BN_ASSERT(b >= 0, "Invalid value: ", b);

        return size(a._width * b, a._height * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend size operator/(const size& a, int b)
    {
        BN_ASSERT(b > 0, "Invalid value: ", b);

        return size(a._width / b, a._height / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const size& a, const size& b) = default;

private:
    int _width = 0;
    int _height = 0;
};


/**
 * @brief Hash support for size.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<size>
{
    /**
     * @brief Returns the hash of the given size.
     */
    [[nodiscard]] constexpr unsigned operator()(const size& value) const
    {
        unsigned result = make_hash(value.width());
        hash_combine(value.height(), result);
        return result;
    }
};

}

#endif
