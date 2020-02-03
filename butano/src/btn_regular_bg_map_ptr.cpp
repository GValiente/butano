#include "btn_regular_bg_map_ptr.h"

#include "btn_size.h"
#include "btn_span.h"
#include "btn_optional.h"
#include "btn_bg_blocks_manager.h"

namespace btn
{

optional<regular_bg_map_ptr> regular_bg_map_ptr::find(const regular_bg_map_cell& cells_ref,
                                                      const size& dimensions, const bg_palette_ptr& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, palette_ptr))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

regular_bg_map_ptr regular_bg_map_ptr::create(const regular_bg_map_cell& cells_ref, const size& dimensions,
                                              const bg_palette_ptr& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, bg_palette_ptr(palette_ptr));
    BTN_ASSERT(handle, "Regular map create failed");

    return regular_bg_map_ptr(*handle);
}

regular_bg_map_ptr regular_bg_map_ptr::create(const regular_bg_map_cell& cells_ref, const size& dimensions,
                                              bg_palette_ptr&& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(palette_ptr));
    BTN_ASSERT(handle, "Regular map create failed");

    return regular_bg_map_ptr(*handle);
}

regular_bg_map_ptr regular_bg_map_ptr::find_or_create(const regular_bg_map_cell& cells_ref, const size& dimensions,
                                                      const bg_palette_ptr& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, palette_ptr);

    if(! handle)
    {
        handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, bg_palette_ptr(palette_ptr));
        BTN_ASSERT(handle, "Regular map find or create failed");
    }

    return regular_bg_map_ptr(*handle);
}

regular_bg_map_ptr regular_bg_map_ptr::find_or_create(const regular_bg_map_cell& cells_ref, const size& dimensions,
                                                      bg_palette_ptr&& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, palette_ptr);

    if(! handle)
    {
        handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(palette_ptr));
        BTN_ASSERT(handle, "Regular map find or create failed");
    }

    return regular_bg_map_ptr(*handle);
}

regular_bg_map_ptr regular_bg_map_ptr::allocate(const size& dimensions, const bg_palette_ptr& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::allocate_regular_map(dimensions, bg_palette_ptr(palette_ptr));
    BTN_ASSERT(handle, "Regular map allocate failed");

    return regular_bg_map_ptr(*handle);
}

regular_bg_map_ptr regular_bg_map_ptr::allocate(const size& dimensions, bg_palette_ptr&& palette_ptr)
{
    optional<int> handle = bg_blocks_manager::allocate_regular_map(dimensions, move(palette_ptr));
    BTN_ASSERT(handle, "Regular map allocate failed");

    return regular_bg_map_ptr(*handle);
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, const bg_palette_ptr& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, bg_palette_ptr(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_palette_ptr&& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_find_or_create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, const bg_palette_ptr& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, palette_ptr))
    {
        result = regular_bg_map_ptr(*handle);
    }
    else if(optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions,
                                                                         bg_palette_ptr(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_find_or_create(
        const regular_bg_map_cell& cells_ref, const size& dimensions, bg_palette_ptr&& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find_regular_map(cells_ref, dimensions, palette_ptr))
    {
        result = regular_bg_map_ptr(*handle);
    }
    else if(optional<int> handle = bg_blocks_manager::create_regular_map(cells_ref, dimensions, move(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_allocate(const size& dimensions,
                                                                   const bg_palette_ptr& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::allocate_regular_map(dimensions, bg_palette_ptr(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_ptr::optional_allocate(const size& dimensions,
                                                                   bg_palette_ptr&& palette_ptr)
{
    optional<regular_bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::allocate_regular_map(dimensions, move(palette_ptr)))
    {
        result = regular_bg_map_ptr(*handle);
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
    return bg_blocks_manager::hw_map_id(_handle);
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

const bg_palette_ptr& regular_bg_map_ptr::palette() const
{
    return bg_blocks_manager::map_palette(_handle);
}

void regular_bg_map_ptr::set_palette(const bg_palette_ptr& palette_ptr)
{
    bg_blocks_manager::set_map_palette(_handle, bg_palette_ptr(palette_ptr));
}

void regular_bg_map_ptr::set_palette(bg_palette_ptr&& palette_ptr)
{
    bg_blocks_manager::set_map_palette(_handle, move(palette_ptr));
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
