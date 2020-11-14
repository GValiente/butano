/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_tiles_ptr.h"

#include "btn_optional.h"
#include "btn_sprite_tiles_item.h"
#include "btn_sprite_tiles_manager.h"

namespace btn
{

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::find(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const sprite_tiles_item& tiles_item)
{
    return find(tiles_item.graphics_tiles_ref());
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const sprite_tiles_item& tiles_item, int graphics_index)
{
    return find(tiles_item.graphics_tiles_ref(graphics_index));
}

sprite_tiles_ptr sprite_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create(tiles_ref));
}

sprite_tiles_ptr sprite_tiles_ptr::create(const sprite_tiles_item& tiles_item)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create(tiles_item.graphics_tiles_ref()));
}

sprite_tiles_ptr sprite_tiles_ptr::create(const sprite_tiles_item& tiles_item, int graphics_index)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create(tiles_item.graphics_tiles_ref(graphics_index)));
}

sprite_tiles_ptr sprite_tiles_ptr::create_new(const span<const tile>& tiles_ref)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create_new(tiles_ref));
}

sprite_tiles_ptr sprite_tiles_ptr::create_new(const sprite_tiles_item& tiles_item)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create_new(tiles_item.graphics_tiles_ref()));
}

sprite_tiles_ptr sprite_tiles_ptr::create_new(const sprite_tiles_item& tiles_item, int graphics_index)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create_new(tiles_item.graphics_tiles_ref(graphics_index)));
}

sprite_tiles_ptr sprite_tiles_ptr::allocate(int tiles_count)
{
    return sprite_tiles_ptr(sprite_tiles_manager::allocate(tiles_count));
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_optional(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::create_optional(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_optional(const sprite_tiles_item& tiles_item)
{
    return create_optional(tiles_item.graphics_tiles_ref());
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_optional(const sprite_tiles_item& tiles_item, int graphics_index)
{
    return create_optional(tiles_item.graphics_tiles_ref(graphics_index));
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_new_optional(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::create_new_optional(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_new_optional(const sprite_tiles_item& tiles_item)
{
    return create_new_optional(tiles_item.graphics_tiles_ref());
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_new_optional(const sprite_tiles_item& tiles_item,
                                                                 int graphics_index)
{
    return create_new_optional(tiles_item.graphics_tiles_ref(graphics_index));
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::allocate_optional(int tiles_count)
{
    int handle = sprite_tiles_manager::allocate_optional(tiles_count);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

sprite_tiles_ptr::sprite_tiles_ptr(const sprite_tiles_ptr& other) :
    sprite_tiles_ptr(other._handle)
{
    sprite_tiles_manager::increase_usages(_handle);
}

sprite_tiles_ptr& sprite_tiles_ptr::operator=(const sprite_tiles_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle >= 0)
        {
            sprite_tiles_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        sprite_tiles_manager::increase_usages(_handle);
    }

    return *this;
}

int sprite_tiles_ptr::id() const
{
    return sprite_tiles_manager::start_tile(_handle);
}

int sprite_tiles_ptr::tiles_count() const
{
    return sprite_tiles_manager::tiles_count(_handle);
}

optional<span<const tile>> sprite_tiles_ptr::tiles_ref() const
{
    return sprite_tiles_manager::tiles_ref(_handle);
}

void sprite_tiles_ptr::set_tiles_ref(const span<const tile>& tiles_ref)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_ref);
}

void sprite_tiles_ptr::set_tiles_ref(const sprite_tiles_item& tiles_item)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_item.graphics_tiles_ref());
}

void sprite_tiles_ptr::set_tiles_ref(const sprite_tiles_item& tiles_item, int graphics_index)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_item.graphics_tiles_ref(graphics_index));
}

void sprite_tiles_ptr::reload_tiles_ref()
{
    sprite_tiles_manager::reload_tiles_ref(_handle);
}

optional<span<tile>> sprite_tiles_ptr::vram()
{
    return sprite_tiles_manager::vram(_handle);
}

void sprite_tiles_ptr::_destroy()
{
    sprite_tiles_manager::decrease_usages(_handle);
}

}
