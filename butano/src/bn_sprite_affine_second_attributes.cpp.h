/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_affine_second_attributes.h"

namespace btn
{

sprite_affine_second_attributes::sprite_affine_second_attributes(fixed x, const sprite_affine_mat_ptr& affine_mat) :
    _x(x),
    _affine_mat(affine_mat)
{
}

sprite_affine_second_attributes::sprite_affine_second_attributes(fixed x, sprite_affine_mat_ptr&& affine_mat) :
    _x(x),
    _affine_mat(move(affine_mat))
{
}

}
