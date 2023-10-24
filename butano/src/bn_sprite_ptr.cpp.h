/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_ptr.h"

#include "bn_size.h"
#include "bn_sprite_builder.h"
#include "bn_sprites_manager.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprite_regular_second_attributes.h"

namespace bn
{

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_item& item)
{
    return sprite_ptr(sprites_manager::create(fixed_point(x, y), item.shape_size(),
                                              item.tiles_item().create_tiles(),
                                              item.palette_item().create_palette()));
}

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    return sprite_ptr(sprites_manager::create(fixed_point(x, y), item.shape_size(),
                                              item.tiles_item().create_tiles(graphics_index),
                                              item.palette_item().create_palette()));
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_item& item)
{
    return sprite_ptr(sprites_manager::create(position, item.shape_size(),
                                              item.tiles_item().create_tiles(),
                                              item.palette_item().create_palette()));
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_item& item, int graphics_index)
{
    return sprite_ptr(sprites_manager::create(position, item.shape_size(),
                                              item.tiles_item().create_tiles(graphics_index),
                                              item.palette_item().create_palette()));
}

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                              sprite_palette_ptr palette)
{
    BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", shape_size.tiles_count(palette.bpp()));

    return sprite_ptr(sprites_manager::create(fixed_point(x, y), shape_size, move(tiles), move(palette)));
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                              sprite_palette_ptr palette)
{
    BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", shape_size.tiles_count(palette.bpp()));

    return sprite_ptr(sprites_manager::create(position, shape_size, move(tiles), move(palette)));
}

sprite_ptr sprite_ptr::create(const sprite_builder& builder)
{
    return sprite_ptr(sprites_manager::create(sprite_builder(builder)));
}

sprite_ptr sprite_ptr::create(sprite_builder&& builder)
{
    return sprite_ptr(sprites_manager::create(move(builder)));
}

optional<sprite_ptr> sprite_ptr::create_optional(fixed x, fixed y, const sprite_item& item)
{
    return create_optional(fixed_point(x, y), item);
}

optional<sprite_ptr> sprite_ptr::create_optional(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    return create_optional(fixed_point(x, y), item, graphics_index);
}

