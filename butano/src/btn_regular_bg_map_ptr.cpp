/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_regular_bg_map_ptr.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_item.h"
#include "btn_bg_blocks_manager.h"

namespace btn
{

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(
        const regular_bg_map_cell& cells_ref, const size& dimensions, const bg_tiles_ptr& tiles,
        const bg_palette_ptr& palette)
{
    int handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, tiles, palette);
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(
        const regular_bg_map_item& map_item, const bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
{
    return find(map_item.cells_ref(), map_item.dimensions(), tiles, palette);
}

regular_bg_map_ptr regular_bg_map_ptr::create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(tiles), move(palette));
    BTN_ASSERT(handle >= 0, "Regular map create failed");

    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(
        const regular_bg_map_item& map_item, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item.cells_ref(), map_item.dimensions(), move(tiles), move(palette));
    BTN_ASSERT(handle >= 0, "Regular map create failed");

    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(const regular_bg_item& item)
{
    const regular_bg_map_item& map_item = item.map_item();
    int handle = bg_blocks_manager::create_regular_map(
                map_item.cells_ref(), map_item.dimensions(), item.tiles_item().create_tiles(),
                item.palette_item().create_palette());
    BTN_ASSERT(handle >= 0, "Regular map create failed");

    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::find_or_create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::find_or_create_regular_map(cells_ref, dimensions, move(tiles), move(palette));
    BTN_ASSERT(handle >= 0, "Regular map find or create failed");

    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::find_or_create(
        const regular_bg_map_item& map_item, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::find_or_create_regular_map(
                map_item.cells_ref(), map_item.dimensions(), move(tiles), move(palette));
    BTN_ASSERT(handle >= 0, "Regular map find or create failed");

    return regular_bg_map_ptr(handle);
}

regular_bg_map_ptr regular_bg_map_ptr::allocate(
        const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_regular_map(dimensions, move(tiles), move(palette));
    BTN_ASSERT(handle >= 0, "Regular map allocate failed");

    return regular_bg_map_ptr(handle);
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(tiles), move(palette));
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(
        const regular_bg_map_item& map_item, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::create_regular_map(
                map_item.cells_ref(), map_item.dimensions(), move(tiles), move(palette));
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::create_optional(const regular_bg_item& item)
{
    optional<regular_bg_map_ptr> result;

    if(optional<bg_tiles_ptr> tiles = item.tiles_item().create_tiles_optional())
    {
        if(optional<bg_palette_ptr> palette = item.palette_item().create_palette_optional())
        {
            const regular_bg_map_item& map_item = item.map_item();
            int handle = bg_blocks_manager::create_regular_map(
                        map_item.cells_ref(), map_item.dimensions(), move(*tiles), move(*palette));

            if(handle >= 0)
            {
                result = regular_bg_map_ptr(handle);
            }
        }
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find_or_create_optional(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::find_or_create_regular_map(cells_ref, dimensions, move(tiles), move(palette));
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::find_or_create_optional(
        const regular_bg_map_item& map_item, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::find_or_create_regular_map(
                map_item.cells_ref(), map_item.dimensions(), move(tiles), move(palette));
    optional<regular_bg_map_ptr> result;

    if(handle >= 0)
    {
        result = regular_bg_map_ptr(handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::allocate_optional(
        const size& dimensions, bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    int handle = bg_blocks_manager::allocate_regular_map(dimensions, move(tiles), move(palette));
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

int regular_bg_map_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle);
}

size regular_bg_map_ptr::dimensions() const
{
    return bg_blocks_manager::map_dimensions(_handle);
}

palette_bpp_mode regular_bg_map_ptr::bpp_mode() const
{
    return palette().bpp_mode();
}

const regular_bg_map_cell* regular_bg_map_ptr::cells_ref() const
{
    return bg_blocks_manager::regular_map_cells_ref(_handle);
}

void regular_bg_map_ptr::set_cells_ref(const regular_bg_map_cell& cells_ref, const size& dimensions)
{
    bg_blocks_manager::set_regular_map_cells_ref(_handle, cells_ref, dimensions);
}

void regular_bg_map_ptr::reload_cells_ref()
{
    bg_blocks_manager::reload(_handle);
}

const bg_tiles_ptr& regular_bg_map_ptr::tiles() const
{
    return bg_blocks_manager::map_tiles(_handle);
}

void regular_bg_map_ptr::set_tiles(const bg_tiles_ptr& tiles)
{
    bg_blocks_manager::set_map_tiles(_handle, bg_tiles_ptr(tiles));
}

void regular_bg_map_ptr::set_tiles(bg_tiles_ptr&& tiles)
{
    bg_blocks_manager::set_map_tiles(_handle, move(tiles));
}

void regular_bg_map_ptr::set_tiles(const bg_tiles_item& tiles_item)
{
    if(optional<bg_tiles_ptr> tiles = tiles_item.find_tiles())
    {
        bg_blocks_manager::set_map_tiles(_handle, move(*tiles));
    }
    else
    {
        bg_blocks_manager::remove_map_tiles(_handle);
        bg_blocks_manager::set_map_tiles(_handle, tiles_item.create_new_tiles());
    }
}

const bg_palette_ptr& regular_bg_map_ptr::palette() const
{
    return bg_blocks_manager::map_palette(_handle);
}

void regular_bg_map_ptr::set_palette(const bg_palette_ptr& palette)
{
    bg_blocks_manager::set_map_palette(_handle, bg_palette_ptr(palette));
}

void regular_bg_map_ptr::set_palette(bg_palette_ptr&& palette)
{
    bg_blocks_manager::set_map_palette(_handle, move(palette));
}

void regular_bg_map_ptr::set_palette(const bg_palette_item& palette_item)
{
    if(optional<bg_palette_ptr> palette = palette_item.find_palette())
    {
        bg_blocks_manager::set_map_palette(_handle, move(*palette));
    }
    else
    {
        bg_blocks_manager::remove_map_palette(_handle);
        bg_blocks_manager::set_map_palette(_handle, palette_item.create_new_palette());
    }
}

void regular_bg_map_ptr::set_tiles_and_palette(bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    bg_blocks_manager::set_map_tiles_and_palette(_handle, move(tiles), move(palette));
}

void regular_bg_map_ptr::set_tiles_and_palette(const bg_tiles_item& tiles_item, const bg_palette_item& palette_item)
{
    optional<bg_tiles_ptr> tiles = tiles_item.find_tiles();

    if(! tiles)
    {
        bg_blocks_manager::remove_map_tiles(_handle);
        tiles = tiles_item.create_new_tiles();
    }

    optional<bg_palette_ptr> palette = palette_item.find_palette();

    if(! palette)
    {
        bg_blocks_manager::remove_map_palette(_handle);
        palette = palette_item.create_new_palette();
    }

    bg_blocks_manager::set_map_tiles_and_palette(_handle, move(*tiles), move(*palette));
}

optional<span<regular_bg_map_cell>> regular_bg_map_ptr::vram()
{
    return bg_blocks_manager::regular_map_vram(_handle);
}

void regular_bg_map_ptr::_destroy()
{
    bg_blocks_manager::decrease_usages(_handle);
}

}
