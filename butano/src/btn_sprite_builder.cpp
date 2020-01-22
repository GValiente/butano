#include "btn_sprite_builder.h"

#include "btn_sprite_ptr.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_affine_mat_builder.h"

namespace btn
{

sprite_builder::sprite_builder(const sprite_item& item, int graphics_index) :
    _item(item),
    _shape(item.shape()),
    _size(item.size()),
    _graphics_index(graphics_index)
{
    BTN_ASSERT(graphics_index >= 0, "Invalid graphics index: ", graphics_index);
    BTN_ASSERT(graphics_index < item.tiles().graphics(), "Invalid graphics index: ", graphics_index, " - ",
               item.tiles().graphics());
}

sprite_builder::sprite_builder(sprite_shape shape, sprite_size size, sprite_tiles_ptr tiles_ptr,
                               sprite_palette_ptr palette_ptr) :
    _tiles_ptr(move(tiles_ptr)),
    _palette_ptr(move(palette_ptr)),
    _shape(shape),
    _size(size),
    _graphics_index(0)
{
    BTN_ASSERT(_tiles_ptr->tiles_count() == shape_size().tiles(_palette_ptr->eight_bits_per_pixel()),
               "Invalid tiles ptr size: ", _tiles_ptr->tiles_count(), " - ",
               shape_size().tiles(_palette_ptr->eight_bits_per_pixel()));
}

sprite_builder& sprite_builder::set_rotation_angle(fixed rotation_angle)
{
    BTN_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle: ", rotation_angle);

    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_rotation_angle(rotation_angle);
    }
    else
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_rotation_angle(rotation_angle);
        affine_mat_builder.set_horizontal_flip(_horizontal_flip);
        affine_mat_builder.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = affine_mat_builder.build();
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale_x(fixed scale_x)
{
    BTN_ASSERT(scale_x > 0, "Invalid scale x: ", scale_x);

    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale_x(scale_x);
    }
    else
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_x(scale_x);
        affine_mat_builder.set_horizontal_flip(_horizontal_flip);
        affine_mat_builder.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = affine_mat_builder.build();
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_scale_y(fixed scale_y)
{
    BTN_ASSERT(scale_y > 0, "Invalid scale y: ", scale_y);

    if(_affine_mat_ptr)
    {
        _affine_mat_ptr->set_scale_y(scale_y);
    }
    else
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_y(scale_y);
        affine_mat_builder.set_horizontal_flip(_horizontal_flip);
        affine_mat_builder.set_vertical_flip(_vertical_flip);
        _affine_mat_ptr = affine_mat_builder.build();
        _remove_affine_mat_when_not_needed = true;
    }

    return *this;
}

sprite_builder& sprite_builder::set_bg_priority(int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites_manager::max_bg_priority(), "Invalid bg priority: ",
               bg_priority);

    _bg_priority = bg_priority;
    return *this;
}

sprite_builder& sprite_builder::set_z_order(int z_order)
{
    BTN_ASSERT(z_order >= 0 && z_order < sprites_manager::z_orders(), "Invalid z order: ", z_order);

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

sprite_ptr sprite_builder::build() const
{
    return sprite_ptr::create(*this);
}

sprite_ptr sprite_builder::build_and_release()
{
    return sprite_ptr::create(move(*this));
}

optional<sprite_ptr> sprite_builder::optional_build() const
{
    return sprite_ptr::optional_create(*this);
}

optional<sprite_ptr> sprite_builder::optional_build_and_release()
{
    return sprite_ptr::optional_create(move(*this));
}

optional<sprite_tiles_ptr> sprite_builder::tiles() const
{
    optional<sprite_tiles_ptr> result;

    if(_tiles_ptr)
    {
        result = _tiles_ptr;
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->tiles().create_tiles_ptr(_graphics_index, _tiles_create_mode);
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::palette() const
{
    optional<sprite_palette_ptr> result;

    if(_palette_ptr)
    {
        result = _palette_ptr;
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->palette().create_sprite_palette_ptr(_palette_create_mode);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_builder::release_tiles()
{
    optional<sprite_tiles_ptr> result;

    if(_tiles_ptr)
    {
        result = move(_tiles_ptr);
        _tiles_ptr.reset();
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->tiles().create_tiles_ptr(_graphics_index, _tiles_create_mode);
    }

    return result;
}

optional<sprite_palette_ptr> sprite_builder::release_palette()
{
    optional<sprite_palette_ptr> result;

    if(_palette_ptr)
    {
        result = move(_palette_ptr);
        _palette_ptr.reset();
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->palette().create_sprite_palette_ptr(_palette_create_mode);
    }

    return result;
}

}
