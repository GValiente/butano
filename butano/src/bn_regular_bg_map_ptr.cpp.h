/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_map_ptr.h"

#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_bg_blocks_manager.h"
#include "bn_regular_bg_tiles_ptr.h"

namespace bn
{

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(
        const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
{
    int handle = bg_blocks_manager::find_regular_map(map_item, map_item.cells_ptr(), tiles, palette);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(
        const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette,
        int map_index)
{
    int handle = bg_blocks_manager::find_regular_map(map_item, map_item.cells_ptr(map_index), tiles, palette);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(const regular_bg_item& item)
{
    optional<regular_bg_tiles_ptr> tiles = regular_bg_tiles_ptr::find(item.tiles_item());
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::find(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            result = find(item.map_item(), *tiles_ptr, *palette_ptr);
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(const regular_bg_item& item, int map_index)
{
    optional<regular_bg_tiles_ptr> tiles = regular_bg_tiles_ptr::find(item.tiles_item());
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = bg_palette_ptr::find(item.palette_item());

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            result = find(item.map_item(), *tiles_ptr, *palette_ptr, map_index);
        }
    }

    return result;
}

regular_bg_map_ptr regular_bg_map_ptr::create(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(const regular_bg_item& item)
{
    const regular_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(), item.tiles_item().create_tiles(),
                item.palette_item().create_palette(), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(const regular_bg_item& item, int map_index)
{
    const regular_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(map_index), item.tiles_item().create_tiles(),
                item.palette_item().create_palette(), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create_new(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create_new(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create_new(const regular_bg_item& item)
{
    const regular_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(), item.tiles_item().create_tiles(),
                item.palette_item().create_palette(), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create_new(const regular_bg_item& item, int map_index)
{
    const regular_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(map_index), item.tiles_item().create_tiles(),
                item.palette_item().create_palette(), false);
    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::allocate(
        const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_regular_map(dimensions, move(tiles), move(palette), false);
    return regular_bg_map_ptr(handle);
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), true);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), true);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(const regular_bg_item& item)
{
    optional<regular_bg_tiles_ptr> tiles = item.tiles_item().create_tiles_optional();
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = item.palette_item().create_palette_optional();

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const regular_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_regular_map(
                        map_item, map_item.cells_ptr(), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = regular_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(const regular_bg_item& item, int map_index)
{
    optional<regular_bg_tiles_ptr> tiles = item.tiles_item().create_tiles_optional();
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = item.palette_item().create_palette_optional();

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const regular_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_regular_map(
                        map_item, map_item.cells_ptr(map_index), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = regular_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_new_optional(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(), move(tiles), move(palette), true);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_new_optional(
        const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index)
{
    int handle = bg_blocks_manager::create_new_regular_map(
                map_item, map_item.cells_ptr(map_index), move(tiles), move(palette), true);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_new_optional(const regular_bg_item& item)
{
    optional<regular_bg_tiles_ptr> tiles = item.tiles_item().create_tiles_optional();
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = item.palette_item().create_palette_optional();

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const regular_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_new_regular_map(
                        map_item, map_item.cells_ptr(), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = regular_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_new_optional(const regular_bg_item& item, int map_index)
{
    optional<regular_bg_tiles_ptr> tiles = item.tiles_item().create_tiles_optional();
    optional<regular_bg_map_ptr> result;

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        optional<bg_palette_ptr> palette = item.palette_item().create_palette_optional();

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            const regular_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_new_regular_map(
                        map_item, map_item.cells_ptr(map_index), move(*tiles_ptr), move(*palette_ptr), true);

            if(handle >= 0)
            {
                result = regular_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::allocate_optional(
        const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_regular_map(dimensions, move(tiles), move(palette), true);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

regular_bg_map_ptr::regular_bg_map_ptr(const regular_bg_map_ptr& other) :
    regular_bg_map_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

regular_bg_map_ptr& regular_bg_map_ptr::operator=(const regular_bg_map_ptr& other)
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

regular_bg_map_ptr::~regular_bg_map_ptr()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
    }
}

int regular_bg_map_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

size regular_bg_map_ptr::dimensions() const
{
    return bg_blocks_manager::map_dimensions(_handle);
}

bool regular_bg_map_ptr::big() const
{
    return bg_blocks_manager::big_map(_handle);
}

bpp_mode regular_bg_map_ptr::bpp() const
{
    return palette().bpp();
}

int regular_bg_map_ptr::tiles_offset() const
{
    return bg_blocks_manager::regular_tiles_offset(_handle);
}

int regular_bg_map_ptr::palette_banks_offset() const
{
    return bg_blocks_manager::palette_offset(_handle);
}

compression_type regular_bg_map_ptr::compression() const
{
    return bg_blocks_manager::compression(_handle);
}

optional<span<const regular_bg_map_cell>> regular_bg_map_ptr::cells_ref() const
{
    return bg_blocks_manager::regular_map_cells_ref(_handle);
}

void regular_bg_map_ptr::set_cells_ref(const regular_bg_map_item& map_item)
{
    bg_blocks_manager::set_regular_map_cells_ref(_handle, map_item, map_item.cells_ptr());
}

void regular_bg_map_ptr::set_cells_ref(const regular_bg_map_item& map_item, int map_index)
{
    bg_blocks_manager::set_regular_map_cells_ref(_handle, map_item, map_item.cells_ptr(map_index));
}

void regular_bg_map_ptr::reload_cells_ref()
{
    bg_blocks_manager::reload(_handle);
}

const regular_bg_tiles_ptr& regular_bg_map_ptr::tiles() const
{
    return bg_blocks_manager::regular_map_tiles(_handle);
}

void regular_bg_map_ptr::set_tiles(const regular_bg_tiles_ptr& tiles)
{
    bg_blocks_manager::set_regular_map_tiles(_handle, regular_bg_tiles_ptr(tiles));
}

void regular_bg_map_ptr::set_tiles(regular_bg_tiles_ptr&& tiles)
{
    bg_blocks_manager::set_regular_map_tiles(_handle, move(tiles));
}

void regular_bg_map_ptr::set_tiles(const regular_bg_tiles_item& tiles_item)
{
    optional<regular_bg_tiles_ptr> tiles = tiles_item.find_tiles();

    if(regular_bg_tiles_ptr* tiles_ptr = tiles.get())
    {
        bg_blocks_manager::set_regular_map_tiles(_handle, move(*tiles_ptr));
    }
    else
    {
        bg_blocks_manager::remove_regular_map_tiles(_handle);
        bg_blocks_manager::set_regular_map_tiles(_handle, tiles_item.create_tiles());
    }
}

const bg_palette_ptr& regular_bg_map_ptr::palette() const
{
    return bg_blocks_manager::map_palette(_handle);
}

void regular_bg_map_ptr::set_palette(const bg_palette_ptr& palette)
{
    bg_blocks_manager::set_regular_map_palette(_handle, bg_palette_ptr(palette));
}

void regular_bg_map_ptr::set_palette(bg_palette_ptr&& palette)
{
    bg_blocks_manager::set_regular_map_palette(_handle, move(palette));
}

void regular_bg_map_ptr::set_palette(const bg_palette_item& palette_item)
{
    if(palette_item.bpp() == bpp_mode::BPP_4 || bpp() == bpp_mode::BPP_4)
    {
        optional<bg_palette_ptr> palette = palette_item.find_palette();

        if(bg_palette_ptr* palette_ptr = palette.get())
        {
            bg_blocks_manager::set_regular_map_palette(_handle, move(*palette_ptr));
        }
        else
        {
            bg_blocks_manager::remove_map_palette(_handle);
            bg_blocks_manager::set_regular_map_palette(_handle, palette_item.create_new_palette());
        }
    }
    else
    {
        bg_blocks_manager::set_regular_map_palette(_handle, palette_item.create_palette());
    }
}

void regular_bg_map_ptr::set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    bg_blocks_manager::set_regular_map_tiles_and_palette(_handle, move(tiles), move(palette));
}

void regular_bg_map_ptr::set_tiles_and_palette(const regular_bg_tiles_item& tiles_item,
                                               const bg_palette_item& palette_item)
{
    optional<regular_bg_tiles_ptr> tiles = tiles_item.find_tiles();
    regular_bg_tiles_ptr* tiles_ptr = tiles.get();

    if(! tiles_ptr)
    {
        bg_blocks_manager::remove_regular_map_tiles(_handle);
        tiles = tiles_item.create_tiles();
        tiles_ptr = tiles.get();
    }

    optional<bg_palette_ptr> palette = palette_item.find_palette();
    bg_palette_ptr* palette_ptr = palette.get();

    if(! palette_ptr)
    {
        if(palette_item.bpp() == bpp_mode::BPP_4 || bpp() == bpp_mode::BPP_4)
        {
            bg_blocks_manager::remove_map_palette(_handle);
        }

        palette = palette_item.create_new_palette();
        palette_ptr = palette.get();
    }

    bg_blocks_manager::set_regular_map_tiles_and_palette(_handle, move(*tiles_ptr), move(*palette_ptr));
}

optional<span<regular_bg_map_cell>> regular_bg_map_ptr::vram()
{
    return bg_blocks_manager::regular_map_vram(_handle);
}

}
