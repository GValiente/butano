#include "btn_second_sprite_attributes.h"

#include "btn_assert.h"
#include "btn_sprite_shape_size.h"

namespace btn
{

second_sprite_attributes::second_sprite_attributes(fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip,
                                                   const optional<sprite_affine_mat_ptr>& affine_mat) :
    _x(x),
    _affine_mat(affine_mat),
    _size(uint8_t(size)),
    _horizontal_flip(horizontal_flip),
    _vertical_flip(vertical_flip)
{
    BTN_ASSERT(! _affine_mat || (! horizontal_flip && ! vertical_flip),
               "Affine mat and flip not allowed at the same time");
}

second_sprite_attributes::second_sprite_attributes(fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip,
                                                   optional<sprite_affine_mat_ptr>&& affine_mat) :
    _x(x),
    _affine_mat(move(affine_mat)),
    _size(uint8_t(size)),
    _horizontal_flip(horizontal_flip),
    _vertical_flip(vertical_flip)
{
    BTN_ASSERT(! _affine_mat || (! horizontal_flip && ! vertical_flip),
               "Affine mat and flip not allowed at the same time");
}

sprite_size second_sprite_attributes::size() const
{
    return sprite_size(_size);
}

void second_sprite_attributes::set_size(sprite_size size)
{
    _size = uint8_t(size);
}

void second_sprite_attributes::set_horizontal_flip(bool horizontal_flip)
{
    BTN_ASSERT(! _affine_mat || ! horizontal_flip, "Affine mat and flip not allowed at the same time");

    _horizontal_flip = horizontal_flip;
}

void second_sprite_attributes::set_vertical_flip(bool vertical_flip)
{
    BTN_ASSERT(! _affine_mat || ! vertical_flip, "Affine mat and flip not allowed at the same time");

    _vertical_flip = vertical_flip;
}

void second_sprite_attributes::set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)
{
    BTN_ASSERT(! affine_mat || (! _horizontal_flip && ! _vertical_flip),
               "Affine mat and flip not allowed at the same time");

    _affine_mat = affine_mat;
}

void second_sprite_attributes::set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)
{
    BTN_ASSERT(! affine_mat || (! _horizontal_flip && ! _vertical_flip),
               "Affine mat and flip not allowed at the same time");

    _affine_mat = move(affine_mat);
}

}