optional<sprite_ptr> sprite_ptr::create_optional(const fixed_point& position, const sprite_item& item)
{
    optional<sprite_tiles_ptr> tiles = item.tiles_item().create_tiles();
    optional<sprite_ptr> result;

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<sprite_palette_ptr> palette = item.palette_item().create_palette();

        if(sprite_palette_ptr* palette_ptr = palette.get())
        {
            if(handle_type handle = sprites_manager::create_optional(
                        position, item.shape_size(), move(*tiles_ptr), move(*palette_ptr)))
            {
                result = sprite_ptr(handle);
            }
        }
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::create_optional(const fixed_point& position, const sprite_item& item,
                                                 int graphics_index)
{
    optional<sprite_tiles_ptr> tiles = item.tiles_item().create_tiles(graphics_index);
    optional<sprite_ptr> result;

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<sprite_palette_ptr> palette = item.palette_item().create_palette();

        if(sprite_palette_ptr* palette_ptr = palette.get())
        {
            if(handle_type handle = sprites_manager::create_optional(
                        position, item.shape_size(), move(*tiles_ptr), move(*palette_ptr)))
            {
                result = sprite_ptr(handle);
            }
        }
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::create_optional(fixed x, fixed y, const sprite_shape_size& shape_size,
                                                 sprite_tiles_ptr tiles, sprite_palette_ptr palette)
{
    BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", shape_size.tiles_count(palette.bpp()));

    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::create_optional(
                fixed_point(x, y), shape_size, move(tiles), move(palette)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::create_optional(const fixed_point& position, const sprite_shape_size& shape_size,
                                                 sprite_tiles_ptr tiles, sprite_palette_ptr palette)
{
    BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", shape_size.tiles_count(palette.bpp()));

    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::create_optional(position, shape_size, move(tiles), move(palette)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::create_optional(const sprite_builder& builder)
{
    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::create_optional(sprite_builder(builder)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::create_optional(sprite_builder&& builder)
{
    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::create_optional(move(builder)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

sprite_ptr::sprite_ptr(const sprite_ptr& other) :
    sprite_ptr(other._handle)
{
    sprites_manager::increase_usages(_handle);
}

sprite_ptr& sprite_ptr::operator=(const sprite_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle)
        {
            sprites_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        sprites_manager::increase_usages(_handle);
    }

    return *this;
}

sprite_ptr::~sprite_ptr()
{
    if(_handle)
    {
        sprites_manager::decrease_usages(_handle);
    }
}

sprite_shape_size sprite_ptr::shape_size() const
{
    return sprites_manager::shape_size(_handle);
}

size sprite_ptr::dimensions() const
{
    return sprites_manager::dimensions(_handle);
}

const sprite_tiles_ptr& sprite_ptr::tiles() const
{
    return sprites_manager::tiles(_handle);
}

void sprite_ptr::set_tiles(const sprite_tiles_ptr& tiles)
{
    sprites_manager::set_tiles(_handle, tiles);
}

void sprite_ptr::set_tiles(sprite_tiles_ptr&& tiles)
{
    sprites_manager::set_tiles(_handle, move(tiles));
}

void sprite_ptr::set_tiles(const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles)
{
    sprites_manager::set_tiles(_handle, shape_size, tiles);
}

void sprite_ptr::set_tiles(const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles)
{
    sprites_manager::set_tiles(_handle, shape_size, move(tiles));
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item)
{
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles();

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        sprites_manager::set_tiles(_handle, move(*tiles_ptr));
    }
    else
    {
        sprites_manager::remove_tiles(_handle);
        sprites_manager::set_tiles(_handle, tiles_item.create_tiles());
    }
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, int graphics_index)
{
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles(graphics_index);

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        sprites_manager::set_tiles(_handle, move(*tiles_ptr));
    }
    else
    {
        sprites_manager::remove_tiles(_handle);
        sprites_manager::set_tiles(_handle, tiles_item.create_tiles(graphics_index));
    }
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size)
{
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles();

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        sprites_manager::set_tiles(_handle, shape_size, move(*tiles_ptr));
    }
    else
    {
        sprites_manager::remove_tiles(_handle);
        sprites_manager::set_tiles(_handle, shape_size, tiles_item.create_tiles());
    }
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size,
                           int graphics_index)
{
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles(graphics_index);

    if(sprite_tiles_ptr* tiles_ptr = tiles.get())
    {
        sprites_manager::set_tiles(_handle, shape_size, move(*tiles_ptr));
    }
    else
    {
        sprites_manager::remove_tiles(_handle);
        sprites_manager::set_tiles(_handle, shape_size, tiles_item.create_tiles(graphics_index));
    }
}

const sprite_palette_ptr& sprite_ptr::palette() const
{
    return sprites_manager::palette(_handle);
}

void sprite_ptr::set_palette(const sprite_palette_ptr& palette)
{
    sprites_manager::set_palette(_handle, this->palette().bpp(), palette);
}

void sprite_ptr::set_palette(sprite_palette_ptr&& palette)
{
    sprites_manager::set_palette(_handle, this->palette().bpp(), move(palette));
}

void sprite_ptr::set_palette(const sprite_palette_item& palette_item)
{
    bpp_mode old_bpp = palette().bpp();

    if(old_bpp == bpp_mode::BPP_4 || palette_item.bpp() == bpp_mode::BPP_4)
    {
        optional<sprite_palette_ptr> palette = palette_item.find_palette();

        if(sprite_palette_ptr* palette_ptr = palette.get())
        {
            sprites_manager::set_palette(_handle, old_bpp, move(*palette_ptr));
        }
        else
        {
            sprites_manager::remove_palette(_handle);
            sprites_manager::set_palette(_handle, old_bpp, palette_item.create_new_palette());
        }
    }
    else
    {
        sprites_manager::set_palette(_handle, old_bpp, palette_item.create_palette());
    }
}

void sprite_ptr::set_tiles_and_palette(sprite_tiles_ptr tiles, sprite_palette_ptr palette)
{
    sprites_manager::set_tiles_and_palette(_handle, shape_size(), move(tiles), move(palette));
}

void sprite_ptr::set_tiles_and_palette(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                                       sprite_palette_ptr palette)
{
    sprites_manager::set_tiles_and_palette(_handle, shape_size, move(tiles), move(palette));
}

void sprite_ptr::set_item(const sprite_item& item)
{
    const sprite_tiles_item& tiles_item = item.tiles_item();
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles();
    sprite_tiles_ptr* tiles_ptr = tiles.get();

    if(! tiles_ptr)
    {
        sprites_manager::remove_tiles(_handle);
        tiles = tiles_item.create_tiles();
        tiles_ptr = tiles.get();
    }

    const sprite_palette_item& palette_item = item.palette_item();
    optional<sprite_palette_ptr> palette = palette_item.find_palette();
    sprite_palette_ptr* palette_ptr = palette.get();

    if(! palette_ptr)
    {
        if(palette_item.bpp() == bpp_mode::BPP_4 || this->palette().bpp() == bpp_mode::BPP_4)
        {
            sprites_manager::remove_palette(_handle);
        }

        palette = palette_item.create_new_palette();
        palette_ptr = palette.get();
    }

    sprites_manager::set_tiles_and_palette(_handle, item.shape_size(), move(*tiles_ptr), move(*palette_ptr));
}

void sprite_ptr::set_item(const sprite_item& item, int graphics_index)
{
    const sprite_tiles_item& tiles_item = item.tiles_item();
    optional<sprite_tiles_ptr> tiles = tiles_item.find_tiles(graphics_index);
    sprite_tiles_ptr* tiles_ptr = tiles.get();

    if(! tiles_ptr)
    {
        sprites_manager::remove_tiles(_handle);
        tiles = tiles_item.create_tiles(graphics_index);
        tiles_ptr = tiles.get();
    }

    const sprite_palette_item& palette_item = item.palette_item();
    optional<sprite_palette_ptr> palette = palette_item.find_palette();
    sprite_palette_ptr* palette_ptr = palette.get();

    if(! palette_ptr)
    {
        if(palette_item.bpp() == bpp_mode::BPP_4 || this->palette().bpp() == bpp_mode::BPP_4)
        {
            sprites_manager::remove_palette(_handle);
        }

        palette = palette_item.create_new_palette();
        palette_ptr = palette.get();
    }

    sprites_manager::set_tiles_and_palette(_handle, item.shape_size(), move(*tiles_ptr), move(*palette_ptr));
}

fixed sprite_ptr::x() const
{
    return position().x();
}

void sprite_ptr::set_x(fixed x)
{
    sprites_manager::set_x(_handle, x);
}

fixed sprite_ptr::y() const
{
    return position().y();
}

void sprite_ptr::set_y(fixed y)
{
    sprites_manager::set_y(_handle, y);
}

const fixed_point& sprite_ptr::position() const
{
    return sprites_manager::position(_handle);
}

void sprite_ptr::set_position(fixed x, fixed y)
{
    sprites_manager::set_position(_handle, fixed_point(x, y));
}

void sprite_ptr::set_position(const fixed_point& position)
{
    sprites_manager::set_position(_handle, position);
}

fixed sprite_ptr::rotation_angle() const
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        return affine_mat_ptr->rotation_angle();
    }

    return 0;
}

void sprite_ptr::set_rotation_angle(fixed rotation_angle)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_rotation_angle(rotation_angle);
    }
    else if(rotation_angle != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_rotation_angle(rotation_angle);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

fixed sprite_ptr::horizontal_scale() const
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        return affine_mat_ptr->horizontal_scale();
    }

    return 1;
}

void sprite_ptr::set_horizontal_scale(fixed horizontal_scale)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_horizontal_scale(horizontal_scale);
    }
    else if(horizontal_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_horizontal_scale(horizontal_scale);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

fixed sprite_ptr::vertical_scale() const
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        return affine_mat_ptr->vertical_scale();
    }

    return 1;
}

