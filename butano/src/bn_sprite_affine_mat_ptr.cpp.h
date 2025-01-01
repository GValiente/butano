/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mat_ptr.h"

#include "bn_math.h"
#include "bn_sprite_affine_mats_manager.h"

namespace bn
{

sprite_affine_mat_ptr sprite_affine_mat_ptr::create()
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create());
}

sprite_affine_mat_ptr sprite_affine_mat_ptr::create(const affine_mat_attributes& attributes)
{
    return sprite_affine_mat_ptr(sprite_affine_mats_manager::create(attributes));
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

optional<sprite_affine_mat_ptr> sprite_affine_mat_ptr::create_optional(const affine_mat_attributes& attributes)
{
    int id = sprite_affine_mats_manager::create_optional(attributes);
    optional<sprite_affine_mat_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_ptr(id);
    }

    return result;
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

sprite_affine_mat_ptr::~sprite_affine_mat_ptr()
{
    if(_id >= 0)
    {
        sprite_affine_mats_manager::decrease_usages(_id);
    }
}

fixed sprite_affine_mat_ptr::rotation_angle() const
{
    return sprite_affine_mats_manager::rotation_angle(_id);
}

void sprite_affine_mat_ptr::set_rotation_angle(fixed rotation_angle)
{
    sprite_affine_mats_manager::set_rotation_angle(_id, rotation_angle);
}

void sprite_affine_mat_ptr::set_rotation_angle_safe(fixed rotation_angle)
{
    sprite_affine_mats_manager::set_rotation_angle(_id, safe_degrees_angle(rotation_angle));
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

fixed sprite_affine_mat_ptr::horizontal_shear() const
{
    return sprite_affine_mats_manager::horizontal_shear(_id);
}

void sprite_affine_mat_ptr::set_horizontal_shear(fixed horizontal_shear)
{
    sprite_affine_mats_manager::set_horizontal_shear(_id, horizontal_shear);
}

fixed sprite_affine_mat_ptr::vertical_shear() const
{
    return sprite_affine_mats_manager::vertical_shear(_id);
}

void sprite_affine_mat_ptr::set_vertical_shear(fixed vertical_shear)
{
    sprite_affine_mats_manager::set_vertical_shear(_id, vertical_shear);
}

void sprite_affine_mat_ptr::set_shear(fixed shear)
{
    sprite_affine_mats_manager::set_shear(_id, shear);
}

void sprite_affine_mat_ptr::set_shear(fixed horizontal_shear, fixed vertical_shear)
{
    sprite_affine_mats_manager::set_shear(_id, horizontal_shear, vertical_shear);
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

const affine_mat_attributes& sprite_affine_mat_ptr::attributes() const
{
    return sprite_affine_mats_manager::attributes(_id);
}

void sprite_affine_mat_ptr::set_attributes(const affine_mat_attributes& attributes)
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

}
