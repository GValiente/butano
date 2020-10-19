#include "btn_sprite_affine_mat_ptr.h"

#include "btn_optional.h"
#include "btn_sprite_affine_mat_builder.h"
#include "btn_sprite_affine_mats_manager.h"

namespace btn
{

sprite_affine_mat_ptr sprite_affine_mat_ptr::create()
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create());
}

sprite_affine_mat_ptr sprite_affine_mat_ptr::create(const sprite_affine_mat_attributes& attributes)
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create(attributes));
}

sprite_affine_mat_ptr sprite_affine_mat_ptr::create(const sprite_affine_mat_builder& builder)
{
    return create(builder.attributes());
}

optional<sprite_affine_mat_ptr> sprite_affine_mat_ptr::create_optional()
{
    int id = sprite_affine_mats_manager::create_optional();
    optional<sprite_affine_mat_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_ptr(id);
    }

    return result;
}

optional<sprite_affine_mat_ptr> sprite_affine_mat_ptr::create_optional(const sprite_affine_mat_attributes& attributes)
{
    int id = sprite_affine_mats_manager::create_optional(attributes);
    optional<sprite_affine_mat_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_ptr(id);
    }

    return result;
}

optional<sprite_affine_mat_ptr> sprite_affine_mat_ptr::create_optional(const sprite_affine_mat_builder& builder)
{
    return create_optional(builder.attributes());
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
        if(_id >= 0)
        {
            sprite_affine_mats_manager::decrease_usages(_id);
        }

        _id = other._id;
        sprite_affine_mats_manager::increase_usages(_id);
    }

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

fixed sprite_affine_mat_ptr::horizontal_scale() const
{
    return sprite_affine_mats_manager::horizontal_scale(_id);
}

void sprite_affine_mat_ptr::set_horizontal_scale(fixed horizontal_scale)
{
    sprite_affine_mats_manager::set_horizontal_scale(_id, horizontal_scale);
}

fixed sprite_affine_mat_ptr::vertical_scale() const
{
    return sprite_affine_mats_manager::vertical_scale(_id);
}

void sprite_affine_mat_ptr::set_vertical_scale(fixed vertical_scale)
{
    sprite_affine_mats_manager::set_vertical_scale(_id, vertical_scale);
}

void sprite_affine_mat_ptr::set_scale(fixed scale)
{
    sprite_affine_mats_manager::set_scale(_id, scale);
}

void sprite_affine_mat_ptr::set_scale(fixed horizontal_scale, fixed vertical_scale)
{
    sprite_affine_mats_manager::set_scale(_id, horizontal_scale, vertical_scale);
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

const sprite_affine_mat_attributes& sprite_affine_mat_ptr::attributes() const
{
    return sprite_affine_mats_manager::attributes(_id);
}

void sprite_affine_mat_ptr::set_attributes(const sprite_affine_mat_attributes& attributes)
{
    sprite_affine_mats_manager::set_attributes(_id, attributes);
}

bool sprite_affine_mat_ptr::identity() const
{
    return sprite_affine_mats_manager::identity(_id);
}

bool sprite_affine_mat_ptr::flipped_identity() const
{
    return sprite_affine_mats_manager::flipped_identity(_id);
}

void sprite_affine_mat_ptr::_destroy()
{
    sprite_affine_mats_manager::decrease_usages(_id);
}

}
