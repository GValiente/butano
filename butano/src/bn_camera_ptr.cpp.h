/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_camera_ptr.h"

#include "bn_cameras_manager.h"

namespace bn
{

camera_ptr camera_ptr::create()
{
    int id = cameras_manager::create(fixed_point());
    return camera_ptr(id);
}

camera_ptr camera_ptr::create(fixed x, fixed y)
{
    int id = cameras_manager::create(fixed_point(x, y));
    return camera_ptr(id);
}

camera_ptr camera_ptr::create(const fixed_point& position)
{
    int id = cameras_manager::create(position);
    return camera_ptr(id);
}

optional<camera_ptr> camera_ptr::create_optional()
{
    int id = cameras_manager::create_optional(fixed_point());
    optional<camera_ptr> result;

    if(id >= 0)
    {
        result = camera_ptr(id);
    }

    return result;
}

optional<camera_ptr> camera_ptr::create_optional(fixed x, fixed y)
{
    int id = cameras_manager::create_optional(fixed_point(x, y));
    optional<camera_ptr> result;

    if(id >= 0)
    {
        result = camera_ptr(id);
    }

    return result;
}

optional<camera_ptr> camera_ptr::create_optional(const fixed_point& position)
{
    int id = cameras_manager::create_optional(position);
    optional<camera_ptr> result;

    if(id >= 0)
    {
        result = camera_ptr(id);
    }

    return result;
}

camera_ptr::camera_ptr(const camera_ptr& other) :
    camera_ptr(other._id)
{
    cameras_manager::increase_usages(_id);
}

camera_ptr& camera_ptr::operator=(const camera_ptr& other)
{
    if(_id != other._id)
    {
        if(_id >= 0)
        {
            cameras_manager::decrease_usages(_id);
        }

        _id = other._id;
        cameras_manager::increase_usages(_id);
    }

    return *this;
}

camera_ptr::~camera_ptr()
{
    if(_id >= 0)
    {
        cameras_manager::decrease_usages(_id);
    }
}

fixed camera_ptr::x() const
{
    return position().x();
}

void camera_ptr::set_x(fixed x)
{
    cameras_manager::set_x(_id, x);
}

fixed camera_ptr::y() const
{
    return position().y();
}

void camera_ptr::set_y(fixed y)
{
    cameras_manager::set_y(_id, y);
}

const fixed_point& camera_ptr::position() const
{
    return cameras_manager::position(_id);
}

void camera_ptr::set_position(fixed x, fixed y)
{
    cameras_manager::set_position(_id, fixed_point(x, y));
}

void camera_ptr::set_position(const fixed_point& position)
{
    cameras_manager::set_position(_id, position);
}

}
