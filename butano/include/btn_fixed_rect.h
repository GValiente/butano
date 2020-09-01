#ifndef BTN_FIXED_RECT_H
#define BTN_FIXED_RECT_H

#include "btn_rect.h"
#include "btn_fixed_size.h"
#include "btn_fixed_point.h"

namespace btn
{

class fixed_rect
{

public:
    constexpr fixed_rect() = default;

    constexpr fixed_rect(const fixed_point& position, const fixed_size& dimensions) :
        _position(position),
        _dimensions(dimensions)
    {
    }

    constexpr fixed_rect(fixed x, fixed y, fixed width, fixed height) :
        _position(x, y),
        _dimensions(width, height)
    {
    }

    constexpr explicit fixed_rect(const rect& rect) :
        _position(rect.position()),
        _dimensions(rect.dimensions())
    {
    }

    [[nodiscard]] constexpr const fixed_point& position() const
    {
        return _position;
    }

    constexpr void set_position(const fixed_point& position)
    {
        _position = position;
    }

    constexpr void set_position(fixed x, fixed y)
    {
        _position = btn::fixed_point(x, y);
    }

    [[nodiscard]] constexpr const fixed_size& dimensions() const
    {
        return _dimensions;
    }

    constexpr void set_dimensions(const fixed_size& dimensions)
    {
        _dimensions = dimensions;
    }

    constexpr void set_dimensions(fixed width, fixed height)
    {
        _dimensions = btn::fixed_size(width, height);
    }

    [[nodiscard]] constexpr fixed x() const
    {
        return _position.x();
    }

    constexpr void set_x(fixed x)
    {
        _position.set_x(x);
    }

    [[nodiscard]] constexpr fixed y() const
    {
        return _position.y();
    }

    constexpr void set_y(fixed y)
    {
        _position.set_y(y);
    }

    [[nodiscard]] constexpr fixed width() const
    {
        return _dimensions.width();
    }

    constexpr void set_width(fixed width)
    {
        _dimensions.set_width(width);
    }

    [[nodiscard]] constexpr fixed height() const
    {
        return _dimensions.height();
    }

    constexpr void set_height(fixed height)
    {
        return _dimensions.set_height(height);
    }

    [[nodiscard]] constexpr fixed top() const
    {
        return y() - (height() / 2);
    }

    [[nodiscard]] constexpr fixed left() const
    {
        return x() - (width() / 2);
    }

    [[nodiscard]] constexpr fixed_point top_left() const
    {
        return fixed_point(left(), top());
    }

    [[nodiscard]] constexpr fixed bottom() const
    {
        return y() + (height() / 2);
    }

    [[nodiscard]] constexpr fixed right() const
    {
        return x() + (width() / 2);
    }

    [[nodiscard]] constexpr fixed_point bottom_right() const
    {
        return fixed_point(right(), bottom());
    }

    [[nodiscard]] constexpr bool contains(const fixed_point& point) const
    {
        fixed this_left = left();

        if(this_left < point.x() && this_left + width() > point.x())
        {
            fixed this_top = top();
            return this_top < point.y() && this_top + height() > point.y();
        }

        return false;
    }

    [[nodiscard]] constexpr bool intersects(const fixed_rect& other) const
    {
        fixed this_left = left();
        fixed other_left = other.left();

        if(this_left < other_left + other.width() && this_left + width() > other_left)
        {
            fixed this_top = top();
            fixed other_top = other.top();
            return this_top < other_top + other.height() && height() + this_top > other_top;
        }

        return false;
    }

    constexpr fixed_rect& operator*=(fixed value)
    {
        _dimensions *= value;
        return *this;
    }

    constexpr fixed_rect& operator/=(fixed value)
    {
        _dimensions /= value;
        return *this;
    }

    [[nodiscard]] constexpr friend fixed_rect operator*(const fixed_rect& a, fixed b)
    {
        return fixed_rect(a._position, a._dimensions * b);
    }

    [[nodiscard]] constexpr friend fixed_rect operator/(const fixed_rect& a, fixed b)
    {
        return fixed_rect(a._position, a._dimensions / b);
    }

    [[nodiscard]] constexpr friend bool operator==(const fixed_rect& a, const fixed_rect& b) = default;

private:
    fixed_point _position;
    fixed_size _dimensions;
};


template<>
struct hash<fixed_rect>
{
    [[nodiscard]] constexpr unsigned operator()(const fixed_rect& value) const
    {
        unsigned result = make_hash(value.position());
        hash_combine(value.dimensions(), result);
        return result;
    }
};

}

#endif
