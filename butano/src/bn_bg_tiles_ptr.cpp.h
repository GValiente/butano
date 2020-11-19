/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_tiles_ptr.h"

#include "bn_optional.h"
#include "bn_bg_tiles_item.h"
#include "bn_bg_blocks_manager.h"

namespace bn
{

optional<bg_tiles_ptr> bg_tiles_ptr::find(const bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::find_tiles(tiles_item);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

bg_tiles_ptr bg_tiles_ptr::create(const bg_tiles_item& tiles_item)
{
    return bg_tiles_ptr(bg_blocks_manager::create_tiles(tiles_item));
}

bg_tiles_ptr bg_tiles_ptr::create_new(const bg_tiles_item& tiles_item)
{
    return bg_tiles_ptr(bg_blocks_manager::create_new_tiles(tiles_item));
}

bg_tiles_ptr bg_tiles_ptr::allocate(int tiles_count, bpp_mode bpp)
{
    return bg_tiles_ptr(bg_blocks_manager::allocate_tiles(tiles_count, bpp));
}

optional<bg_tiles_ptr> bg_tiles_ptr::create_optional(const bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::create_tiles_optional(tiles_item);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::create_new_optional(const bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::create_new_tiles_optional(tiles_item);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::allocate_optional(int tiles_count, bpp_mode bpp)
{
    int handle = bg_blocks_manager::allocate_tiles_optional(tiles_count, bpp);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

bg_tiles_ptr::bg_tiles_ptr(const bg_tiles_ptr& other) :
    bg_tiles_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

bg_tiles_ptr& bg_tiles_ptr::operator=(const bg_tiles_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle >= 0)
        {
            bg_blocks_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        bg_blocks_manager::increase_usages(_handle);
    }

    return *this;
}

int bg_tiles_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

int bg_tiles_ptr::cbb() const
{
    return bg_blocks_manager::hw_tiles_cbb(_handle);
}

int bg_tiles_ptr::tiles_count() const
{
    return bg_blocks_manager::tiles_count(_handle);
}

optional<span<const tile>> bg_tiles_ptr::tiles_ref() const
{
    return bg_blocks_manager::tiles_ref(_handle);
}

void bg_tiles_ptr::set_tiles_ref(const bg_tiles_item& tiles_item)
{
    bg_blocks_manager::set_tiles_ref(_handle, tiles_item);
}

void bg_tiles_ptr::reload_tiles_ref()
{
    bg_blocks_manager::reload(_handle);
}

optional<span<tile>> bg_tiles_ptr::vram()
{
    return bg_blocks_manager::tiles_vram(_handle);
}

void bg_tiles_ptr::_destroy()
{
    bg_blocks_manager::decrease_usages(_handle);
}

}
