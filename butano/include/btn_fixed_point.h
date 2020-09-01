#ifndef BTN_FIXED_POINT_H
#define BTN_FIXED_POINT_H

#include "btn_fixed.h"
#include "btn_point.h"

namespace btn
{

class fixed_point
{

public:
    constexpr fixed_point() = default;

    constexpr fixed_point(fixed x, fixed y) :
        _x(x),
        _y(y)
    {
    }

    constexpr explicit fixed_point(const point& point) :
        _x(point.x()),
        _y(point.y())
    {
    }

    [[nodiscard]] constexpr fixed x() const
    {
        return _x;
    }

    constexpr void set_x(fixed x)
    {
        _x = x;
    }

    [[nodiscard]] constexpr fixed y() const
    {
        return _y;
    }

    constexpr void set_y(fixed y)
    {
        _y = y;
    }

    [[nodiscard]] constexpr fixed_point operator-() const
    {
        return fixed_point(-_x, -_y);
    }

    constexpr fixed_point& operator+=(const fixed_point& other)
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    constexpr fixed_point& operator-=(const fixed_point& other)
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    constexpr fixed_point& operator*=(int value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    constexpr fixed_point& operator*=(fixed value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    constexpr fixed_point& operator/=(int value)
    {
        BTN_CONSTEXPR_ASSERT(value != 0, "Invalid value");

        _x /= value;
        _y /= value;
        return *this;
    }

    constexpr fixed_point& operator/=(fixed value)
    {
        BTN_CONSTEXPR_ASSERT(value != 0, "Invalid value");

        _x /= value;
        _y /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend fixed_point operator+(const fixed_point& a, const fixed_point& b)
    {
        return fixed_point(a._x + b._x, a._y + b._y);
    }

    [[nodiscard]] constexpr friend fixed_point operator-(const fixed_point& a, const fixed_point& b)
    {
        return fixed_point(a._x - b._x, a._y - b._y);
    }

    [[nodiscard]] constexpr friend fixed_point operator*(const fixed_point& a, int b)
    {
        return fixed_point(a._x * b, a._y * b);
    }

    [[nodiscard]] constexpr friend fixed_point operator*(const fixed_point& a, fixed b)
    {
        return fixed_point(a._x * b, a._y * b);
    }

    [[nodiscard]] constexpr friend fixed_point operator/(const fixed_point& a, int b)
    {
        BTN_CONSTEXPR_ASSERT(b != 0, "Invalid value");

        return fixed_point(a._x / b, a._y / b);
    }

    [[nodiscard]] constexpr friend fixed_point operator/(const fixed_point& a, fixed b)
    {
        BTN_CONSTEXPR_ASSERT(b != 0, "Invalid value");

        return fixed_point(a._x / b, a._y / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const fixed_point& a, const fixed_point& b) = default;

private:
    fixed _x = 0;
    fixed _y = 0;
};


template<>
struct hash<fixed_point>
{
    [[nodiscard]] constexpr unsigned operator()(const fixed_point& value) const
    {
        unsigned result = make_hash(value.x());
        hash_combine(value.y(), result);
        return result;
    }
};

}

#endif
