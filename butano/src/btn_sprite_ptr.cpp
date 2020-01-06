#include "btn_sprite_ptr.h"

#include "btn_size.h"
#include "btn_sprite_builder.h"
#include "btn_sprites_manager.h"

namespace btn
{

sprite_ptr sprite_ptr::create(fixed x, fixed y, const sprite_item& item, int graphics_id)
{
    return create(fixed_point(x, y), item, graphics_id);
}

sprite_ptr sprite_ptr::create(const fixed_point& position, const sprite_item& item, int graphics_id)
{
    sprite_builder builder(item, graphics_id);
    builder.set_position(position);
    return sprite_ptr(sprites_manager::create(move(builder)));
}

sprite_ptr sprite_ptr::create(const sprite_builder& builder)
{
    return sprite_ptr(sprites_manager::create(sprite_builder(builder)));
}

sprite_ptr sprite_ptr::create(sprite_builder&& builder)
{
    return sprite_ptr(sprites_manager::create(move(builder)));
}

sprite_ptr::sprite_ptr(const sprite_ptr& other) :
    sprite_ptr(other._id)
{
    sprites_manager::increase_usages(_id);
}

sprite_ptr& sprite_ptr::operator=(const sprite_ptr& other)
{
    if(_id != other._id)
    {
        _destroy();
        _id = other._id;
        sprites_manager::increase_usages(_id);
    }

    return *this;
}

sprite_ptr::sprite_ptr(sprite_ptr&& other) :
    sprite_ptr(other._id)
{
    other._id = nullptr;
}

sprite_ptr& sprite_ptr::operator=(sprite_ptr&& other)
{
    swap(_id, other._id);
    return *this;
}

size sprite_ptr::dimensions() const
{
    return sprites_manager::dimensions(_id);
}

const sprite_tiles_ptr& sprite_ptr::tiles() const
{
    return sprites_manager::tiles_ptr(_id);
}

void sprite_ptr::set_tiles(sprite_tiles_ptr tiles_ptr)
{
    sprites_manager::set_tiles_ptr(_id, move(tiles_ptr));
}

void sprite_ptr::set_tiles(const sprite_item& item, int graphics_id, create_mode create_mode)
{
    set_tiles(item.tiles_item(), graphics_id, create_mode);
}

void sprite_ptr::set_tiles(const sprite_tiles_item& tiles_item, int graphics_id, create_mode create_mode)
{
    set_tiles(tiles_item.tiles_ptr(graphics_id, create_mode));
}

const sprite_palette_ptr& sprite_ptr::palette() const
{
    return sprites_manager::palette_ptr(_id);
}

void sprite_ptr::set_palette(sprite_palette_ptr palette_ptr)
{
    sprites_manager::set_palette_ptr(_id, move(palette_ptr));
}

void sprite_ptr::set_palette(const sprite_item& item, create_mode create_mode)
{
    set_palette(item.palette_item(), create_mode);
}

void sprite_ptr::set_palette(const sprite_palette_item& palette_item, create_mode create_mode)
{
    set_palette(palette_item.palette_ptr(create_mode));
}

void sprite_ptr::set_tiles_and_palette(const sprite_item& item, int graphics_id, create_mode create_mode)
{
    set_tiles(item, graphics_id, create_mode);
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
    return sprites_manager::position(_id);
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
    sprites_manager::set_position(_id, position);
}

int sprite_ptr::bg_priority() const
{
    return sprites_manager::bg_priority(_id);
}

void sprite_ptr::set_bg_priority(int bg_priority)
{
    sprites_manager::set_bg_priority(_id, bg_priority);
}

int sprite_ptr::z_order() const
{
    return sprites_manager::z_order(_id);
}

void sprite_ptr::set_z_order(int z_order)
{
    sprites_manager::set_z_order(_id, z_order);
}

bool sprite_ptr::visible() const
{
    return sprites_manager::visible(_id);
}

void sprite_ptr::set_visible(bool visible)
{
    sprites_manager::set_visible(_id, visible);
}

bool sprite_ptr::ignore_camera() const
{
    return sprites_manager::ignore_camera(_id);
}

void sprite_ptr::set_ignore_camera(bool ignore_camera)
{
    sprites_manager::set_ignore_camera(_id, ignore_camera);
}

void sprite_ptr::_destroy()
{
    if(_id)
    {
        sprites_manager::decrease_usages(_id);
        _id = nullptr;
    }
}

}
