/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_ptr.h"

#include "bn_size.h"
#include "bn_window.h"
#include "bn_bgs_manager.h"
#include "bn_bg_palette_ptr.h"
#include "bn_affine_bg_builder.h"
#include "bn_affine_bg_tiles_ptr.h"
#include "bn_affine_bg_attributes.h"

namespace bn
{

affine_bg_ptr affine_bg_ptr::create(fixed x, fixed y, const affine_bg_item& item)
{
    affine_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(const fixed_point& position, const affine_bg_item& item)
{
    affine_bg_builder builder(item);
    builder.set_position(position);
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(const affine_bg_builder& builder)
{
    return affine_bg_ptr(bgs_manager::create(affine_bg_builder(builder)));
}

affine_bg_ptr affine_bg_ptr::create(affine_bg_builder&& builder)
{
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(fixed x, fixed y, const affine_bg_item& item)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(const fixed_point& position, const affine_bg_item& item)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(item);
    builder.set_position(position);

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(const affine_bg_builder& builder)
{
    optional<affine_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(affine_bg_builder(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(affine_bg_builder&& builder)
{
    optional<affine_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

affine_bg_ptr::affine_bg_ptr(const affine_bg_ptr& other) :
    affine_bg_ptr(other._handle)
{
    bgs_manager::increase_usages(_handle);
}

affine_bg_ptr& affine_bg_ptr::operator=(const affine_bg_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle)
        {
            bgs_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        bgs_manager::increase_usages(_handle);
    }

    return *this;
}

size affine_bg_ptr::dimensions() const
{
    return bgs_manager::dimensions(_handle);
}

const affine_bg_tiles_ptr& affine_bg_ptr::tiles() const
{
    return bgs_manager::map(_handle).tiles();
}

void affine_bg_ptr::set_tiles(const affine_bg_tiles_ptr& tiles)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles(tiles);
}

void affine_bg_ptr::set_tiles(affine_bg_tiles_ptr&& tiles)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles(move(tiles));
}

void affine_bg_ptr::set_tiles(const affine_bg_tiles_item& tiles_item)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles(tiles_item);
}

const bg_palette_ptr& affine_bg_ptr::palette() const
{
    return bgs_manager::map(_handle).palette();
}

void affine_bg_ptr::set_palette(const bg_palette_ptr& palette)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_palette(palette);
}

void affine_bg_ptr::set_palette(bg_palette_ptr&& palette)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_palette(move(palette));
}

void affine_bg_ptr::set_palette(const bg_palette_item& palette_item)
{
    affine_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_palette(palette_item);
}

const affine_bg_map_ptr& affine_bg_ptr::map() const
{
    return bgs_manager::map(_handle);
}

void affine_bg_ptr::set_map(const affine_bg_map_ptr& map)
{
    bgs_manager::set_map(_handle, map);
}

void affine_bg_ptr::set_map(affine_bg_map_ptr&& map)
{
    bgs_manager::set_map(_handle, move(map));
}

void affine_bg_ptr::set_map(const affine_bg_map_item& map_item)
{
    const affine_bg_tiles_ptr& current_tiles = tiles();
    const bg_palette_ptr& current_palette = palette();

    if(optional<affine_bg_map_ptr> map = map_item.find_map(current_tiles, current_palette))
    {
        bgs_manager::set_map(_handle, move(*map));
    }
    else
    {
        affine_bg_tiles_ptr tiles_copy(current_tiles);
        bg_palette_ptr palette_copy(current_palette);
        bgs_manager::remove_map(_handle);
        bgs_manager::set_map(_handle, map_item.create_new_map(move(tiles_copy), move(palette_copy)));
    }
}

void affine_bg_ptr::set_item(const affine_bg_item& item)
{
    if(optional<affine_bg_map_ptr> map = item.map_item().find_map(tiles(), palette()))
    {
        bgs_manager::set_map(_handle, move(*map));
    }
    else
    {
        bgs_manager::remove_map(_handle);
        bgs_manager::set_map(_handle, item.create_new_map());
    }
}

fixed affine_bg_ptr::x() const
{
    return position().x();
}

void affine_bg_ptr::set_x(fixed x)
{
    bgs_manager::set_x(_handle, x);
}

fixed affine_bg_ptr::y() const
{
    return position().y();
}

void affine_bg_ptr::set_y(fixed y)
{
    bgs_manager::set_y(_handle, y);
}

const fixed_point& affine_bg_ptr::position() const
{
    return bgs_manager::position(_handle);
}

void affine_bg_ptr::set_position(fixed x, fixed y)
{
    bgs_manager::set_position(_handle, fixed_point(x, y));
}

void affine_bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_position(_handle, position);
}

int affine_bg_ptr::priority() const
{
    return bgs_manager::priority(_handle);
}

void affine_bg_ptr::set_priority(int priority)
{
    bgs_manager::set_priority(_handle, priority);
}

int affine_bg_ptr::z_order() const
{
    return bgs_manager::z_order(_handle);
}

void affine_bg_ptr::set_z_order(int z_order)
{
    bgs_manager::set_z_order(_handle, z_order);
}

void affine_bg_ptr::put_above()
{
    bgs_manager::put_above(_handle);
}

bool affine_bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_handle);
}

void affine_bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool affine_bg_ptr::blending_enabled() const
{
    return bgs_manager::blending_enabled(_handle);
}

void affine_bg_ptr::set_blending_enabled(bool blending_enabled)
{
    bgs_manager::set_blending_enabled(_handle, blending_enabled);
}

bool affine_bg_ptr::visible() const
{
    return bgs_manager::visible(_handle);
}

void affine_bg_ptr::set_visible(bool visible)
{
    bgs_manager::set_visible(_handle, visible);
}

bool affine_bg_ptr::visible_in_window(const window& window) const
{
    return window.show_bg(*this);
}

void affine_bg_ptr::set_visible_in_window(bool visible, window& window)
{
    window.set_show_bg(*this, visible);
}

const optional<camera_ptr>& affine_bg_ptr::camera() const
{
    return bgs_manager::camera(_handle);
}

void affine_bg_ptr::set_camera(const camera_ptr& camera)
{
    bgs_manager::set_camera(_handle, camera_ptr(camera));
}

void affine_bg_ptr::set_camera(camera_ptr&& camera)
{
    bgs_manager::set_camera(_handle, move(camera));
}

void affine_bg_ptr::set_camera(const optional<camera_ptr>& camera)
{
    if(camera)
    {
        bgs_manager::set_camera(_handle, camera_ptr(*camera));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void affine_bg_ptr::set_camera(optional<camera_ptr>&& camera)
{
    if(camera)
    {
        bgs_manager::set_camera(_handle, move(*camera));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void affine_bg_ptr::remove_camera()
{
    bgs_manager::remove_camera(_handle);
}

affine_bg_attributes affine_bg_ptr::attributes() const
{
    return bgs_manager::affine_attributes(_handle);
}

void affine_bg_ptr::set_attributes(const affine_bg_attributes& attributes)
{
    bgs_manager::set_affine_attributes(_handle, attributes);
}

void affine_bg_ptr::_destroy()
{
    bgs_manager::decrease_usages(_handle);
}

}
