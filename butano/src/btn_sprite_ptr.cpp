#include "btn_sprite_ptr.h"

#include "btn_size.h"
#include "btn_sprite_builder.h"
#include "btn_regular_bg_ptr.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprite_affine_mat_builder.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprite_regular_second_attributes.h"

namespace btn
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
    BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
               "Invalid tiles ptr size: ", tiles.tiles_count(), " - ",
               shape_size.tiles_count(palette.bpp_mode()));

    return sprite_ptr(sprites_manager::create(fixed_point(x, y), shape_size, move(tiles), move(palette)));
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr tiles,
                              sprite_palette_ptr palette)
{
    BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
               "Invalid tiles ptr size: ", tiles.tiles_count(), " - ",
               shape_size.tiles_count(palette.bpp_mode()));

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

optional<sprite_ptr> sprite_ptr::optional_create(fixed x, fixed y, const sprite_item& item)
{
    return optional_create(fixed_point(x, y), item);
}

optional<sprite_ptr> sprite_ptr::optional_create(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    return optional_create(fixed_point(x, y), item, graphics_index);
}

optional<sprite_ptr> sprite_ptr::optional_create(const fixed_point& position, const sprite_item& item)
{
    optional<sprite_ptr> result;

    if(optional<sprite_tiles_ptr> tiles = item.tiles_item().create_tiles())
    {
        if(optional<sprite_palette_ptr> palette = item.palette_item().create_palette())
        {
            if(handle_type handle = sprites_manager::optional_create(position, item.shape_size(), move(*tiles),
                                                                     move(*palette)))
            {
                result = sprite_ptr(handle);
            }
        }
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(const fixed_point& position, const sprite_item& item,
                                                 int graphics_index)
{
    optional<sprite_ptr> result;

    if(optional<sprite_tiles_ptr> tiles = item.tiles_item().create_tiles(graphics_index))
    {
        if(optional<sprite_palette_ptr> palette = item.palette_item().create_palette())
        {
            if(handle_type handle = sprites_manager::optional_create(position, item.shape_size(), move(*tiles),
                                                                     move(*palette)))
            {
                result = sprite_ptr(handle);
            }
        }
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(fixed x, fixed y, const sprite_shape_size& shape_size,
                                                 sprite_tiles_ptr tiles, sprite_palette_ptr palette)
{
    BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
               "Invalid tiles ptr size: ", tiles.tiles_count(), " - ",
               shape_size.tiles_count(palette.bpp_mode()));

    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::optional_create(fixed_point(x, y), shape_size, move(tiles), move(palette)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(const fixed_point& position, const sprite_shape_size& shape_size,
                                                 sprite_tiles_ptr tiles, sprite_palette_ptr palette)
{
    BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
               "Invalid tiles ptr size: ", tiles.tiles_count(), " - ",
               shape_size.tiles_count(palette.bpp_mode()));

    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::optional_create(position, shape_size, move(tiles), move(palette)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(const sprite_builder& builder)
{
    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::optional_create(sprite_builder(builder)))
    {
        result = sprite_ptr(handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(sprite_builder&& builder)
{
    optional<sprite_ptr> result;

    if(handle_type handle = sprites_manager::optional_create(move(builder)))
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

void sprite_ptr::set_tiles(const sprite_tiles_ptr& tiles_ptr)
{
    sprites_manager::set_tiles(_handle, tiles_ptr);
}

void sprite_ptr::set_tiles(sprite_tiles_ptr&& tiles_ptr)
{
    sprites_manager::set_tiles(_handle, move(tiles_ptr));
}

void sprite_ptr::set_tiles(const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles_ptr)
{
    sprites_manager::set_tiles(_handle, shape_size, tiles_ptr);
}

void sprite_ptr::set_tiles(const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles_ptr)
{
    sprites_manager::set_tiles(_handle, shape_size, move(tiles_ptr));
}

void sprite_ptr::set_tiles(const sprite_item& item)
{
    set_tiles(item.tiles_item(), item.shape_size());
}

void sprite_ptr::set_tiles(const sprite_item& item, int graphics_index)
{
    set_tiles(item.tiles_item(), item.shape_size(), graphics_index);
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item)
{
    set_tiles(tiles_item.create_tiles());
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, int graphics_index)
{
    set_tiles(tiles_item.create_tiles(graphics_index));
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size)
{
    set_tiles(shape_size, tiles_item.create_tiles());
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, const sprite_shape_size& shape_size,
                           int graphics_index)
{
    set_tiles(shape_size, tiles_item.create_tiles(graphics_index));
}

const sprite_palette_ptr& sprite_ptr::palette() const
{
    return sprites_manager::palette(_handle);
}

void sprite_ptr::set_palette(const sprite_palette_ptr& palette_ptr)
{
    sprites_manager::set_palette(_handle, palette_ptr);
}

void sprite_ptr::set_palette(sprite_palette_ptr&& palette_ptr)
{
    sprites_manager::set_palette(_handle, move(palette_ptr));
}

void sprite_ptr::set_palette(const sprite_item& item)
{
    set_palette(item.palette_item());
}

void sprite_ptr::set_palette(const sprite_palette_item& palette_item)
{
    set_palette(palette_item.create_palette());
}

void sprite_ptr::set_item(const sprite_item& item)
{
    sprites_manager::set_tiles_and_palette(_handle, item.shape_size(), item.tiles_item().create_tiles(),
                                           item.palette_item().create_palette());
}

void sprite_ptr::set_item(const sprite_item& item, int graphics_index)
{
    sprites_manager::set_tiles_and_palette(_handle, item.shape_size(), item.tiles_item().create_tiles(graphics_index),
                                           item.palette_item().create_palette());
}

fixed sprite_ptr::x() const
{
    return position().x();
}

fixed sprite_ptr::y() const
{
    return position().y();
}

const fixed_point& sprite_ptr::position() const
{
    return sprites_manager::position(_handle);
}

void sprite_ptr::set_x(fixed x)
{
    set_position(fixed_point(x, position().y()));
}

void sprite_ptr::set_y(fixed y)
{
    set_position(fixed_point(position().x(), y));
}

void sprite_ptr::set_position(fixed x, fixed y)
{
    set_position(fixed_point(x, y));
}

void sprite_ptr::set_position(const fixed_point& position)
{
    sprites_manager::set_position(_handle, position);
}

fixed sprite_ptr::rotation_angle() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        return affine_mat->rotation_angle();
    }

    return 0;
}

void sprite_ptr::set_rotation_angle(fixed rotation_angle)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_rotation_angle(rotation_angle);

        if(remove_affine_mat_when_not_needed() && affine_mat->identity())
        {
            set_affine_mat(nullopt);
        }
    }
    else if(rotation_angle != 0)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_rotation_angle(rotation_angle);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(sprite_affine_mat_ptr::create(move(affine_mat_builder)));
    }
}

fixed sprite_ptr::scale_x() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        return affine_mat->scale_x();
    }

    return 1;
}

void sprite_ptr::set_scale_x(fixed scale_x)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale_x(scale_x);

        if(remove_affine_mat_when_not_needed() && affine_mat->identity())
        {
            set_affine_mat(nullopt);
        }
    }
    else if(scale_x != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_x(scale_x);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(sprite_affine_mat_ptr::create(move(affine_mat_builder)));
    }
}

