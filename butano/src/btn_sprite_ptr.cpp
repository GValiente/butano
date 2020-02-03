#include "btn_sprite_ptr.h"

#include "btn_size.h"
#include "btn_sprite_builder.h"
#include "btn_regular_bg_ptr.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_affine_mat_builder.h"

namespace btn
{

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    sprite_builder builder(item, graphics_index);
    builder.set_position(fixed_point(x, y));

    optional<handle_type> handle = sprites_manager::create(move(builder));
    BTN_ASSERT(handle, "Sprite create failed");

    return sprite_ptr(*handle);
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_item& item, int graphics_index)
{
    sprite_builder builder(item, graphics_index);
    builder.set_position(position);

    optional<handle_type> handle = sprites_manager::create(move(builder));
    BTN_ASSERT(handle, "Sprite create failed");

    return sprite_ptr(*handle);
}

sprite_ptr sprite_ptr::create(const sprite_builder& builder)
{
    optional<handle_type> handle = sprites_manager::create(sprite_builder(builder));
    BTN_ASSERT(handle, "Sprite create failed");

    return sprite_ptr(*handle);
}

sprite_ptr sprite_ptr::create(sprite_builder&& builder)
{
    optional<handle_type> handle = sprites_manager::create(move(builder));
    BTN_ASSERT(handle, "Sprite create failed");

    return sprite_ptr(*handle);
}

optional<sprite_ptr> sprite_ptr::optional_create(fixed x, fixed y, const sprite_item& item, int graphics_index)
{
    optional<sprite_ptr> result;
    sprite_builder builder(item, graphics_index);
    builder.set_position(fixed_point(x, y));

    if(optional<handle_type> handle = sprites_manager::create(move(builder)))
    {
        result = sprite_ptr(*handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(const fixed_point& position, const sprite_item& item,
                                                 int graphics_index)
{
    optional<sprite_ptr> result;
    sprite_builder builder(item, graphics_index);
    builder.set_position(position);

    if(optional<handle_type> handle = sprites_manager::create(move(builder)))
    {
        result = sprite_ptr(*handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(const sprite_builder& builder)
{
    optional<sprite_ptr> result;

    if(optional<handle_type> handle = sprites_manager::create(sprite_builder(builder)))
    {
        result = sprite_ptr(*handle);
    }

    return result;
}

optional<sprite_ptr> sprite_ptr::optional_create(sprite_builder&& builder)
{
    optional<sprite_ptr> result;

    if(optional<handle_type> handle = sprites_manager::create(move(builder)))
    {
        result = sprite_ptr(*handle);
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

void sprite_ptr::set_tiles(const sprite_item& item, int graphics_index, create_mode create_mode)
{
    set_tiles(item.tiles_item(), graphics_index, create_mode);
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, int graphics_index, create_mode create_mode)
{
    optional<sprite_tiles_ptr> tiles_ptr = tiles_item.create_tiles(graphics_index, create_mode);
    BTN_ASSERT(tiles_ptr, "Tiles create failed");

    set_tiles(move(*tiles_ptr));
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

void sprite_ptr::set_palette(const sprite_item& item, create_mode create_mode)
{
    set_palette(item.palette_item(), create_mode);
}

void sprite_ptr::set_palette(const sprite_palette_item& palette_item, create_mode create_mode)
{
    optional<sprite_palette_ptr> palette_ptr = palette_item.create_palette(create_mode);
    BTN_ASSERT(palette_ptr, "Palette create failed");

    set_palette(move(*palette_ptr));
}

void sprite_ptr::set_item(const sprite_item& item, int graphics_index, create_mode create_mode)
{
    set_tiles(item, graphics_index, create_mode);
    set_palette(item, create_mode);
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

    return 0;
}

void sprite_ptr::set_scale_x(fixed scale_x)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale_x(scale_x);
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

    return 0;
}

void sprite_ptr::set_scale_y(fixed scale_y)
{
    if(optional<sprite_affine_mat_ptr>& affine_mat = sprites_manager::affine_mat(_handle))
    {
        affine_mat->set_scale_y(scale_y);
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

void sprite_ptr::put_above(const sprite_ptr& other)
{
    if(*this != other)
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
    else
    {
        sprites_manager::put_in_front_of_sort_layer(_handle);
    }
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

void sprite_ptr::_destroy()
{
    sprites_manager::decrease_usages(_handle);
}

}
