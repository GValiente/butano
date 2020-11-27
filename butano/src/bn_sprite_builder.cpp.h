/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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
    _shape_size(item.shape_size()),
    _graphics_index(0)
{
}

sprite_builder::sprite_builder(const sprite_item& item, int graphics_index) :
    _item(item),
    _shape_size(item.shape_size()),
    _graphics_index(graphics_index)
{
    BN_ASSERT(graphics_index >= 0, "Invalid graphics index: ", graphics_index);
    BN_ASSERT(graphics_index < item.tiles_item().graphics_count(), "Invalid graphics index: ", graphics_index, " - ",
              item.tiles_item().graphics_count());
}

sprite_builder::sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                               sprite_palette_ptr palette) :
    _shape_size(shape_size),
    _graphics_index(0),
    _tiles(move(tiles)),
    _palette(move(palette))
{
    BN_ASSERT(_tiles->tiles_count() == _shape_size.tiles_count(_palette->bpp()),
              "Invalid tiles count: ", _tiles->tiles_count(), " - ", _shape_size.tiles_count(_palette->bpp()));
}

sprite_builder& sprite_builder::set_rotation_angle(fixed rotation_angle)
{
    if(_affine_mat)
    {
        _affine_mat->set_rotation_angle(rotation_angle);
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

sprite_builder& sprite_builder::set_horizontal_scale(fixed horizontal_scale)
{
    if(_affine_mat)
    {
        _affine_mat->set_horizontal_scale(horizontal_scale);
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

sprite_builder& sprite_builder::set_vertical_scale(fixed vertical_scale)
{
    if(_affine_mat)
    {
        _affine_mat->set_vertical_scale(vertical_scale);
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
    if(_affine_mat)
    {
        _affine_mat->set_scale(scale);
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
    if(_affine_mat)
    {
        _affine_mat->set_scale(horizontal_scale, vertical_scale);
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

sprite_builder& sprite_builder::set_bg_priority(int bg_priority)
{
    BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid BG priority: ", bg_priority);

    _bg_priority = bg_priority;
    return *this;
}

sprite_builder& sprite_builder::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= sprites::min_z_order() && z_order <= sprites::max_z_order(), "Invalid Z order: ", z_order);

    _z_order = z_order;
    return *this;
}

sprite_builder& sprite_builder::set_horizontal_flip(bool horizontal_flip)
{
    _horizontal_flip = horizontal_flip;

    if(_affine_mat)
    {
        _affine_mat->set_horizontal_flip(horizontal_flip);
    }

    return *this;
}

sprite_builder& sprite_builder::set_vertical_flip(bool vertical_flip)
{
    _vertical_flip = vertical_flip;

    if(_affine_mat)
    {
        _affine_mat->set_vertical_flip(vertical_flip);
    }

    return *this;
}

sprite_builder& sprite_builder::set_blending_enabled(bool blending_enabled)
{
    BN_ASSERT(! blending_enabled || ! _window_enabled, "Blending and window can't be enabled at the same time");

    _blending_enabled = blending_enabled;
    return *this;
}

sprite_builder& sprite_builder::set_window_enabled(bool window_enabled)
{
    BN_ASSERT(! window_enabled || ! _blending_enabled, "Blending and window can't be enabled at the same time");

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
    if(_item)
    {
        return _item->tiles_item().create_tiles(_graphics_index);
    }

    BN_ASSERT(_tiles, "Tiles have been already released");

    return *_tiles;
}

sprite_palette_ptr sprite_builder::palette() const
{
    if(_item)
    {
        return _item->palette_item().create_palette();
    }

    BN_ASSERT(_palette, "Palette has been already released");

    return *_palette;
}

optional<sprite_tiles_ptr> sprite_builder::tiles_optional() const
{
    optional<sprite_tiles_ptr> result;

    if(_item)
    {
        result = _item->tiles_item().create_tiles_optional(_graphics_index);
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

    if(_item)
    {
        result = _item->palette_item().create_palette_optional();
    }
    else
    {
        result = _palette;
    }

    return result;
}

sprite_tiles_ptr sprite_builder::release_tiles()
{
    if(_item)
    {
        return _item->tiles_item().create_tiles(_graphics_index);
    }

    BN_ASSERT(_tiles, "Tiles have been already released");

    sprite_tiles_ptr result = move(*_tiles);
    _tiles.reset();
    return result;
}

sprite_palette_ptr sprite_builder::release_palette()
{
    if(_item)
    {
        return _item->palette_item().create_palette();
    }

    BN_ASSERT(_palette, "Palette has been already released");

    sprite_palette_ptr result = move(*_palette);
    _palette.reset();
    return result;
}

optional<sprite_tiles_ptr> sprite_builder::release_tiles_optional()
{
    optional<sprite_tiles_ptr> result;

    if(_item)
    {
        result = _item->tiles_item().create_tiles_optional(_graphics_index);
    }
    else
    {
        if(_tiles)
        {
            result = move(*_tiles);
            _tiles.reset();
        }
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::release_palette_optional()
{
    optional<sprite_palette_ptr> result;

    if(_item)
    {
        result = _item->palette_item().create_palette_optional();
    }
    else
    {
        if(_palette)
        {
            result = move(*_palette);
            _palette.reset();
        }
    }

    return result;
}

sprite_builder& sprite_builder::set_affine_mat(const sprite_affine_mat_ptr& affine_mat)
{
    _affine_mat = affine_mat;
    _remove_affine_mat_when_not_needed = false;
    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(sprite_affine_mat_ptr&& affine_mat)
{
    _affine_mat = move(affine_mat);
    _remove_affine_mat_when_not_needed = false;
    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat)
{
    if(affine_mat)
    {
        _affine_mat = affine_mat;
        _remove_affine_mat_when_not_needed = false;
    }
    else
    {
        _affine_mat.reset();
    }

    return *this;
}

sprite_builder& sprite_builder::set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat)
{
    if(affine_mat)
    {
        _affine_mat = move(affine_mat);
        _remove_affine_mat_when_not_needed = false;
    }
    else
    {
        _affine_mat.reset();
    }

    return *this;
}

}
