/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_tiles_ptr.h"

#include "bn_affine_bg_tiles_item.h"
#include "bn_bg_blocks_manager.h"

namespace bn
{

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::find(const affine_bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::find_affine_tiles(tiles_item);
    optional<affine_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_tiles_ptr(handle);
    }

    return result;
}

affine_bg_tiles_ptr affine_bg_tiles_ptr::create(const affine_bg_tiles_item& tiles_item)
{
    return affine_bg_tiles_ptr(bg_blocks_manager::create_affine_tiles(tiles_item, false));
}

affine_bg_tiles_ptr affine_bg_tiles_ptr::allocate(int tiles_count)
{
    return affine_bg_tiles_ptr(bg_blocks_manager::allocate_affine_tiles(tiles_count, false));
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::create_optional(const affine_bg_tiles_item& tiles_item)
{
    int handle = bg_blocks_manager::create_affine_tiles(tiles_item, true);
    optional<affine_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_tiles_ptr(handle);
    }

    return result;
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::allocate_optional(int tiles_count)
{
    int handle = bg_blocks_manager::allocate_affine_tiles(tiles_count, true);
    optional<affine_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_tiles_ptr(handle);
    }

    return result;
}

affine_bg_tiles_ptr::affine_bg_tiles_ptr(const affine_bg_tiles_ptr& other) :
    affine_bg_tiles_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

affine_bg_tiles_ptr& affine_bg_tiles_ptr::operator=(const affine_bg_tiles_ptr& other)
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

affine_bg_tiles_ptr::~affine_bg_tiles_ptr()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
    }
}

int affine_bg_tiles_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

int affine_bg_tiles_ptr::cbb() const
{
    return bg_blocks_manager::hw_tiles_cbb(_handle);
}

int affine_bg_tiles_ptr::tiles_count() const
{
    return bg_blocks_manager::tiles_count(_handle);
}

compression_type affine_bg_tiles_ptr::compression() const
{
    return bg_blocks_manager::compression(_handle);
}

optional<span<const tile>> affine_bg_tiles_ptr::tiles_ref() const
{
    return bg_blocks_manager::tiles_ref(_handle);
}

void affine_bg_tiles_ptr::set_tiles_ref(const affine_bg_tiles_item& tiles_item)
{
    bg_blocks_manager::set_affine_tiles_ref(_handle, tiles_item);
}

void affine_bg_tiles_ptr::reload_tiles_ref()
{
    bg_blocks_manager::reload(_handle);
}

optional<span<tile>> affine_bg_tiles_ptr::vram()
{
    return bg_blocks_manager::tiles_vram(_handle);
}

}
