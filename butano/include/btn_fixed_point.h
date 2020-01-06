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

    [[nodiscard]] constexpr fixed_point operator+(const fixed_point& other) const
    {
        return fixed_point(_x + other._x, _y + other._y);
    }

    [[nodiscard]] constexpr fixed_point operator-(const fixed_point& other) const
    {
        return fixed_point(_x - other._x, _y - other._y);
    }

    [[nodiscard]] constexpr fixed_point operator*(int value) const
    {
        return fixed_point(_x * value, _y * value);
    }

    [[nodiscard]] constexpr fixed_point operator*(fixed value) const
    {
        return fixed_point(_x * value, _y * value);
    }

    [[nodiscard]] constexpr fixed_point operator/(int value) const
    {
        BTN_CONSTEXPR_ASSERT(value != 0, "Invalid value");

        return fixed_point(_x / value, _y / value);
    }

    [[nodiscard]] constexpr fixed_point operator/(fixed value) const
    {
        BTN_CONSTEXPR_ASSERT(value != 0, "Invalid value");

        return fixed_point(_x / value, _y / value);
    }

    [[nodiscard]] constexpr bool operator==(const fixed_point& other) const
    {
        return _x == other._x && _y == other._y;
    }

    [[nodiscard]] constexpr bool operator!=(const fixed_point& other) const
    {
        return ! (*this == other);
    }

private:
    fixed _x = 0;
    fixed _y = 0;
};


template<>
struct hash<fixed_point>
{
    [[nodiscard]] constexpr size_t operator()(const fixed_point& value) const
    {
        size_t result = make_hash(value.x());
        hash_combine(value.y(), result);
        return result;
    }
};

}

#endif
