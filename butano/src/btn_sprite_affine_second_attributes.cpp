#include "btn_sprite_affine_second_attributes.h"

namespace btn
{

sprite_affine_second_attributes::sprite_affine_second_attributes(fixed x, sprite_size size,
                                                                 const sprite_affine_mat_ptr& affine_mat) :
    _x(x),
    _size(size),
    _affine_mat(affine_mat)
{
}

sprite_affine_second_attributes::sprite_affine_second_attributes(fixed x, sprite_size size,
                                                                 sprite_affine_mat_ptr&& affine_mat) :
    _x(x),
    _size(size),
    _affine_mat(move(affine_mat))
{
}

}
