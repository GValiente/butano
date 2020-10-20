/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_POINT_H
#define BTN_POINT_H

#include "btn_assert.h"
#include "btn_functional.h"

namespace btn
{

class point
{

public:
    constexpr point() = default;

    constexpr point(int x, int y) :
        _x(x),
        _y(y)
    {
    }

    [[nodiscard]] constexpr int x() const
    {
        return _x;
    }

    constexpr void set_x(int x)
    {
        _x = x;
    }

    [[nodiscard]] constexpr int y() const
    {
        return _y;
    }

    constexpr void set_y(int y)
    {
        _y = y;
    }

    [[nodiscard]] constexpr point operator-() const
    {
        return point(-_x, -_y);
    }

    constexpr point& operator+=(const point& other)
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    constexpr point& operator-=(const point& other)
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    constexpr point& operator*=(int value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    constexpr point& operator/=(int value)
    {
        BTN_ASSERT(value != 0, "Invalid value: ", value);

        _x /= value;
        _y /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend point operator+(const point& a, const point& b)
    {
        return point(a._x + b._x, a._y + b._y);
    }

    [[nodiscard]] constexpr friend point operator-(const point& a, const point& b)
    {
        return point(a._x - b._x, a._y - b._y);
    }

    [[nodiscard]] constexpr friend point operator*(const point& a, int b)
    {
        return point(a._x * b, a._y * b);
    }

    [[nodiscard]] constexpr friend point operator/(const point& a, int b)
    {
        BTN_ASSERT(b != 0, "Invalid value: ", b);

        return point(a._x / b, a._y / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const point& a, const point& b) = default;

private:
    int _x = 0;
    int _y = 0;
};


template<>
struct hash<point>
{
    [[nodiscard]] constexpr unsigned operator()(const point& value) const
    {
        unsigned result = make_hash(value.x());
        hash_combine(value.y(), result);
        return result;
    }
};

}

#endif
