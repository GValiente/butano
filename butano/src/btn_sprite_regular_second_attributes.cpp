#include "btn_sprite_regular_second_attributes.h"

#include "btn_sprite_shape_size.h"

namespace btn
{

sprite_regular_second_attributes::sprite_regular_second_attributes(
        fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip) :
    _x(x),
    _size(uint8_t(size)),
    _horizontal_flip(horizontal_flip),
    _vertical_flip(vertical_flip)
{
}

sprite_size sprite_regular_second_attributes::size() const
{
    return sprite_size(_size);
}

void sprite_regular_second_attributes::set_size(sprite_size size)
{
    _size = uint8_t(size);
}

}