void sprite_ptr::set_vertical_scale(fixed vertical_scale)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_vertical_scale(vertical_scale);
    }
    else if(vertical_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_vertical_scale(vertical_scale);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

void sprite_ptr::set_scale(fixed scale)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_scale(scale);
    }
    else if(scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_scale(scale);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

void sprite_ptr::set_scale(fixed horizontal_scale, fixed vertical_scale)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_scale(horizontal_scale, vertical_scale);
    }
    else if(horizontal_scale != 1 || vertical_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_scale(horizontal_scale, vertical_scale);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

fixed sprite_ptr::horizontal_shear() const
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        return affine_mat_ptr->horizontal_shear();
    }

    return 1;
}

void sprite_ptr::set_horizontal_shear(fixed horizontal_shear)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_horizontal_shear(horizontal_shear);
    }
    else if(horizontal_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_horizontal_shear(horizontal_shear);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

fixed sprite_ptr::vertical_shear() const
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        return affine_mat_ptr->vertical_shear();
    }

    return 1;
}

void sprite_ptr::set_vertical_shear(fixed vertical_shear)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_vertical_shear(vertical_shear);
    }
    else if(vertical_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_vertical_shear(vertical_shear);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

void sprite_ptr::set_shear(fixed shear)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_shear(shear);
    }
    else if(shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_shear(shear);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

void sprite_ptr::set_shear(fixed horizontal_shear, fixed vertical_shear)
{
    optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle);

    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        affine_mat_ptr->set_shear(horizontal_shear, vertical_shear);
    }
    else if(horizontal_shear != 0 || vertical_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_shear(horizontal_shear, vertical_shear);
        sprites_manager::set_new_affine_mat(_handle, mat_attributes);
    }
}

