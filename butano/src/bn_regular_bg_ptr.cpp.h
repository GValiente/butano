/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_ptr.h"

#include "bn_size.h"
#include "bn_window.h"
#include "bn_bgs_manager.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_regular_bg_attributes.h"

namespace bn
{

regular_bg_ptr regular_bg_ptr::create(fixed x, fixed y, const regular_bg_item& item)
{
    regular_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(fixed x, fixed y, const regular_bg_item& item, int map_index)
{
    regular_bg_builder builder(item, map_index);
    builder.set_position(fixed_point(x, y));
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const fixed_point& position, const regular_bg_item& item)
{
    regular_bg_builder builder(item);
    builder.set_position(position);
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const fixed_point& position, const regular_bg_item& item, int map_index)
{
    regular_bg_builder builder(item, map_index);
    builder.set_position(position);
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(fixed x, fixed y, regular_bg_map_ptr map)
{
    regular_bg_builder builder(move(map));
    builder.set_position(fixed_point(x, y));
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const fixed_point& position, regular_bg_map_ptr map)
{
    regular_bg_builder builder(move(map));
    builder.set_position(position);
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const regular_bg_builder& builder)
{
    return regular_bg_ptr(bgs_manager::create(regular_bg_builder(builder)));
}

regular_bg_ptr regular_bg_ptr::create(regular_bg_builder&& builder)
{
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(fixed x, fixed y, const regular_bg_item& item)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(
        fixed x, fixed y, const regular_bg_item& item, int map_index)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item, map_index);
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(const fixed_point& position, const regular_bg_item& item)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item);
    builder.set_position(position);

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(
        const fixed_point& position, const regular_bg_item& item, int map_index)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item, map_index);
    builder.set_position(position);

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(fixed x, fixed y, regular_bg_map_ptr map)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(move(map));
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(const fixed_point& position, regular_bg_map_ptr map)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(move(map));
    builder.set_position(position);

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(const regular_bg_builder& builder)
{
    optional<regular_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(regular_bg_builder(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::create_optional(regular_bg_builder&& builder)
{
    optional<regular_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

regular_bg_ptr::regular_bg_ptr(const regular_bg_ptr& other) :
    regular_bg_ptr(other._handle)
{
    bgs_manager::increase_usages(_handle);
}

regular_bg_ptr& regular_bg_ptr::operator=(const regular_bg_ptr& other)
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

regular_bg_ptr::~regular_bg_ptr()
{
    if(_handle)
    {
        bgs_manager::decrease_usages(_handle);
    }
}

size regular_bg_ptr::dimensions() const
{
    return bgs_manager::dimensions(_handle);
}

bool regular_bg_ptr::big() const
{
    return bgs_manager::big(_handle);
}

const regular_bg_tiles_ptr& regular_bg_ptr::tiles() const
{
    return bgs_manager::regular_map(_handle).tiles();
}

void regular_bg_ptr::set_tiles(const regular_bg_tiles_ptr& tiles)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_tiles(tiles);
}

void regular_bg_ptr::set_tiles(regular_bg_tiles_ptr&& tiles)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_tiles(move(tiles));
}

void regular_bg_ptr::set_tiles(const regular_bg_tiles_item& tiles_item)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_tiles(tiles_item);
}

const bg_palette_ptr& regular_bg_ptr::palette() const
{
    return bgs_manager::regular_map(_handle).palette();
}

void regular_bg_ptr::set_palette(const bg_palette_ptr& palette)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_palette(palette);
}

void regular_bg_ptr::set_palette(bg_palette_ptr&& palette)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_palette(move(palette));
}

void regular_bg_ptr::set_palette(const bg_palette_item& palette_item)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_palette(palette_item);
}

void regular_bg_ptr::set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_tiles_and_palette(move(tiles), move(palette));
}

void regular_bg_ptr::set_tiles_and_palette(const regular_bg_tiles_item& tiles_item,
                                           const bg_palette_item& palette_item)
{
    regular_bg_map_ptr map = bgs_manager::regular_map(_handle);
    map.set_tiles_and_palette(tiles_item, palette_item);
}

