#include "btn_sprite_affine_mat_ptr.h"

#include "btn_utility.h"
#include "btn_sprite_affine_mat_builder.h"
#include "btn_sprite_affine_mats_manager.h"

namespace btn
{

sprite_affine_mat_ptr sprite_affine_mat_ptr::create()
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create(sprite_affine_mat_builder()));
}

sprite_affine_mat_ptr sprite_affine_mat_ptr::create(const sprite_affine_mat_builder& builder)
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create(builder));
}

sprite_affine_mat_ptr::sprite_affine_mat_ptr(const sprite_affine_mat_ptr& other) :
    sprite_affine_mat_ptr(other._id)
{
    sprite_affine_mats_manager::increase_usages(_id);
}

sprite_affine_mat_ptr& sprite_affine_mat_ptr::operator=(const sprite_affine_mat_ptr& other)
{
    if(_id != other._id)
    {
        _destroy();
        _id = other._id;
        sprite_affine_mats_manager::increase_usages(_id);
    }

    return *this;
}

sprite_affine_mat_ptr::sprite_affine_mat_ptr(sprite_affine_mat_ptr&& other) :
    sprite_affine_mat_ptr(other._id)
{
    other._id = -1;
}

sprite_affine_mat_ptr& sprite_affine_mat_ptr::operator=(sprite_affine_mat_ptr&& other)
{
    swap(_id, other._id);
    return *this;
}

fixed sprite_affine_mat_ptr::rotation_angle() const
{
    return sprite_affine_mats_manager::rotation_angle(_id);
}

void sprite_affine_mat_ptr::set_rotation_angle(fixed rotation_angle)
{
    sprite_affine_mats_manager::set_rotation_angle(_id, rotation_angle);
}

fixed sprite_affine_mat_ptr::scale_x() const
{
    return sprite_affine_mats_manager::scale_x(_id);
}

void sprite_affine_mat_ptr::set_scale_x(fixed scale_x)
{
    sprite_affine_mats_manager::set_scale_x(_id, scale_x);
}

fixed sprite_affine_mat_ptr::scale_y() const
{
    return sprite_affine_mats_manager::scale_y(_id);
}

void sprite_affine_mat_ptr::set_scale_y(fixed scale_y)
{
    sprite_affine_mats_manager::set_scale_y(_id, scale_y);
}

bool sprite_affine_mat_ptr::horizontal_flip() const
{
    return sprite_affine_mats_manager::horizontal_flip(_id);
}

void sprite_affine_mat_ptr::set_horizontal_flip(bool horizontal_flip)
{
    sprite_affine_mats_manager::set_horizontal_flip(_id, horizontal_flip);
}

bool sprite_affine_mat_ptr::vertical_flip() const
{
    return sprite_affine_mats_manager::vertical_flip(_id);
}

void sprite_affine_mat_ptr::set_vertical_flip(bool vertical_flip)
{
    sprite_affine_mats_manager::set_vertical_flip(_id, vertical_flip);
}

bool sprite_affine_mat_ptr::is_identity() const
{
    return sprite_affine_mats_manager::is_identity(_id);
}

void sprite_affine_mat_ptr::_destroy()
{
    if(_id >= 0)
    {
        sprite_affine_mats_manager::decrease_usages(_id);
        _id = -1;
    }
}

}
