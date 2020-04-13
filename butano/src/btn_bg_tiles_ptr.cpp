#include "btn_bg_tiles_ptr.h"

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional.h"
#include "btn_palette_bpp_mode.h"
#include "btn_bg_blocks_manager.h"

namespace btn
{

optional<bg_tiles_ptr> bg_tiles_ptr::find(const span<const tile>& tiles_ref)
{
    int handle = bg_blocks_manager::find_tiles(tiles_ref);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

bg_tiles_ptr bg_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    int handle = bg_blocks_manager::create_tiles(tiles_ref);
    BTN_ASSERT(handle >= 0, "Tiles create failed");

    return bg_tiles_ptr(handle);
}

bg_tiles_ptr bg_tiles_ptr::find_or_create(const span<const tile>& tiles_ref)
{
    int handle = bg_blocks_manager::find_tiles(tiles_ref);

    if(handle < 0)
    {
        handle = bg_blocks_manager::create_tiles(tiles_ref);
        BTN_ASSERT(handle >= 0, "Tiles find or create failed");
    }

    return bg_tiles_ptr(handle);
}

bg_tiles_ptr bg_tiles_ptr::allocate(int tiles_count)
{
    int handle = bg_blocks_manager::allocate_tiles(tiles_count);
    BTN_ASSERT(handle >= 0, "Tiles allocate failed");

    return bg_tiles_ptr(handle);
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_create(const span<const tile>& tiles_ref)
{
    int handle = bg_blocks_manager::create_tiles(tiles_ref);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_find_or_create(const span<const tile>& tiles_ref)
{
    int handle = bg_blocks_manager::find_tiles(tiles_ref);
    optional<bg_tiles_ptr> result;

    if(handle >= 0)
    {
        result = bg_tiles_ptr(handle);
    }
    else
    {
        handle = bg_blocks_manager::create_tiles(tiles_ref);

        if(handle >= 0)
        {
            result = bg_tiles_ptr(handle);
        }
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_allocate(int tiles_count)
{
    int handle = bg_blocks_manager::allocate_tiles(tiles_count);
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

bool bg_tiles_ptr::valid_tiles_count(palette_bpp_mode bpp_mode) const
{
    int count = tiles_count();

    if(bpp_mode == palette_bpp_mode::BPP_8)
    {
        return count > 0 && count < 2048 && (count % 2) == 0;
    }

    return count > 0 && count < 1024;
}

optional<span<const tile>> bg_tiles_ptr::tiles_ref() const
{
    return bg_blocks_manager::tiles_ref(_handle);
}

void bg_tiles_ptr::set_tiles_ref(const span<const tile>& tiles_ref)
{
    bg_blocks_manager::set_tiles_ref(_handle, tiles_ref);
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
