/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_map_ptr.h"

#include "bn_bg_palette_ptr.h"
#include "bn_affine_bg_item.h"
#include "bn_bg_blocks_manager.h"
#include "bn_affine_bg_tiles_ptr.h"

namespace bn
{

optional<affine_bg_map_ptr> affine_bg_map_ptr::find(
        const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
{
    int handle = bg_blocks_manager::find_affine_map(map_item, map_item.cells_ptr(), tiles, palette);
    optional<affine_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_map_ptr(handle);
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::find(
        const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette,
        int map_index)
{
    int handle = bg_blocks_manager::find_affine_map(map_item, map_item.cells_ptr(map_index), tiles, palette);
    optional<affine_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_map_ptr(handle);
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::find(const affine_bg_item& item)
{
    optional<affine_bg_tiles_ptr> tiles = affine_bg_tiles_ptr::find(item.tiles_item());
    optional<affine_bg_map_ptr> result;

    if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::find(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            result = find(item.map_item(), *tiles_ptr, *palette_ptr);
        }
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::find(const affine_bg_item& item, int map_index)
{
    optional<affine_bg_tiles_ptr> tiles = affine_bg_tiles_ptr::find(item.tiles_item());
    optional<affine_bg_map_ptr> result;

    if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::find(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            result = find(item.map_item(), *tiles_ptr, *palette_ptr, map_index);
        }
    }

    return result;
}

affine_bg_map_ptr affine_bg_map_ptr::create(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), false);
    return affine_bg_map_ptr(handle);
}

affine_bg_map_ptr affine_bg_map_ptr::create(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), false);
    return affine_bg_map_ptr(handle);
}

affine_bg_map_ptr affine_bg_map_ptr::create(const affine_bg_item& item)
{
    const affine_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(), affine_bg_tiles_ptr::create(item.tiles_item()),
                bg_palette_ptr::create(item.palette_item()), false);
    return affine_bg_map_ptr(handle);
}

affine_bg_map_ptr affine_bg_map_ptr::create(const affine_bg_item& item, int map_index)
{
    const affine_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(map_index), affine_bg_tiles_ptr::create(item.tiles_item()),
                bg_palette_ptr::create(item.palette_item()), false);
    return affine_bg_map_ptr(handle);
}

affine_bg_map_ptr affine_bg_map_ptr::create_new(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    return create(map_item, move(tiles), move(palette));
}

affine_bg_map_ptr affine_bg_map_ptr::create_new(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    return create(map_item, move(tiles), move(palette), map_index);
}

affine_bg_map_ptr affine_bg_map_ptr::allocate(
        const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_affine_map(dimensions, move(tiles), move(palette), false);
    return affine_bg_map_ptr(handle);
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_optional(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), true);
    optional<affine_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_map_ptr(handle);
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_optional(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_affine_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), true);
    optional<affine_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_map_ptr(handle);
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_optional(const affine_bg_item& item)
{
    optional<affine_bg_tiles_ptr> tiles = affine_bg_tiles_ptr::create_optional(item.tiles_item());
    optional<affine_bg_map_ptr> result;

    if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::create_optional(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const affine_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_affine_map(
                        map_item, map_item.cells_ptr(), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = affine_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_optional(const affine_bg_item& item, int map_index)
{
    optional<affine_bg_tiles_ptr> tiles = affine_bg_tiles_ptr::create_optional(item.tiles_item());
    optional<affine_bg_map_ptr> result;

    if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::create_optional(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const affine_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_affine_map(
                        map_item, map_item.cells_ptr(map_index), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = affine_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_new_optional(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    return create_optional(map_item, move(tiles), move(palette));
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::create_new_optional(
        const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    return create_optional(map_item, move(tiles), move(palette), map_index);
}

optional<affine_bg_map_ptr> affine_bg_map_ptr::allocate_optional(
        const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_affine_map(dimensions, move(tiles), move(palette), true);
    optional<affine_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = affine_bg_map_ptr(handle);
    }

    return result;
}

affine_bg_map_ptr::affine_bg_map_ptr(const affine_bg_map_ptr& other) :
    affine_bg_map_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

affine_bg_map_ptr& affine_bg_map_ptr::operator=(const affine_bg_map_ptr& other)
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

affine_bg_map_ptr::~affine_bg_map_ptr()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
    }
}

int affine_bg_map_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

size affine_bg_map_ptr::dimensions() const
{
    return bg_blocks_manager::map_dimensions(_handle);
}

bool affine_bg_map_ptr::big() const
{
    return bg_blocks_manager::big_map(_handle);
}

affine_bg_big_map_canvas_size affine_bg_map_ptr::big_canvas_size() const
{
    return bg_blocks_manager::affine_big_map_canvas_size(_handle);
}

int affine_bg_map_ptr::tiles_offset() const
{
    return bg_blocks_manager::affine_tiles_offset(_handle);
}

compression_type affine_bg_map_ptr::compression() const
{
    return bg_blocks_manager::compression(_handle);
}

optional<span<const affine_bg_map_cell>> affine_bg_map_ptr::cells_ref() const
{
    return bg_blocks_manager::affine_map_cells_ref(_handle);
}

void affine_bg_map_ptr::set_cells_ref(const affine_bg_map_item& map_item)
{
    bg_blocks_manager::set_affine_map_cells_ref(_handle, map_item, map_item.cells_ptr());
}

void affine_bg_map_ptr::set_cells_ref(const affine_bg_map_item& map_item, int map_index)
{
    bg_blocks_manager::set_affine_map_cells_ref(_handle, map_item, map_item.cells_ptr(map_index));
}

void affine_bg_map_ptr::reload_cells_ref()
{
    bg_blocks_manager::reload(_handle);
}

const affine_bg_tiles_ptr& affine_bg_map_ptr::tiles() const
{
    return bg_blocks_manager::affine_map_tiles(_handle);
}

void affine_bg_map_ptr::set_tiles(const affine_bg_tiles_ptr& tiles)
{
    bg_blocks_manager::set_affine_map_tiles(_handle, affine_bg_tiles_ptr(tiles));
}

void affine_bg_map_ptr::set_tiles(affine_bg_tiles_ptr&& tiles)
{
    bg_blocks_manager::set_affine_map_tiles(_handle, move(tiles));
}

void affine_bg_map_ptr::set_tiles(const affine_bg_tiles_item& tiles_item)
{
    optional<affine_bg_tiles_ptr> tiles = tiles_item.find_tiles();

    if(affine_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        bg_blocks_manager::set_affine_map_tiles(_handle, move(*tiles_ptr));
    }
    else
    {
        bg_blocks_manager::remove_affine_map_tiles(_handle);
        bg_blocks_manager::set_affine_map_tiles(_handle, affine_bg_tiles_ptr::create(tiles_item));
    }
}

const bg_palette_ptr& affine_bg_map_ptr::palette() const
{
    return bg_blocks_manager::map_palette(_handle);
}

void affine_bg_map_ptr::set_palette(const bg_palette_ptr& palette)
{
    bg_blocks_manager::set_affine_map_palette(_handle, bg_palette_ptr(palette));
}

void affine_bg_map_ptr::set_palette(bg_palette_ptr&& palette)
{
    bg_blocks_manager::set_affine_map_palette(_handle, move(palette));
}

void affine_bg_map_ptr::set_palette(const bg_palette_item& palette_item)
{
    bg_blocks_manager::set_affine_map_palette(_handle, bg_palette_ptr::create(palette_item));
}

optional<span<affine_bg_map_cell>> affine_bg_map_ptr::vram()
{
    return bg_blocks_manager::affine_map_vram(_handle);
}

}
