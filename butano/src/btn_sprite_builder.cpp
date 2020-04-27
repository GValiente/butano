#include "btn_sprite_builder.h"

#include "btn_sprites.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace btn
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
    BTN_ASSERT(graphics_index >= 0, "Invalid graphics index: ", graphics_index);
    BTN_ASSERT(graphics_index < item.tiles_item().graphics_count(), "Invalid graphics index: ", graphics_index, " - ",
               item.tiles_item().graphics_count());
}

sprite_builder::sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles_ptr,
                               sprite_palette_ptr palette_ptr) :
    _tiles_ptr(move(tiles_ptr)),
    _palette_ptr(move(palette_ptr)),
    _shape_size(shape_size),
    _graphics_index(0)
{
    BTN_ASSERT(_tiles_ptr->tiles_count() == _shape_size.tiles_count(_palette_ptr->bpp_mode()),
               "Invalid tiles ptr size: ", _tiles_ptr->tiles_count(), " - ",
               _shape_size.tiles_count(_palette_ptr->bpp_mode()));
}

sprite_builder& sprite_builder::set_rotation_angle(fixed rotation_angle)
{
    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_rotation_angle(rotation_angle);

        if(_remove_affine_mat_when_not_needed && _affine_mat_ptr->identity())
        {
            _affine_mat_ptr.reset();
        }
    }
    else if(rotation_angle != 0)
    {
        sprite_affine_mat_attributes affine_mat_attributes;
        affine_mat_attributes.set_rotation_angle(rotation_angle);
        affine_mat_attributes.set_horizontal_flip(_horizontal_flip);
        affine_mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = sprite_affine_mat_ptr::create(affine_mat_attributes);
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale_x(fixed scale_x)
{
    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale_x(scale_x);

        if(_remove_affine_mat_when_not_needed && _affine_mat_ptr->identity())
        {
            _affine_mat_ptr.reset();
        }
    }
    else if(scale_x != 1)
    {
        sprite_affine_mat_attributes affine_mat_attributes;
        affine_mat_attributes.set_scale_x(scale_x);
        affine_mat_attributes.set_horizontal_flip(_horizontal_flip);
        affine_mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = sprite_affine_mat_ptr::create(affine_mat_attributes);
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale_y(fixed scale_y)
{
    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale_y(scale_y);

        if(_remove_affine_mat_when_not_needed && _affine_mat_ptr->identity())
        {
            _affine_mat_ptr.reset();
        }
    }
    else if(scale_y != 1)
    {
        sprite_affine_mat_attributes affine_mat_attributes;
        affine_mat_attributes.set_scale_y(scale_y);
        affine_mat_attributes.set_horizontal_flip(_horizontal_flip);
        affine_mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = sprite_affine_mat_ptr::create(affine_mat_attributes);
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale(fixed scale)
{
    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale(scale);

        if(_remove_affine_mat_when_not_needed && _affine_mat_ptr->identity())
        {
            _affine_mat_ptr.reset();
        }
    }
    else if(scale != 1)
    {
        sprite_affine_mat_attributes affine_mat_attributes;
        affine_mat_attributes.set_scale(scale);
        affine_mat_attributes.set_horizontal_flip(_horizontal_flip);
        affine_mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = sprite_affine_mat_ptr::create(affine_mat_attributes);
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale(fixed scale_x, fixed scale_y)
{
    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale(scale_x, scale_y);

        if(_remove_affine_mat_when_not_needed && _affine_mat_ptr->identity())
        {
            _affine_mat_ptr.reset();
        }
    }
    else if(scale_x != 1 || scale_y != 1)
    {
        sprite_affine_mat_attributes affine_mat_attributes;
        affine_mat_attributes.set_scale(scale_x, scale_y);
        affine_mat_attributes.set_horizontal_flip(_horizontal_flip);
        affine_mat_attributes.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = sprite_affine_mat_ptr::create(affine_mat_attributes);
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_bg_priority(int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid bg priority: ", bg_priority);

    _bg_priority = bg_priority;
    return *this;
}

sprite_builder& sprite_builder::set_z_order(int z_order)
{
    BTN_ASSERT(z_order >= sprites::min_z_order() && z_order <= sprites::max_z_order(), "Invalid z order: ", z_order);

    _z_order = z_order;
    return *this;
}

sprite_builder& sprite_builder::set_horizontal_flip(bool horizontal_flip)
{
    _horizontal_flip = horizontal_flip;

    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_horizontal_flip(horizontal_flip);
    }

    return *this;
}

sprite_builder& sprite_builder::set_vertical_flip(bool vertical_flip)
{
    _vertical_flip = vertical_flip;

    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_vertical_flip(vertical_flip);
    }

    return *this;
}

sprite_builder& sprite_builder::set_blending_enabled(bool blending_enabled)
{
    BTN_ASSERT(! blending_enabled || ! _window_enabled, "Blending and window can't be enabled at the same time");

    _blending_enabled = blending_enabled;
    return *this;
}

sprite_builder& sprite_builder::set_window_enabled(bool window_enabled)
{
    BTN_ASSERT(! window_enabled || ! _blending_enabled, "Blending and window can't be enabled at the same time");

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

optional<sprite_ptr> sprite_builder::optional_build() const
{
    return sprite_ptr::optional_create(*this);
}

optional<sprite_ptr> sprite_builder::optional_release_build()
{
    return sprite_ptr::optional_create(move(*this));
}

sprite_tiles_ptr sprite_builder::tiles() const
{
    if(_item)
    {
        return _item->tiles_item().create_tiles(_graphics_index);
    }

    BTN_ASSERT(_tiles_ptr, "Tiles has been already released");

    return *_tiles_ptr;
}

sprite_palette_ptr sprite_builder::palette() const
{
    if(_item)
    {
        return _item->palette_item().create_palette();
    }

    BTN_ASSERT(_palette_ptr, "Palette has been already released");

    return *_palette_ptr;
}

optional<sprite_tiles_ptr> sprite_builder::optional_tiles() const
{
    optional<sprite_tiles_ptr> result;

    if(_item)
    {
        result = _item->tiles_item().optional_create_tiles(_graphics_index);
    }
    else
    {
        result = _tiles_ptr;
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::optional_palette() const
{
    optional<sprite_palette_ptr> result;

    if(_item)
    {
        result = _item->palette_item().optional_create_palette();
    }
    else
    {
        result = _palette_ptr;
    }

    return result;
}

sprite_tiles_ptr sprite_builder::release_tiles()
{
    if(_item)
    {
        return _item->tiles_item().create_tiles(_graphics_index);
    }

    BTN_ASSERT(_tiles_ptr, "Tiles has been already released");

    sprite_tiles_ptr result = move(*_tiles_ptr);
    _tiles_ptr.reset();
    return result;
}

sprite_palette_ptr sprite_builder::release_palette()
{
    if(_item)
    {
        return _item->palette_item().create_palette();
    }

    BTN_ASSERT(_palette_ptr, "Palette has been already released");

    sprite_palette_ptr result = move(*_palette_ptr);
    _palette_ptr.reset();
    return result;
}

optional<sprite_tiles_ptr> sprite_builder::optional_release_tiles()
{
    optional<sprite_tiles_ptr> result;

    if(_item)
    {
        result = _item->tiles_item().optional_create_tiles(_graphics_index);
    }
    else
    {
        result = move(_tiles_ptr);
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::optional_release_palette()
{
    optional<sprite_palette_ptr> result;

    if(_item)
    {
        result = _item->palette_item().optional_create_palette();
    }
    else
    {
        result = move(_palette_ptr);
    }

    return result;
}

}
