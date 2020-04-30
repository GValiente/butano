#ifndef BTN_RECT_H
#define BTN_RECT_H

#include "btn_size.h"
#include "btn_point.h"

namespace btn
{

class rect
{

public:
    constexpr rect() = default;

    constexpr rect(const point& position, const size& dimensions) :
        _position(position),
        _dimensions(dimensions)
    {
    }

    constexpr rect(int x, int y, int width, int height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    [[nodiscard]] constexpr const point& position() const
    {
        return _position;
    }

    constexpr void set_position(const point& position)
    {
        _position = position;
    }

    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    constexpr void set_dimensions(const size& dimensions)
    {
        _dimensions = dimensions;
    }

    [[nodiscard]] constexpr int x() const
    {
        return _position.x();
    }

    constexpr void set_x(int x)
    {
        _position.set_x(x);
    }

    [[nodiscard]] constexpr int y() const
    {
        return _position.y();
    }

    constexpr void set_y(int y)
    {
        _position.set_y(y);
    }

    [[nodiscard]] constexpr int width() const
    {
        return _dimensions.width();
    }

    constexpr void set_width(int width)
    {
        _dimensions.set_width(width);
    }

    [[nodiscard]] constexpr int height() const
    {
        return _dimensions.height();
    }

    constexpr void set_height(int height)
    {
        return _dimensions.set_height(height);
    }

    [[nodiscard]] constexpr int top() const
    {
        return y() - (height() / 2);
    }

    [[nodiscard]] constexpr int left() const
    {
        return x() - (width() / 2);
    }

    [[nodiscard]] constexpr point top_left() const
    {
        return point(left(), top());
    }

    [[nodiscard]] constexpr int bottom() const
    {
        return y() + (height() / 2);
    }

    [[nodiscard]] constexpr int right() const
    {
        return x() + (width() / 2);
    }

    [[nodiscard]] constexpr point bottom_right() const
    {
        return point(right(), bottom());
    }

    [[nodiscard]] constexpr bool contains(const point& point) const
    {
        int this_left = left();

        if(this_left < point.x() && this_left + width() > point.x())
        {
            int this_top = top();
            return this_top < point.y() && this_top + height() > point.y();
        }

        return false;
    }

    [[nodiscard]] constexpr bool intersects(const rect& other) const
    {
        int this_left = left();
        int other_left = other.left();

        if(this_left < other_left + other.width() && this_left + width() > other_left)
        {
            int this_top = top();
            int other_top = other.top();
            return this_top < other_top + other.height() && height() + this_top > other_top;
        }

        return false;
    }

    constexpr rect& operator*=(int value)
    {
        _dimensions *= value;
        return *this;
    }

    constexpr rect& operator/=(int value)
    {
        _dimensions /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend rect operator*(const rect& a, int b)
    {
        return rect(a._position, a._dimensions * b);
    }

    [[nodiscard]] constexpr friend rect operator/(const rect& a, int b)
    {
        return rect(a._position, a._dimensions / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const rect& a, const rect& b)
    {
        return a._position == b._position && a._dimensions == b._dimensions;
    }

    [[nodiscard]] constexpr friend bool operator!=(const rect& a, const rect& b)
    {
        return ! (a == b);
    }

private:
    point _position;
    size _dimensions;
};


template<>
struct hash<rect>
{
    [[nodiscard]] constexpr unsigned operator()(const rect& value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
