#ifndef BTN_SPRITE_AFFINE_MAT_BUILDER_H
#define BTN_SPRITE_AFFINE_MAT_BUILDER_H

#include "btn_fixed.h"
#include "btn_optional_fwd.h"

namespace btn
{

class sprite_affine_mat_ptr;

class sprite_affine_mat_builder
{

public:
    [[nodiscard]] fixed rotation_angle() const
    {
        return _rotation_angle;
    }

    sprite_affine_mat_builder& set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed scale_x() const
    {
        return _scale_x;
    }

    sprite_affine_mat_builder& set_scale_x(fixed scale_x);

    [[nodiscard]] fixed scale_y() const
    {
        return _scale_y;
    }

    sprite_affine_mat_builder& set_scale_y(fixed scale_y);

    sprite_affine_mat_builder& set_scale(fixed scale);

    sprite_affine_mat_builder& set_scale(fixed scale_x, fixed scale_y);

    [[nodiscard]] bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    sprite_affine_mat_builder& set_horizontal_flip(bool horizontal_flip)
    {
        _horizontal_flip = horizontal_flip;
        return *this;
    }

    [[nodiscard]] bool vertical_flip() const
    {
        return _vertical_flip;
    }

    sprite_affine_mat_builder& set_vertical_flip(bool vertical_flip)
    {
        _vertical_flip = vertical_flip;
        return *this;
    }

    [[nodiscard]] sprite_affine_mat_ptr build() const;

    [[nodiscard]] optional<sprite_affine_mat_ptr> optional_build() const;

private:
    fixed _rotation_angle = 0;
    fixed _scale_x = 1;
    fixed _scale_y = 1;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
};

}

#endif

