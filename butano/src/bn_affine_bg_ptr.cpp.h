/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_ptr.h"

#include "bn_window.h"
#include "bn_bgs_manager.h"
#include "bn_bg_palette_ptr.h"
#include "bn_top_left_utils.h"
#include "bn_affine_bg_builder.h"
#include "bn_affine_bg_tiles_ptr.h"
#include "bn_affine_bg_attributes.h"

namespace bn
{

affine_bg_ptr affine_bg_ptr::create(const affine_bg_item& item)
{
    return affine_bg_ptr(bgs_manager::create(affine_bg_builder(item)));
}

affine_bg_ptr affine_bg_ptr::create(const affine_bg_item& item, int map_index)
{
    return affine_bg_ptr(bgs_manager::create(affine_bg_builder(item, map_index)));
}

affine_bg_ptr affine_bg_ptr::create(affine_bg_map_ptr map)
{
    return affine_bg_ptr(bgs_manager::create(affine_bg_builder(move(map))));
}

affine_bg_ptr affine_bg_ptr::create(fixed x, fixed y, const affine_bg_item& item)
{
    affine_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(fixed x, fixed y, const affine_bg_item& item, int map_index)
{
    affine_bg_builder builder(item, map_index);
    builder.set_position(fixed_point(x, y));
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(const fixed_point& position, const affine_bg_item& item)
{
    affine_bg_builder builder(item);
    builder.set_position(position);
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(const fixed_point& position, const affine_bg_item& item, int map_index)
{
    affine_bg_builder builder(item, map_index);
    builder.set_position(position);
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(fixed x, fixed y, affine_bg_map_ptr map)
{
    affine_bg_builder builder(move(map));
    builder.set_position(fixed_point(x, y));
    return affine_bg_ptr(bgs_manager::create(move(builder)));
}

affine_bg_ptr affine_bg_ptr::create(const fixed_point& position, affine_bg_map_ptr map)
{
    affine_bg_builder builder(move(map));
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

optional<affine_bg_ptr> affine_bg_ptr::create_optional(const affine_bg_item& item)
{
    optional<affine_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(affine_bg_builder(item)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(const affine_bg_item& item, int map_index)
{
    optional<affine_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(affine_bg_builder(item, map_index)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(affine_bg_map_ptr map)
{
    optional<affine_bg_ptr> result;

    if(handle_type handle = bgs_manager::create_optional(affine_bg_builder(move(map))))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
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

optional<affine_bg_ptr> affine_bg_ptr::create_optional(fixed x, fixed y, const affine_bg_item& item, int map_index)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(item, map_index);
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

optional<affine_bg_ptr> affine_bg_ptr::create_optional(
        const fixed_point& position, const affine_bg_item& item, int map_index)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(item, map_index);
    builder.set_position(position);

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(fixed x, fixed y, affine_bg_map_ptr map)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(move(map));
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::create_optional(move(builder)))
    {
        result = affine_bg_ptr(handle);
    }

    return result;
}

optional<affine_bg_ptr> affine_bg_ptr::create_optional(const fixed_point& position, affine_bg_map_ptr map)
{
    optional<affine_bg_ptr> result;
    affine_bg_builder builder(move(map));
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

affine_bg_ptr::~affine_bg_ptr()
{
    if(_handle)
    {
        bgs_manager::decrease_usages(_handle);
    }
}

size affine_bg_ptr::dimensions() const
{
    return bgs_manager::dimensions(_handle);
}

bool affine_bg_ptr::big() const
{
    return bgs_manager::big(_handle);
}

const affine_bg_tiles_ptr& affine_bg_ptr::tiles() const
{
    return bgs_manager::affine_map(_handle).tiles();
}

void affine_bg_ptr::set_tiles(const affine_bg_tiles_ptr& tiles)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_tiles(tiles);
}

void affine_bg_ptr::set_tiles(affine_bg_tiles_ptr&& tiles)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_tiles(move(tiles));
}

void affine_bg_ptr::set_tiles(const affine_bg_tiles_item& tiles_item)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_tiles(tiles_item);
}

const bg_palette_ptr& affine_bg_ptr::palette() const
{
    return bgs_manager::affine_map(_handle).palette();
}

void affine_bg_ptr::set_palette(const bg_palette_ptr& palette)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_palette(palette);
}

void affine_bg_ptr::set_palette(bg_palette_ptr&& palette)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_palette(move(palette));
}

void affine_bg_ptr::set_palette(const bg_palette_item& palette_item)
{
    affine_bg_map_ptr map = bgs_manager::affine_map(_handle);
    map.set_palette(palette_item);
}

const affine_bg_map_ptr& affine_bg_ptr::map() const
{
    return bgs_manager::affine_map(_handle);
}

void affine_bg_ptr::set_map(const affine_bg_map_ptr& map)
{
    bgs_manager::set_affine_map(_handle, map);
}

void affine_bg_ptr::set_map(affine_bg_map_ptr&& map)
{
    bgs_manager::set_affine_map(_handle, move(map));
}

void affine_bg_ptr::set_map(const affine_bg_map_item& map_item)
{
    const affine_bg_tiles_ptr& current_tiles = tiles();
    const bg_palette_ptr& current_palette = palette();
    optional<affine_bg_map_ptr> map = map_item.find_map(current_tiles, current_palette);

    if(affine_bg_map_ptr* map_ptr = map.get())
    {
        bgs_manager::set_affine_map(_handle, move(*map_ptr));
    }
    else
    {
        affine_bg_tiles_ptr tiles_copy(current_tiles);
        bg_palette_ptr palette_copy(current_palette);
        bgs_manager::remove_affine_map(_handle);
        bgs_manager::set_affine_map(
                _handle, affine_bg_map_ptr::create(map_item, move(tiles_copy), move(palette_copy)));
    }
}

void affine_bg_ptr::set_map(const affine_bg_map_item& map_item, int map_index)
{
    const affine_bg_tiles_ptr& current_tiles = tiles();
    const bg_palette_ptr& current_palette = palette();
    optional<affine_bg_map_ptr> map = map_item.find_map(current_tiles, current_palette, map_index);

    if(affine_bg_map_ptr* map_ptr = map.get())
    {
        bgs_manager::set_affine_map(_handle, move(*map_ptr));
    }
    else
    {
        affine_bg_tiles_ptr tiles_copy(current_tiles);
        bg_palette_ptr palette_copy(current_palette);
        bgs_manager::remove_affine_map(_handle);
        bgs_manager::set_affine_map(
                _handle, affine_bg_map_ptr::create(map_item, move(tiles_copy), move(palette_copy), map_index));
    }
}

void affine_bg_ptr::set_item(const affine_bg_item& item)
{
    const affine_bg_map_item& map_item = item.map_item();
    optional<affine_bg_map_ptr> map = map_item.find_map(tiles(), palette());

    if(affine_bg_map_ptr* map_ptr = map.get())
    {
        map_ptr->set_tiles(item.tiles_item());
        map_ptr->set_palette(item.palette_item());
        bgs_manager::set_affine_map(_handle, move(*map_ptr));
    }
    else
    {
        optional<affine_bg_tiles_ptr> tiles = item.tiles_item().find_tiles();
        bgs_manager::remove_affine_map(_handle);

        if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
        {
            bgs_manager::set_affine_map(
                    _handle, affine_bg_map_ptr::create(
                            map_item, move(*tiles_ptr), bg_palette_ptr::create(item.palette_item())));
        }
        else
        {
            bgs_manager::set_affine_map(_handle, affine_bg_map_ptr::create(item));
        }
    }
}

void affine_bg_ptr::set_item(const affine_bg_item& item, int map_index)
{
    const affine_bg_map_item& map_item = item.map_item();
    optional<affine_bg_map_ptr> map = map_item.find_map(tiles(), palette(), map_index);

    if(affine_bg_map_ptr* map_ptr = map.get())
    {
        map_ptr->set_tiles(item.tiles_item());
        map_ptr->set_palette(item.palette_item());
        bgs_manager::set_affine_map(_handle, move(*map_ptr));
    }
    else
    {
        optional<affine_bg_tiles_ptr> tiles = item.tiles_item().find_tiles();
        bgs_manager::remove_affine_map(_handle);

        if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
        {
            bgs_manager::set_affine_map(
                    _handle, affine_bg_map_ptr::create(
                            map_item, move(*tiles_ptr), bg_palette_ptr::create(item.palette_item()), map_index));
        }
        else
        {
            bgs_manager::set_affine_map(_handle, affine_bg_map_ptr::create(item, map_index));
        }
    }
}

fixed affine_bg_ptr::x() const
{
    return position().x();
}

void affine_bg_ptr::set_x(fixed x)
{
    bgs_manager::set_affine_x(_handle, x);
}

fixed affine_bg_ptr::y() const
{
    return position().y();
}

void affine_bg_ptr::set_y(fixed y)
{
    bgs_manager::set_affine_y(_handle, y);
}

const fixed_point& affine_bg_ptr::position() const
{
    return bgs_manager::position(_handle);
}

void affine_bg_ptr::set_position(fixed x, fixed y)
{
    bgs_manager::set_affine_position(_handle, fixed_point(x, y));
}

void affine_bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_affine_position(_handle, position);
}

fixed affine_bg_ptr::top_left_x() const
{
    return to_top_left_x(position().x(), dimensions().width());
}

void affine_bg_ptr::set_top_left_x(fixed top_left_x)
{
    set_x(from_top_left_x(top_left_x, dimensions().width()));
}

fixed affine_bg_ptr::top_left_y() const
{
    return to_top_left_y(position().y(), dimensions().height());
}

void affine_bg_ptr::set_top_left_y(fixed top_left_y)
{
    set_y(from_top_left_y(top_left_y, dimensions().height()));
}

fixed_point affine_bg_ptr::top_left_position() const
{
    return to_top_left_position(position(), dimensions());
}

void affine_bg_ptr::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), dimensions()));
}

