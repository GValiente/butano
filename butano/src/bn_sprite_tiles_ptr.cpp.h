/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_tiles_ptr.h"

#include "bn_sprite_tiles_item.h"
#include "bn_sprite_tiles_manager.h"

namespace bn
{

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const sprite_tiles_item& tiles_item)
{
    int handle = sprite_tiles_manager::find(tiles_item.graphics_tiles_ref(), tiles_item.compression());
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const sprite_tiles_item& tiles_item, int graphics_index)
{
    int handle = sprite_tiles_manager::find(tiles_item.graphics_tiles_ref(graphics_index),
                                            tiles_item.compression());
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

sprite_tiles_ptr sprite_tiles_ptr::create(const sprite_tiles_item& tiles_item)
{
    int handle = sprite_tiles_manager::create(tiles_item.graphics_tiles_ref(), tiles_item.compression());
    return sprite_tiles_ptr(handle);
}

sprite_tiles_ptr sprite_tiles_ptr::create(const sprite_tiles_item& tiles_item, int graphics_index)
{
    int handle = sprite_tiles_manager::create(
            tiles_item.graphics_tiles_ref(graphics_index), tiles_item.compression());
    return sprite_tiles_ptr(handle);
}

sprite_tiles_ptr sprite_tiles_ptr::allocate(int tiles_count, bpp_mode bpp)
{
    return sprite_tiles_ptr(sprite_tiles_manager::allocate(tiles_count, bpp));
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_optional(const sprite_tiles_item& tiles_item)
{
    int handle = sprite_tiles_manager::create_optional(tiles_item.graphics_tiles_ref(), tiles_item.compression());
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::create_optional(
        const sprite_tiles_item& tiles_item, int graphics_index)
{
    int handle = sprite_tiles_manager::create_optional(
            tiles_item.graphics_tiles_ref(graphics_index), tiles_item.compression());
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::allocate_optional(int tiles_count, bpp_mode bpp)
{
    int handle = sprite_tiles_manager::allocate_optional(tiles_count, bpp);
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

sprite_tiles_ptr::~sprite_tiles_ptr()
{
    if(_handle >= 0)
    {
        sprite_tiles_manager::decrease_usages(_handle);
    }
}

int sprite_tiles_ptr::id() const
{
    return sprite_tiles_manager::start_tile(_handle);
}

int sprite_tiles_ptr::tiles_count() const
{
    return sprite_tiles_manager::tiles_count(_handle);
}

compression_type sprite_tiles_ptr::compression() const
{
    return sprite_tiles_manager::compression(_handle);
}

optional<span<const tile>> sprite_tiles_ptr::tiles_ref() const
{
    return sprite_tiles_manager::tiles_ref(_handle);
}

void sprite_tiles_ptr::set_tiles_ref(const sprite_tiles_item& tiles_item)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_item.graphics_tiles_ref(), tiles_item.compression());
}

void sprite_tiles_ptr::set_tiles_ref(const sprite_tiles_item& tiles_item, int graphics_index)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_item.graphics_tiles_ref(graphics_index),
                                        tiles_item.compression());
}

void sprite_tiles_ptr::reload_tiles_ref()
{
    sprite_tiles_manager::reload_tiles_ref(_handle);
}

optional<span<tile>> sprite_tiles_ptr::vram()
{
    return sprite_tiles_manager::vram(_handle);
}

}