int sprite_ptr::bg_priority() const
{
    return sprites_manager::bg_priority(_handle);
}

void sprite_ptr::set_bg_priority(int bg_priority)
{
    sprites_manager::set_bg_priority(_handle, bg_priority);
}

int sprite_ptr::z_order() const
{
    return sprites_manager::z_order(_handle);
}

void sprite_ptr::set_z_order(int z_order)
{
    sprites_manager::set_z_order(_handle, z_order);
}

void sprite_ptr::put_above()
{
    sprites_manager::put_above(_handle);
}

void sprite_ptr::put_below()
{
    sprites_manager::put_below(_handle);
}

bool sprite_ptr::horizontal_flip() const
{
    return sprites_manager::horizontal_flip(_handle);
}

void sprite_ptr::set_horizontal_flip(bool horizontal_flip)
{
    sprites_manager::set_horizontal_flip(_handle, horizontal_flip);
}

bool sprite_ptr::vertical_flip() const
{
    return sprites_manager::vertical_flip(_handle);
}

void sprite_ptr::set_vertical_flip(bool vertical_flip)
{
    sprites_manager::set_vertical_flip(_handle, vertical_flip);
}

bool sprite_ptr::mosaic_enabled() const
{
    return sprites_manager::mosaic_enabled(_handle);
}

void sprite_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    sprites_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool sprite_ptr::blending_enabled() const
{
    return sprites_manager::blending_enabled(_handle);
}

void sprite_ptr::set_blending_enabled(bool blending_enabled)
{
    sprites_manager::set_blending_enabled(_handle, blending_enabled);
}

bool sprite_ptr::window_enabled() const
{
    return sprites_manager::window_enabled(_handle);
}

void sprite_ptr::set_window_enabled(bool window_enabled)
{
    sprites_manager::set_window_enabled(_handle, window_enabled);
}

bool sprite_ptr::double_size() const
{
    return sprites_manager::double_size(_handle);
}

sprite_double_size_mode sprite_ptr::double_size_mode() const
{
    return sprites_manager::double_size_mode(_handle);
}

void sprite_ptr::set_double_size_mode(sprite_double_size_mode double_size_mode)
{
    sprites_manager::set_double_size_mode(_handle, double_size_mode);
}

