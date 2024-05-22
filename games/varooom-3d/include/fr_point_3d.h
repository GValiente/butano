/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_POINT_3D_H
#define FR_POINT_3D_H

#include "bn_fixed.h"

namespace fr
{

class point_3d
{

public:
    constexpr point_3d() = default;

    constexpr point_3d(bn::fixed x, bn::fixed y, bn::fixed z) :
        _x(x),
        _y(y),
        _z(z)
    {
    }

    [[nodiscard]] constexpr bn::fixed x() const
    {
        return _x;
    }

    constexpr void set_x(bn::fixed x)
    {
        _x = x;
    }

    [[nodiscard]] constexpr bn::fixed y() const
    {
        return _y;
    }

    constexpr void set_y(bn::fixed y)
    {
        _y = y;
    }

    [[nodiscard]] constexpr bn::fixed z() const
    {
        return _z;
    }

    constexpr void set_z(bn::fixed z)
    {
        _z = z;
    }

    [[nodiscard]] constexpr bn::fixed dot_product(const point_3d& other) const
    {
        return _x.multiplication(other._x) + _y.multiplication(other._y) +
                _z.multiplication(other._z);
    }

    [[nodiscard]] constexpr bn::fixed unsafe_dot_product(const point_3d& other) const
    {
        return _x.unsafe_multiplication(other._x) + _y.unsafe_multiplication(other._y) +
                _z.unsafe_multiplication(other._z);
    }

    [[nodiscard]] constexpr bn::fixed safe_dot_product(const point_3d& other) const
    {
        return _x.safe_multiplication(other._x) + _y.safe_multiplication(other._y) +
                _z.safe_multiplication(other._z);
    }

    [[nodiscard]] constexpr bn::fixed vertical_dot_product(const point_3d& other) const
    {
        return _x.multiplication(other._x) + _z.multiplication(other._z);
    }

    [[nodiscard]] constexpr bn::fixed unsafe_vertical_dot_product(const point_3d& other) const
    {
        return _x.unsafe_multiplication(other._x) + _z.unsafe_multiplication(other._z);
    }

    [[nodiscard]] constexpr bn::fixed safe_vertical_dot_product(const point_3d& other) const
    {
        return _x.safe_multiplication(other._x) + _z.safe_multiplication(other._z);
    }

    [[nodiscard]] constexpr point_3d cross_product(const point_3d& other) const
    {
        return point_3d(_y.multiplication(other._z) - _z.multiplication(other._y),
                _z.multiplication(other._x) - _x.multiplication(other._z),
                _x.multiplication(other._y) - _y.multiplication(other._x));
    }

    [[nodiscard]] constexpr point_3d unsafe_cross_product(const point_3d& other) const
    {
        return point_3d(_y.unsafe_multiplication(other._z) - _z.unsafe_multiplication(other._y),
                _z.unsafe_multiplication(other._x) - _x.unsafe_multiplication(other._z),
                _x.unsafe_multiplication(other._y) - _y.unsafe_multiplication(other._x));
    }

    [[nodiscard]] constexpr point_3d safe_cross_product(const point_3d& other) const
    {
        return point_3d(_y.safe_multiplication(other._z) - _z.safe_multiplication(other._y),
                _z.safe_multiplication(other._x) - _x.safe_multiplication(other._z),
                _x.safe_multiplication(other._y) - _y.safe_multiplication(other._x));
    }

    [[nodiscard]] constexpr point_3d operator-() const
    {
        return point_3d(-_x, -_y, -_z);
    }

    constexpr point_3d& operator+=(const point_3d& other)
    {
        _x += other._x;
        _y += other._y;
        _z += other._z;
        return *this;
    }

    constexpr point_3d& operator-=(const point_3d& other)
    {
        _x -= other._x;
        _y -= other._y;
        _z -= other._z;
        return *this;
    }

    constexpr point_3d& operator*=(int value)
    {
        _x *= value;
        _y *= value;
        _z *= value;
        return *this;
    }

    constexpr point_3d& operator*=(unsigned value)
    {
        _x *= value;
        _y *= value;
        _z *= value;
        return *this;
    }

    constexpr point_3d& operator*=(bn::fixed value)
    {
        _x *= value;
        _y *= value;
        _z *= value;
        return *this;
    }

    constexpr point_3d& operator/=(int value)
    {
        _x /= value;
        _y /= value;
        _z /= value;
        return *this;
    }

    constexpr point_3d& operator/=(unsigned value)
    {
        _x /= value;
        _y /= value;
        _z /= value;
        return *this;
    }

    constexpr point_3d& operator/=(bn::fixed value)
    {
        _x /= value;
        _y /= value;
        _z /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend point_3d operator+(const point_3d& a, const point_3d& b)
    {
        return point_3d(a._x + b._x, a._y + b._y, a._z + b._z);
    }

    [[nodiscard]] constexpr friend point_3d operator-(const point_3d& a, const point_3d& b)
    {
        return point_3d(a._x - b._x, a._y - b._y, a._z - b._z);
    }

    [[nodiscard]] constexpr friend point_3d operator*(const point_3d& a, int b)
    {
        return point_3d(a._x * b, a._y * b, a._z * b);
    }

    [[nodiscard]] constexpr friend point_3d operator*(const point_3d& a, unsigned b)
    {
        return point_3d(a._x * b, a._y * b, a._z * b);
    }

    [[nodiscard]] constexpr friend point_3d operator*(const point_3d& a, bn::fixed b)
    {
        return point_3d(a._x * b, a._y * b, a._z * b);
    }

    [[nodiscard]] constexpr friend point_3d operator/(const point_3d& a, int b)
    {
        return point_3d(a._x / b, a._y / b, a._z / b);
    }

    [[nodiscard]] constexpr friend point_3d operator/(const point_3d& a, unsigned b)
    {
        return point_3d(a._x / b, a._y / b, a._z / b);
    }

    [[nodiscard]] constexpr friend point_3d operator/(const point_3d& a, bn::fixed b)
    {
        return point_3d(a._x / b, a._y / b, a._z / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const point_3d& a, const point_3d& b) = default;

private:
    bn::fixed _x = 0;
    bn::fixed _y = 0;
    bn::fixed _z = 0;
};


class vertex_3d
{

public:
    constexpr vertex_3d(bn::fixed x, bn::fixed y, bn::fixed z) :
        _point(x, y, z),
        _xy(x.safe_multiplication(y))
    {
    }

    constexpr explicit vertex_3d(const point_3d& point) :
        _point(point),
        _xy(point.x().safe_multiplication(point.y()))
    {
    }

    [[nodiscard]] constexpr const point_3d& point() const
    {
        return _point;
    }

    [[nodiscard]] constexpr bn::fixed xy() const
    {
        return _xy;
    }

private:
    point_3d _point;
    bn::fixed _xy;
};

}

#endif
