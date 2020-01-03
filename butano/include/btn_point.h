#ifndef BTN_POINT_H
#define BTN_POINT_H

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

    [[nodiscard]] constexpr bool operator==(const point& other) const
    {
        return _x == other._x && _y == other._y;
    }

    [[nodiscard]] constexpr bool operator!=(const point& other) const
    {
        return ! (*this == other);
    }

private:
    int _x = 0;
    int _y = 0;
};


template<>
struct hash<point>
{
    [[nodiscard]] constexpr size_t operator()(const point& value) const
    {
        size_t result = make_hash(value.x());
        hash_combine(value.y(), result);
        return result;
    }
};

}

#endif