bool sprite_ptr::visible() const
{
    return sprites_manager::visible(_handle);
}

void sprite_ptr::set_visible(bool visible)
{
    sprites_manager::set_visible(_handle, visible);
}

const optional<camera_ptr>& sprite_ptr::camera() const
{
    return sprites_manager::camera(_handle);
}

void sprite_ptr::set_camera(const camera_ptr& camera)
{
    sprites_manager::set_camera(_handle, camera_ptr(camera));
}

void sprite_ptr::set_camera(camera_ptr&& camera)
{
    sprites_manager::set_camera(_handle, move(camera));
}

void sprite_ptr::set_camera(const optional<camera_ptr>& camera)
{
    if(const camera_ptr* camera_ref = camera.get())
    {
        sprites_manager::set_camera(_handle, camera_ptr(*camera_ref));
    }
    else
    {
        sprites_manager::remove_camera(_handle);
    }
}

void sprite_ptr::set_camera(optional<camera_ptr>&& camera)
{
    if(camera_ptr* camera_ref = camera.get())
    {
        sprites_manager::set_camera(_handle, move(*camera_ref));
    }
    else
    {
        sprites_manager::remove_camera(_handle);
    }
}

void sprite_ptr::remove_camera()
{
    sprites_manager::remove_camera(_handle);
}

const optional<sprite_affine_mat_ptr>& sprite_ptr::affine_mat() const
{
    return sprites_manager::affine_mat(_handle);
}

void sprite_ptr::set_affine_mat(const sprite_affine_mat_ptr& affine_mat)
{
    sprites_manager::set_affine_mat(_handle, affine_mat);
}

void sprite_ptr::set_affine_mat(sprite_affine_mat_ptr&& affine_mat)
{
    sprites_manager::set_affine_mat(_handle, move(affine_mat));
}

void sprite_ptr::set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)
{
    if(const sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        sprites_manager::set_affine_mat(_handle, *affine_mat_ptr);
    }
    else
    {
        sprites_manager::remove_affine_mat(_handle);
    }
}

void sprite_ptr::set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)
{
    if(sprite_affine_mat_ptr* affine_mat_ptr = affine_mat.get())
    {
        sprites_manager::set_affine_mat(_handle, move(*affine_mat_ptr));
    }
    else
    {
        sprites_manager::remove_affine_mat(_handle);
    }
}

void sprite_ptr::remove_affine_mat()
{
    sprites_manager::remove_affine_mat(_handle);
}

bool sprite_ptr::remove_affine_mat_when_not_needed() const
{
    return sprites_manager::remove_affine_mat_when_not_needed(_handle);
}

void sprite_ptr::set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)
{
    sprites_manager::set_remove_affine_mat_when_not_needed(_handle, remove_when_not_needed);
}

sprite_first_attributes sprite_ptr::first_attributes() const
{
    return sprites_manager::first_attributes(_handle);
}

void sprite_ptr::set_first_attributes(const sprite_first_attributes& first_attributes)
{
    sprites_manager::set_first_attributes(_handle, first_attributes);
}

sprite_regular_second_attributes sprite_ptr::regular_second_attributes() const
{
    return sprites_manager::regular_second_attributes(_handle);
}

void sprite_ptr::set_regular_second_attributes(const sprite_regular_second_attributes& second_attributes)
{
    sprites_manager::set_regular_second_attributes(_handle, second_attributes);
}

sprite_affine_second_attributes sprite_ptr::affine_second_attributes() const
{
    return sprites_manager::affine_second_attributes(_handle);
}

void sprite_ptr::set_affine_second_attributes(const sprite_affine_second_attributes& second_attributes)
{
    sprites_manager::set_affine_second_attributes(_handle, second_attributes);
}

sprite_third_attributes sprite_ptr::third_attributes() const
{
    return sprites_manager::third_attributes(_handle);
}

void sprite_ptr::set_third_attributes(const sprite_third_attributes& third_attributes)
{
    sprites_manager::set_third_attributes(_handle, third_attributes);
}

}