void affine_bg_ptr::set_top_left_position(const fixed_point& top_left_position)
{
    set_position(from_top_left_position(top_left_position, dimensions()));
}

fixed affine_bg_ptr::rotation_angle() const
{
    return bgs_manager::rotation_angle(_handle);
}

void affine_bg_ptr::set_rotation_angle(fixed rotation_angle)
{
    bgs_manager::set_rotation_angle(_handle, rotation_angle);
}

void affine_bg_ptr::set_rotation_angle_safe(fixed rotation_angle)
{
    bgs_manager::set_rotation_angle(_handle, safe_degrees_angle(rotation_angle));
}

fixed affine_bg_ptr::horizontal_scale() const
{
    return bgs_manager::horizontal_scale(_handle);
}

void affine_bg_ptr::set_horizontal_scale(fixed horizontal_scale)
{
    bgs_manager::set_horizontal_scale(_handle, horizontal_scale);
}

fixed affine_bg_ptr::vertical_scale() const
{
    return bgs_manager::vertical_scale(_handle);
}

void affine_bg_ptr::set_vertical_scale(fixed vertical_scale)
{
    bgs_manager::set_vertical_scale(_handle, vertical_scale);
}

void affine_bg_ptr::set_scale(fixed scale)
{
    bgs_manager::set_scale(_handle, scale);
}

