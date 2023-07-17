/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_builder.h"

#include "bn_sprites.h"
#include "bn_sprite_ptr.h"
#include "bn_affine_mat_attributes.h"

namespace bn
{

sprite_builder::sprite_builder(const sprite_item& item) :
    _item(item),
    _graphics_index(0),
    _shape_size(item.shape_size())
{
}

sprite_builder::sprite_builder(const sprite_item& item, int graphics_index) :
    _item(item),
    _graphics_index(graphics_index),
    _shape_size(item.shape_size())
{
    BN_ASSERT(graphics_index >= 0 && graphics_index < item.tiles_item().graphics_count(),
              "Invalid graphics index: ", graphics_index, " - ", item.tiles_item().graphics_count());
}

sprite_builder::sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                               sprite_palette_ptr palette) :
    _graphics_index(0),
    _tiles(move(tiles)),
    _palette(move(palette)),
    _shape_size(shape_size)
{
    BN_ASSERT(_tiles->tiles_count() == _shape_size.tiles_count(_palette->bpp()),
              "Invalid tiles count: ", _tiles->tiles_count(), " - ", _shape_size.tiles_count(_palette->bpp()));
}

fixed sprite_builder::rotation_angle() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->rotation_angle() : 0;
}

sprite_builder& sprite_builder::set_rotation_angle(fixed rotation_angle)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_rotation_angle(rotation_angle);
    }
    else if(rotation_angle != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_rotation_angle(rotation_angle);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

fixed sprite_builder::horizontal_scale() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->horizontal_scale() : 1;
}

sprite_builder& sprite_builder::set_horizontal_scale(fixed horizontal_scale)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_horizontal_scale(horizontal_scale);
    }
    else if(horizontal_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_horizontal_scale(horizontal_scale);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

fixed sprite_builder::vertical_scale() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->vertical_scale() : 1;
}

sprite_builder& sprite_builder::set_vertical_scale(fixed vertical_scale)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_vertical_scale(vertical_scale);
    }
    else if(vertical_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_vertical_scale(vertical_scale);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale(fixed scale)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_scale(scale);
    }
    else if(scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_scale(scale);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale(fixed horizontal_scale, fixed vertical_scale)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_scale(horizontal_scale, vertical_scale);
    }
    else if(horizontal_scale != 1 || vertical_scale != 1)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_scale(horizontal_scale, vertical_scale);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

fixed sprite_builder::horizontal_shear() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->horizontal_shear() : 0;
}

sprite_builder& sprite_builder::set_horizontal_shear(fixed horizontal_shear)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_horizontal_shear(horizontal_shear);
    }
    else if(horizontal_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_horizontal_shear(horizontal_shear);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

fixed sprite_builder::vertical_shear() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->vertical_shear() : 0;
}

sprite_builder& sprite_builder::set_vertical_shear(fixed vertical_shear)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_vertical_shear(vertical_shear);
    }
    else if(vertical_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_vertical_shear(vertical_shear);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

sprite_builder& sprite_builder::set_shear(fixed shear)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_shear(shear);
    }
    else if(shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_shear(shear);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

sprite_builder& sprite_builder::set_shear(fixed horizontal_shear, fixed vertical_shear)
{
    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_shear(horizontal_shear, vertical_shear);
    }
    else if(horizontal_shear != 0 || vertical_shear != 0)
    {
        affine_mat_attributes mat_attributes;
        mat_attributes.set_shear(horizontal_shear, vertical_shear);
        mat_attributes.set_horizontal_flip(_horizontal_flip);
        mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat = sprite_affine_mat_ptr::create(mat_attributes);
    }

    return *this;
}

sprite_builder& sprite_builder::set_bg_priority(int bg_priority)
{
    BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid BG priority: ", bg_priority);

    _bg_priority = bg_priority;
    return *this;
}

sprite_builder& sprite_builder::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= sprites::min_z_order() && z_order <= sprites::max_z_order(), "Invalid z order: ", z_order);

    _z_order = z_order;
    return *this;
}

bool sprite_builder::horizontal_flip() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->horizontal_flip() : _horizontal_flip;
}

sprite_builder& sprite_builder::set_horizontal_flip(bool horizontal_flip)
{
    _horizontal_flip = horizontal_flip;

    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_horizontal_flip(horizontal_flip);
    }

    return *this;
}

