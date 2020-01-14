#include "../include/btn_hw_sprite_affine_mats.h"

#include "tonc.h"
#include "btn_math.h"
#include "btn_algorithm.h"
#include "btn_sprite_affine_mat_builder.h"

namespace btn::hw::sprite_affine_mats
{

namespace
{
    static_assert(sizeof(OBJ_AFFINE) == sizeof(handle));
    static_assert(alignof(OBJ_AFFINE) == alignof(handle));
}

void item_type::init(const sprite_affine_mat_builder& builder)
{
    set_rotation_angle(builder.rotation_angle());
    set_scale_x(builder.scale_x());
    set_scale_y(builder.scale_y());
    set_horizontal_flip(builder.horizontal_flip());
    set_vertical_flip(builder.vertical_flip());
}

void item_type::set_rotation_angle(fixed rotation_angle)
{
    sin = int16_t(degrees_sin(rotation_angle).value());
    cos = int16_t(degrees_cos(rotation_angle).value());
}

void item_type::set_scale_x(fixed scale_x)
{
    fixed_t inv_scale_x = 1 / scale_x;
    sx = uint16_t(fixed_t<8>(inv_scale_x).value());
}

void item_type::set_scale_y(fixed scale_y)
{
    fixed_t inv_scale_y = 1 / scale_y;
    sy = uint16_t(fixed_t<8>(inv_scale_y).value());
}

void item_type::set_horizontal_flip(bool horizontal_flip)
{
    hflip = 1 - (2 * horizontal_flip);
}

void item_type::set_vertical_flip(bool vertical_flip)
{
    vflip = 1 - (2 * vertical_flip);
}

void setup(const item_type& item, handle& affine_mat)
{
    auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
    int cos = item.cos;
    int sin = item.sin;
    int sx = item.hflip * item.sx;
    int sy = item.vflip * item.sy;
    affine_mat_ptr->pa = int16_t(cos * sx >> 12);
    affine_mat_ptr->pb = int16_t(-sin * sx >> 12);
    affine_mat_ptr->pc = int16_t(sin * sy >> 12);
    affine_mat_ptr->pd = int16_t(cos * sy >> 12);
}

}