void affine_bg_ptr::set_scale(fixed horizontal_scale, fixed vertical_scale)
{
    bgs_manager::set_scale(_handle, horizontal_scale, vertical_scale);
}

fixed affine_bg_ptr::horizontal_shear() const
{
    return bgs_manager::horizontal_shear(_handle);
}

void affine_bg_ptr::set_horizontal_shear(fixed horizontal_shear)
{
    bgs_manager::set_horizontal_shear(_handle, horizontal_shear);
}

fixed affine_bg_ptr::vertical_shear() const
{
    return bgs_manager::vertical_shear(_handle);
}

void affine_bg_ptr::set_vertical_shear(fixed vertical_shear)
{
    bgs_manager::set_vertical_shear(_handle, vertical_shear);
}

void affine_bg_ptr::set_shear(fixed shear)
{
    bgs_manager::set_shear(_handle, shear);
}

void affine_bg_ptr::set_shear(fixed horizontal_shear, fixed vertical_shear)
{
    bgs_manager::set_shear(_handle, horizontal_shear, vertical_shear);
}

bool affine_bg_ptr::horizontal_flip() const
{
    return bgs_manager::horizontal_flip(_handle);
}

void affine_bg_ptr::set_horizontal_flip(bool horizontal_flip)
{
    bgs_manager::set_horizontal_flip(_handle, horizontal_flip);
}

