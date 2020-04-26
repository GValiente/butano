#ifndef BTN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H
#define BTN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

enum class sprite_size;

class sprite_regular_second_attributes
{

public:
    sprite_regular_second_attributes(fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip);

    [[nodiscard]] fixed x() const
    {
        return _x;
    }

    void set_x(fixed x)
    {
        _x = x;
    }

    [[nodiscard]] sprite_size size() const;

    void set_size(sprite_size size);

    [[nodiscard]] bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    void set_horizontal_flip(bool horizontal_flip)
    {
        _horizontal_flip = horizontal_flip;
    }

    [[nodiscard]] bool vertical_flip() const
    {
        return _vertical_flip;
    }

    void set_vertical_flip(bool vertical_flip)
    {
        _vertical_flip = vertical_flip;
    }

    [[nodiscard]] friend bool operator==(const sprite_regular_second_attributes& a,
                                         const sprite_regular_second_attributes& b)
    {
        return a._x == b._x && a._size == b._size && a._horizontal_flip == b._horizontal_flip &&
                a._vertical_flip == b._vertical_flip;
    }

    [[nodiscard]] friend bool operator!=(const sprite_regular_second_attributes& a,
                                         const sprite_regular_second_attributes& b)
    {
        return ! (a == b);
    }

private:
    fixed _x;
    uint8_t _size;
    unsigned _horizontal_flip: 1;
    unsigned _vertical_flip: 1;
};

}

#endif