fixed sprite_ptr::scale_y() const
{
    if(const optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        return affine_mat->scale_y();
    }

    return 1;
}

void sprite_ptr::set_scale_y(fixed scale_y)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale_y(scale_y);

        if(remove_affine_mat_when_not_needed() && affine_mat->identity())
        {
            set_affine_mat(nullopt);
        }
    }
    else if(scale_y != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale_y(scale_y);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(sprite_affine_mat_ptr::create(move(affine_mat_builder)));
    }
}

void sprite_ptr::set_scale(fixed scale)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale(scale);

        if(remove_affine_mat_when_not_needed() && affine_mat->identity())
        {
            set_affine_mat(nullopt);
        }
    }
    else if(scale != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale(scale);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(sprite_affine_mat_ptr::create(move(affine_mat_builder)));
    }
}

void sprite_ptr::set_scale(fixed scale_x, fixed scale_y)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale(scale_x, scale_y);

        if(remove_affine_mat_when_not_needed() && affine_mat->identity())
        {
            set_affine_mat(nullopt);
        }
    }
    else if(scale_x != 1 || scale_y != 1)
    {
        sprite_affine_mat_builder affine_mat_builder;
        affine_mat_builder.set_scale(scale_x, scale_y);
        affine_mat_builder.set_horizontal_flip(horizontal_flip());
        affine_mat_builder.set_vertical_flip(vertical_flip());
        set_remove_affine_mat_when_not_needed(true);
        set_affine_mat(sprite_affine_mat_ptr::create(move(affine_mat_builder)));
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

optional<bool> sprite_ptr::above(const sprite_ptr& other) const
{
    if(*this == other)
    {
        return false;
    }

    int this_priority = bg_priority();
    int other_priority = other.bg_priority();

    if(this_priority < other_priority)
    {
        return true;
    }

    if(this_priority > other_priority)
    {
        return false;
    }

    int this_z_order = z_order();
    int other_z_order = other.z_order();

    if(this_z_order < other_z_order)
    {
        return true;
    }

    if(this_z_order > other_z_order)
    {
        return false;
    }

    return nullopt;
}

bool sprite_ptr::above(const regular_bg_ptr& bg_ptr) const
{
    return bg_priority() <= bg_ptr.priority();
}

void sprite_ptr::put_above(const sprite_ptr& other)
{
    if(*this == other)
    {
        return;
    }

    int this_priority = bg_priority();
    int other_priority = other.bg_priority();

    if(this_priority < other_priority)
    {
        return;
    }

    if(this_priority > other_priority)
    {
        set_bg_priority(other_priority);
    }

    int this_z_order = z_order();
    int other_z_order = other.z_order();

    if(this_z_order < other_z_order)
    {
        return;
    }

    if(this_z_order > other_z_order)
    {
        set_z_order(other_z_order);
    }

    sprites_manager::put_in_front_of_sort_layer(_handle);
}

void sprite_ptr::put_above(const regular_bg_ptr& bg_ptr)
{
    int this_priority = bg_priority();
    int bg_priority = bg_ptr.priority();

    if(this_priority > bg_priority)
    {
        set_bg_priority(bg_priority);
    }
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

bool sprite_ptr::ignore_camera() const
{
    return sprites_manager::ignore_camera(_handle);
}

void sprite_ptr::set_ignore_camera(bool ignore_camera)
{
    sprites_manager::set_ignore_camera(_handle, ignore_camera);
}

const optional<sprite_affine_mat_ptr>& sprite_ptr::affine_mat() const
{
    return sprites_manager::affine_mat(_handle);
}

void sprite_ptr::set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat_ptr)
{
    sprites_manager::set_affine_mat(_handle, affine_mat_ptr);
}

void sprite_ptr::set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat_ptr)
{
    sprites_manager::set_affine_mat(_handle, move(affine_mat_ptr));
}

bool sprite_ptr::remove_affine_mat_when_not_needed() const
{
    return sprites_manager::remove_affine_mat_when_not_needed(_handle);
}

void sprite_ptr::set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)
{
    sprites_manager::set_remove_affine_mat_when_not_needed(_handle, remove_when_not_needed);
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

void sprite_ptr::_destroy()
{
    sprites_manager::decrease_usages(_handle);
}

}
