/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_tiles_ptr.h"

#include "bn_regular_bg_tiles_item.h"
#include "bn_bg_blocks_manager.h"

namespace bn
{

optional<regular_bg_tiles_ptr> regular_bg_tiles_ptr::find(const regular_bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::find_regular_tiles(tiles_item);
    optional<regular_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_tiles_ptr(handle);
    }

    return result;
}

regular_bg_tiles_ptr regular_bg_tiles_ptr::create(const regular_bg_tiles_item& tiles_item)
{
    return regular_bg_tiles_ptr(bg_blocks_manager::create_regular_tiles(tiles_item, false));
}

regular_bg_tiles_ptr regular_bg_tiles_ptr::allocate(int tiles_count, bpp_mode bpp)
{
    return regular_bg_tiles_ptr(bg_blocks_manager::allocate_regular_tiles(tiles_count, bpp, false));
}

optional<regular_bg_tiles_ptr> regular_bg_tiles_ptr::create_optional(const regular_bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::create_regular_tiles(tiles_item, true);
    optional<regular_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_tiles_ptr(handle);
    }

    return result;
}

optional<regular_bg_tiles_ptr> regular_bg_tiles_ptr::allocate_optional(int tiles_count, bpp_mode bpp)
{
    int handle = bg_blocks_manager::allocate_regular_tiles(tiles_count, bpp, true);
    optional<regular_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_tiles_ptr(handle);
    }

    return result;
}

regular_bg_tiles_ptr::regular_bg_tiles_ptr(const regular_bg_tiles_ptr& other) :
    regular_bg_tiles_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

regular_bg_tiles_ptr& regular_bg_tiles_ptr::operator=(const regular_bg_tiles_ptr& other)
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

regular_bg_tiles_ptr::~regular_bg_tiles_ptr()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
    }
}

int regular_bg_tiles_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

int regular_bg_tiles_ptr::cbb() const
{
    return bg_blocks_manager::hw_tiles_cbb(_handle);
}

int regular_bg_tiles_ptr::tiles_count() const
{
    return bg_blocks_manager::tiles_count(_handle);
}

compression_type regular_bg_tiles_ptr::compression() const
{
    return bg_blocks_manager::compression(_handle);
}

optional<span<const tile>> regular_bg_tiles_ptr::tiles_ref() const
{
    return bg_blocks_manager::tiles_ref(_handle);
}

void regular_bg_tiles_ptr::set_tiles_ref(const regular_bg_tiles_item& tiles_item)
{
    bg_blocks_manager::set_regular_tiles_ref(_handle, tiles_item);
}

void regular_bg_tiles_ptr::reload_tiles_ref()
{
    bg_blocks_manager::reload(_handle);
}

optional<span<tile>> regular_bg_tiles_ptr::vram()
{
    return bg_blocks_manager::tiles_vram(_handle);
}

}
