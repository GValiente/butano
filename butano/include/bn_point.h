/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_POINT_H
#define BN_POINT_H

/**
 * @file
 * bn::point header file.
 *
 * @ingroup math
 */

#include "bn_assert.h"
#include "bn_functional.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional point using integer precision.
 *
 * @ingroup math
 */
class point
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr point() = default;

    /**
     * @brief Constructor.
     * @param x Horizontal coordinate.
     * @param y Vertical coordinate.
     */
    constexpr point(int x, int y) :
        _x(x),
        _y(y)
    {
    }

    /**
     * @brief Returns the horizontal coordinate.
     */
    [[nodiscard]] constexpr int x() const
    {
        return _x;
    }

    /**
     * @brief Sets the horizontal coordinate.
     */
    constexpr void set_x(int x)
    {
        _x = x;
    }

    /**
     * @brief Returns the vertical coordinate.
     */
    [[nodiscard]] constexpr int y() const
    {
        return _y;
    }

    /**
     * @brief Sets the vertical coordinate.
     */
    constexpr void set_y(int y)
    {
        _y = y;
    }

    /**
     * @brief Returns a point that is formed by changing the sign of both coordinates.
     */
    [[nodiscard]] constexpr point operator-() const
    {
        return point(-_x, -_y);
    }

    /**
     * @brief Adds the given point to this one.
     * @param other point to add.
     * @return Reference to this.
     */
    constexpr point& operator+=(const point& other)
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    /**
     * @brief Subtracts the given point to this one.
     * @param other point to subtract.
     * @return Reference to this.
     */
    constexpr point& operator-=(const point& other)
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    /**
     * @brief Multiplies both coordinates by the given factor.
     * @param value Integer multiplication factor.
     * @return Reference to this.
     */
    constexpr point& operator*=(int value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    /**
     * @brief Divides both coordinates by the given divisor.
     * @param value Valid integer divisor (!= 0).
     * @return Reference to this.
     */
    constexpr point& operator/=(int value)
    {
        BN_ASSERT(value != 0, "Invalid value: ", value);

        _x /= value;
        _y /= value;
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend point operator+(const point& a, const point& b)
    {
        return point(a._x + b._x, a._y + b._y);
    }

    /**
     * @brief Returns b subtracted from a.
     */
    [[nodiscard]] constexpr friend point operator-(const point& a, const point& b)
    {
        return point(a._x - b._x, a._y - b._y);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend point operator*(const point& a, int b)
    {
        return point(a._x * b, a._y * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend point operator/(const point& a, int b)
    {
        BN_ASSERT(b != 0, "Invalid value: ", b);

        return point(a._x / b, a._y / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const point& a, const point& b) = default;

private:
    int _x = 0;
    int _y = 0;
};


/**
 * @brief Hash support for point.
 *
 * @ingroup math
 * @ingroup functional
 */
template<>
struct hash<point>
{
    /**
     * @brief Returns the hash of the given point.
     */
    [[nodiscard]] constexpr unsigned operator()(const point& value) const
    {
        unsigned result = make_hash(value.x());
        hash_combine(value.y(), result);
        return result;
    }
};

}

#endif
