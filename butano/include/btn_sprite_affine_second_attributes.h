/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_H
#define BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_H

#include "btn_fixed.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

class sprite_affine_second_attributes
{

public:
    sprite_affine_second_attributes(fixed x, const sprite_affine_mat_ptr& affine_mat);

    sprite_affine_second_attributes(fixed x, sprite_affine_mat_ptr&& affine_mat);

    [[nodiscard]] fixed x() const
    {
        return _x;
    }

    void set_x(fixed x)
    {
        _x = x;
    }

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat;
    }

    void set_affine_mat(const sprite_affine_mat_ptr& affine_mat)
    {
        _affine_mat = affine_mat;
    }

    void set_affine_mat(sprite_affine_mat_ptr&& affine_mat)
    {
        _affine_mat = move(affine_mat);
    }

    [[nodiscard]] friend bool operator==(const sprite_affine_second_attributes& a,
                                         const sprite_affine_second_attributes& b) = default;

private:
    fixed _x;
    sprite_affine_mat_ptr _affine_mat;
};

}

#endif