bool sprite_builder::vertical_flip() const
{
    const sprite_affine_mat_ptr* affine_mat = _affine_mat.get();
    return affine_mat ? affine_mat->vertical_flip() : _vertical_flip;
}

sprite_builder& sprite_builder::set_vertical_flip(bool vertical_flip)
{
    _vertical_flip = vertical_flip;

    if(sprite_affine_mat_ptr* affine_mat = _affine_mat.get())
    {
        affine_mat->set_vertical_flip(vertical_flip);
    }

    return *this;
}

sprite_builder& sprite_builder::set_blending_enabled(bool blending_enabled)
{
    BN_BASIC_ASSERT(! blending_enabled || ! _window_enabled,
                    "Blending and window can't be enabled at the same time");

    _blending_enabled = blending_enabled;
    return *this;
}

sprite_builder& sprite_builder::set_window_enabled(bool window_enabled)
{
    BN_BASIC_ASSERT(! window_enabled || ! _blending_enabled,
                    "Blending and window can't be enabled at the same time");

    _window_enabled = window_enabled;
    return *this;
}

sprite_ptr sprite_builder::build() const
{
    return sprite_ptr::create(*this);
}

sprite_ptr sprite_builder::release_build()
{
    return sprite_ptr::create(move(*this));
}

optional<sprite_ptr> sprite_builder::build_optional() const
{
    return sprite_ptr::create_optional(*this);
}

optional<sprite_ptr> sprite_builder::release_build_optional()
{
    return sprite_ptr::create_optional(move(*this));
}

sprite_tiles_ptr sprite_builder::tiles() const
{
    if(const sprite_item* item = _item.get())
    {
        return item->tiles_item().create_tiles(_graphics_index);
    }

    return *_tiles;
}

sprite_palette_ptr sprite_builder::palette() const
{
    if(const sprite_item* item = _item.get())
    {
        return item->palette_item().create_palette();
    }

    return *_palette;
}

optional<sprite_tiles_ptr> sprite_builder::tiles_optional() const
{
    optional<sprite_tiles_ptr> result;

    if(const sprite_item* item = _item.get())
    {
        result = item->tiles_item().create_tiles_optional(_graphics_index);
    }
    else
    {
        result = _tiles;
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::palette_optional() const
{
    optional<sprite_palette_ptr> result;

    if(const sprite_item* item = _item.get())
    {
        result = item->palette_item().create_palette_optional();
    }
    else
    {
        result = _palette;
    }

    return result;
}

sprite_tiles_ptr sprite_builder::release_tiles()
{
    if(const sprite_item* item = _item.get())
    {
        return item->tiles_item().create_tiles(_graphics_index);
    }

    sprite_tiles_ptr result = move(*_tiles);
    _tiles.reset();
    return result;
}

sprite_palette_ptr sprite_builder::release_palette()
{
    if(const sprite_item* item = _item.get())
    {
        return item->palette_item().create_palette();
    }

    sprite_palette_ptr result = move(*_palette);
    _palette.reset();
    return result;
}

optional<sprite_tiles_ptr> sprite_builder::release_tiles_optional()
{
    optional<sprite_tiles_ptr> result;

    if(const sprite_item* item = _item.get())
    {
        result = item->tiles_item().create_tiles_optional(_graphics_index);
    }
    else
    {
        if(sprite_tiles_ptr* tiles = _tiles.get())
        {
            result = move(*tiles);
            _tiles.reset();
        }
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::release_palette_optional()
{
    optional<sprite_palette_ptr> result;

    if(const sprite_item* item = _item.get())
    {
        result = item->palette_item().create_palette_optional();
    }
    else
    {
        if(sprite_palette_ptr* palette = _palette.get())
        {
            result = move(*palette);
            _palette.reset();
        }
    }

    return result;
}

sprite_builder& sprite_builder::set_affine_mat(const sprite_affine_mat_ptr& affine_mat)
{
    _remove_affine_mat_when_not_needed = false;
    _affine_mat = affine_mat;
    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(sprite_affine_mat_ptr&& affine_mat)
{
    _remove_affine_mat_when_not_needed = false;
    _affine_mat = move(affine_mat);
    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)
{
    _remove_affine_mat_when_not_needed = affine_mat.has_value();
    _affine_mat = affine_mat;
    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)
{
    _remove_affine_mat_when_not_needed = affine_mat.has_value();
    _affine_mat = move(affine_mat);
    return *this;
}

}
