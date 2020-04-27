#ifndef BTN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H
#define BTN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

class sprite_regular_second_attributes
{

public:
    constexpr sprite_regular_second_attributes() = default;

    constexpr sprite_regular_second_attributes(fixed x, bool horizontal_flip, bool vertical_flip) :
        _x(x),
        _horizontal_flip(horizontal_flip),
        _vertical_flip(vertical_flip)
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

    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _horizontal_flip = horizontal_flip;
    }

    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vertical_flip;
    }

    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vertical_flip = vertical_flip;
    }

    [[nodiscard]] constexpr friend bool operator==(const sprite_regular_second_attributes& a,
                                                   const sprite_regular_second_attributes& b)
    {
        return a._x == b._x && a._horizontal_flip == b._horizontal_flip && a._vertical_flip == b._vertical_flip;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_regular_second_attributes& a,
                                                   const sprite_regular_second_attributes& b)
    {
        return ! (a == b);
    }

private:
    fixed _x = 0;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
};

}

#endif