const regular_bg_map_ptr& regular_bg_ptr::map() const
{
    return bgs_manager::regular_map(_handle);
}

void regular_bg_ptr::set_map(const regular_bg_map_ptr& map)
{
    bgs_manager::set_regular_map(_handle, map);
}

void regular_bg_ptr::set_map(regular_bg_map_ptr&& map)
{
    bgs_manager::set_regular_map(_handle, move(map));
}

void regular_bg_ptr::set_map(const regular_bg_map_item& map_item)
{
    const regular_bg_tiles_ptr& current_tiles = tiles();
    const bg_palette_ptr& current_palette = palette();
    optional<regular_bg_map_ptr> map = map_item.find_map(current_tiles, current_palette);

    if(regular_bg_map_ptr* map_ptr = map.get())
    {
        bgs_manager::set_regular_map(_handle, move(*map_ptr));
    }
    else
    {
        regular_bg_tiles_ptr tiles_copy(current_tiles);
        bg_palette_ptr palette_copy(current_palette);
        bgs_manager::remove_regular_map(_handle);
        bgs_manager::set_regular_map(_handle, map_item.create_new_map(move(tiles_copy), move(palette_copy)));
    }
}

void regular_bg_ptr::set_map(const regular_bg_map_item& map_item, int map_index)
{
    const regular_bg_tiles_ptr& current_tiles = tiles();
    const bg_palette_ptr& current_palette = palette();
    optional<regular_bg_map_ptr> map = map_item.find_map(current_tiles, current_palette, map_index);

    if(regular_bg_map_ptr* map_ptr = map.get())
    {
        bgs_manager::set_regular_map(_handle, move(*map_ptr));
    }
    else
    {
        regular_bg_tiles_ptr tiles_copy(current_tiles);
        bg_palette_ptr palette_copy(current_palette);
        bgs_manager::remove_regular_map(_handle);
        bgs_manager::set_regular_map(
                    _handle, map_item.create_new_map(move(tiles_copy), move(palette_copy), map_index));
    }
}

void regular_bg_ptr::set_item(const regular_bg_item& item)
{
    const regular_bg_map_item& map_item = item.map_item();
    optional<regular_bg_map_ptr> map = map_item.find_map(tiles(), palette());

    if(regular_bg_map_ptr* map_ptr = map.get())
    {
        map_ptr->set_tiles_and_palette(item.tiles_item(), item.palette_item());
        bgs_manager::set_regular_map(_handle, move(*map_ptr));
    }
    else
    {
        optional<regular_bg_tiles_ptr> tiles = item.tiles_item().find_tiles();
        bgs_manager::remove_regular_map(_handle);

        if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
        {
            bgs_manager::set_regular_map(
                        _handle, map_item.create_new_map(move(*tiles_ptr), item.palette_item().create_palette()));
        }
        else
        {
            bgs_manager::set_regular_map(_handle, item.create_new_map());
        }
    }
}

void regular_bg_ptr::set_item(const regular_bg_item& item, int map_index)
{
    const regular_bg_map_item& map_item = item.map_item();
    optional<regular_bg_map_ptr> map = map_item.find_map(tiles(), palette(), map_index);

    if(regular_bg_map_ptr* map_ptr = map.get())
    {
        map_ptr->set_tiles_and_palette(item.tiles_item(), item.palette_item());
        bgs_manager::set_regular_map(_handle, move(*map_ptr));
    }
    else
    {
        optional<regular_bg_tiles_ptr> tiles = item.tiles_item().find_tiles();
        bgs_manager::remove_regular_map(_handle);

        if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
        {
            bgs_manager::set_regular_map(_handle, map_item.create_new_map(
                                             move(*tiles_ptr), item.palette_item().create_palette(), map_index));
        }
        else
        {
            bgs_manager::set_regular_map(_handle, item.create_new_map(map_index));
        }
    }
}

