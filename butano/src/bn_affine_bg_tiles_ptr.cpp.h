/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
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
    return create(tiles_item, bg_blocks_manager::allow_tiles_offset());
}

affine_bg_tiles_ptr affine_bg_tiles_ptr::create(const affine_bg_tiles_item& tiles_item, bool allow_offset)
{
    return affine_bg_tiles_ptr(bg_blocks_manager::create_affine_tiles(tiles_item, allow_offset, false));
}

affine_bg_tiles_ptr affine_bg_tiles_ptr::allocate(int tiles_count)
{
    return allocate(tiles_count, bg_blocks_manager::allow_tiles_offset());
}

affine_bg_tiles_ptr affine_bg_tiles_ptr::allocate(int tiles_count, bool allow_offset)
{
    return affine_bg_tiles_ptr(bg_blocks_manager::allocate_affine_tiles(tiles_count, allow_offset, false));
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::create_optional(const affine_bg_tiles_item& tiles_item)
{
    return create_optional(tiles_item, bg_blocks_manager::allow_tiles_offset());
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::create_optional(
        const affine_bg_tiles_item& tiles_item, bool allow_offset)
{
    int handle = bg_blocks_manager::create_affine_tiles(tiles_item, allow_offset, true);
    optional<affine_bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_tiles_ptr(handle);
    }

    return result;
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::allocate_optional(int tiles_count)
{
    return allocate_optional(tiles_count, bg_blocks_manager::allow_tiles_offset());
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_ptr::allocate_optional(int tiles_count, bool allow_offset)
{
    int handle = bg_blocks_manager::allocate_affine_tiles(tiles_count, allow_offset, true);
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

int affine_bg_tiles_ptr::offset() const
{
    return bg_blocks_manager::affine_tiles_offset(_handle);
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

void affine_bg_tiles_ptr::overwrite_tile(int tile_index, const tile& tiles_ref)
{
    bg_blocks_manager::overwrite_tile(_handle, tile_index, tiles_ref);
}

optional<span<const tile>> affine_bg_tiles_ptr::vram() const
{
    optional<span<tile>> vram_opt = bg_blocks_manager::tiles_vram(_handle);
    optional<span<const tile>> result;

    if(span<tile>* vram = vram_opt.get())
    {
        result = span<const tile>(vram->data(), vram->size());
    }

    return result;
}

optional<span<tile>> affine_bg_tiles_ptr::vram()
{
    return bg_blocks_manager::tiles_vram(_handle);
}

}
