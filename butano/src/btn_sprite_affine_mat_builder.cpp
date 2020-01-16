#include "btn_sprite_affine_mat_builder.h"

#include "btn_optional.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

sprite_affine_mat_builder& sprite_affine_mat_builder::set_rotation_angle(fixed rotation_angle)
{
    BTN_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle: ", rotation_angle);

    _rotation_angle = rotation_angle;
    return *this;
}

sprite_affine_mat_builder& sprite_affine_mat_builder::set_scale_x(fixed scale_x)
{
    BTN_ASSERT(scale_x > 0, "Invalid scale x: ", scale_x);

    _scale_x = scale_x;
    return *this;
}

sprite_affine_mat_builder& sprite_affine_mat_builder::set_scale_y(fixed scale_y)
{
    BTN_ASSERT(scale_y > 0, "Invalid scale y: ", scale_y);

    _scale_y = scale_y;
    return *this;
}

sprite_affine_mat_ptr sprite_affine_mat_builder::build() const
{
    return sprite_affine_mat_ptr::create(*this);
}

optional<sprite_affine_mat_ptr> sprite_affine_mat_builder::optional_build() const
{
    return sprite_affine_mat_ptr::optional_create(*this);
}

}