bool affine_bg_ptr::vertical_flip() const
{
    return bgs_manager::vertical_flip(_handle);
}

void affine_bg_ptr::set_vertical_flip(bool vertical_flip)
{
    bgs_manager::set_vertical_flip(_handle, vertical_flip);
}

fixed affine_bg_ptr::pivot_x() const
{
    return bgs_manager::pivot_position(_handle).x();
}

void affine_bg_ptr::set_pivot_x(fixed pivot_x)
{
    bgs_manager::set_pivot_x(_handle, pivot_x);
}

fixed affine_bg_ptr::pivot_y() const
{
    return bgs_manager::pivot_position(_handle).y();
}

void affine_bg_ptr::set_pivot_y(fixed pivot_y)
{
    bgs_manager::set_pivot_y(_handle, pivot_y);
}

const fixed_point& affine_bg_ptr::pivot_position() const
{
    return bgs_manager::pivot_position(_handle);
}

void affine_bg_ptr::set_pivot_position(fixed pivot_x, fixed pivot_y)
{
    bgs_manager::set_pivot_position(_handle, fixed_point(pivot_x, pivot_y));
}

void affine_bg_ptr::set_pivot_position(const fixed_point& pivot_position)
{
    bgs_manager::set_pivot_position(_handle, pivot_position);
}

const affine_bg_mat_attributes& affine_bg_ptr::mat_attributes() const
{
    return bgs_manager::mat_attributes(_handle);
}

void affine_bg_ptr::set_mat_attributes(const affine_mat_attributes& mat_attributes)
{
    bgs_manager::set_mat_attributes(_handle, mat_attributes);
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

void affine_bg_ptr::put_below()
{
    bgs_manager::put_below(_handle);
}

bool affine_bg_ptr::wrapping_enabled() const
{
    return bgs_manager::wrapping_enabled(_handle);
}

void affine_bg_ptr::set_wrapping_enabled(bool wrapping_enabled)
{
    bgs_manager::set_wrapping_enabled(_handle, wrapping_enabled);
}

bool affine_bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_handle);
}

void affine_bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool affine_bg_ptr::blending_top_enabled() const
{
    return bgs_manager::blending_top_enabled(_handle);
}

void affine_bg_ptr::set_blending_top_enabled(bool blending_top_enabled)
{
    bgs_manager::set_blending_top_enabled(_handle, blending_top_enabled);
}

bool affine_bg_ptr::blending_bottom_enabled() const
{
    return bgs_manager::blending_bottom_enabled(_handle);
}

void affine_bg_ptr::set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    bgs_manager::set_blending_bottom_enabled(_handle, blending_bottom_enabled);
}

bn::green_swap_mode affine_bg_ptr::green_swap_mode() const
{
    return bgs_manager::green_swap_mode(_handle);
}

void affine_bg_ptr::set_green_swap_mode(bn::green_swap_mode green_swap_mode)
{
    bgs_manager::set_green_swap_mode(_handle, green_swap_mode);
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
    if(const camera_ptr* camera_ref = camera.get())
    {
        bgs_manager::set_camera(_handle, camera_ptr(*camera_ref));
    }
    else
    {
        bgs_manager::remove_camera(_handle);
    }
}

void affine_bg_ptr::set_camera(optional<camera_ptr>&& camera)
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

optional<int> affine_bg_ptr::hw_id() const
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