fixed regular_bg_ptr::x() const
{
    return position().x();
}

void regular_bg_ptr::set_x(fixed x)
{
    bgs_manager::set_regular_x(_handle, x);
}

fixed regular_bg_ptr::y() const
{
    return position().y();
}

void regular_bg_ptr::set_y(fixed y)
{
    bgs_manager::set_regular_y(_handle, y);
}

const fixed_point& regular_bg_ptr::position() const
{
    return bgs_manager::position(_handle);
}

void regular_bg_ptr::set_position(fixed x, fixed y)
{
    bgs_manager::set_regular_position(_handle, fixed_point(x, y));
}

void regular_bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_regular_position(_handle, position);
}

int regular_bg_ptr::priority() const
{
    return bgs_manager::priority(_handle);
}

void regular_bg_ptr::set_priority(int priority)
{
    bgs_manager::set_priority(_handle, priority);
}

int regular_bg_ptr::z_order() const
{
    return bgs_manager::z_order(_handle);
}

void regular_bg_ptr::set_z_order(int z_order)
{
    bgs_manager::set_z_order(_handle, z_order);
}

void regular_bg_ptr::put_above()
{
    bgs_manager::put_above(_handle);
}

void regular_bg_ptr::put_below()
{
    bgs_manager::put_below(_handle);
}

bool regular_bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_handle);
}

void regular_bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool regular_bg_ptr::blending_top_enabled() const
{
    return bgs_manager::blending_top_enabled(_handle);
}

void regular_bg_ptr::set_blending_top_enabled(bool blending_top_enabled)
{
    bgs_manager::set_blending_top_enabled(_handle, blending_top_enabled);
}

bool regular_bg_ptr::blending_bottom_enabled() const
{
    return bgs_manager::blending_bottom_enabled(_handle);
}

void regular_bg_ptr::set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    bgs_manager::set_blending_bottom_enabled(_handle, blending_bottom_enabled);
}

bool regular_bg_ptr::visible() const
{
    return bgs_manager::visible(_handle);
}

void regular_bg_ptr::set_visible(bool visible)
{
    bgs_manager::set_visible(_handle, visible);
}

bool regular_bg_ptr::visible_in_window(const window& window) const
{
    return window.show_bg(*this);
}

void regular_bg_ptr::set_visible_in_window(bool visible, window& window)
{
    window.set_show_bg(*this, visible);
}

const optional<camera_ptr>& regular_bg_ptr::camera() const
{
    return bgs_manager::camera(_handle);
}

void regular_bg_ptr::set_camera(const camera_ptr& camera)
{
    bgs_manager::set_camera(_handle, camera_ptr(camera));
}

void regular_bg_ptr::set_camera(camera_ptr&& camera)
{
    bgs_manager::set_camera(_handle, move(camera));
}

void regular_bg_ptr::set_camera(const optional<camera_ptr>& camera)
{
    if(const camera_ptr* camera_ref = camera.get())
    {
        bgs_manager::set_camera(_handle, camera_ptr(*camera_ref));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void regular_bg_ptr::set_camera(optional<camera_ptr>&& camera)
{
    if(camera_ptr* camera_ref = camera.get())
    {
        bgs_manager::set_camera(_handle, move(*camera_ref));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void regular_bg_ptr::remove_camera()
{
    bgs_manager::remove_camera(_handle);
}

regular_bg_attributes regular_bg_ptr::attributes() const
{
    return bgs_manager::regular_attributes(_handle);
}

void regular_bg_ptr::set_attributes(const regular_bg_attributes& attributes)
{
    bgs_manager::set_regular_attributes(_handle, attributes);
}

optional<int> regular_bg_ptr::hw_id() const
{
    bgs_manager::rebuild_handles();

    optional<int> result;
    int id = bgs_manager::hw_id(_handle);

    if(id >= 0)
    {
        result = id;
    }

    return result;
}

}
