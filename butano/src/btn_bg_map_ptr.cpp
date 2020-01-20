#include "btn_bg_map_ptr.h"

#include "btn_span.h"
#include "btn_optional.h"
#include "btn_bg_blocks_manager.h"

namespace btn
{

namespace
{
    constexpr const bool aligned = false;

    [[nodiscard]] const uint16_t& to_data_ref(const bg_map_cell& cells_ref)
    {
        return reinterpret_cast<const uint16_t&>(cells_ref);
    }

    [[nodiscard]] span<bg_map_cell> to_cells_ref(span<uint16_t>& data_ref)
    {
        auto cells_ptr = reinterpret_cast<bg_map_cell*>(data_ref.data());
        return span<bg_map_cell>(cells_ptr, data_ref.size());
    }
}

optional<bg_map_ptr> bg_map_ptr::find(const bg_map_cell& cells_ref, int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    optional<bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find(to_data_ref(cells_ref), width, height))
    {
        result = bg_map_ptr(*handle);
    }

    return result;
}

bg_map_ptr bg_map_ptr::create(const bg_map_cell& cells_ref, int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    optional<int> handle = bg_blocks_manager::create(to_data_ref(cells_ref), width, height, aligned);
    BTN_ASSERT(handle, "Map create failed");

    return bg_map_ptr(*handle);
}

bg_map_ptr bg_map_ptr::find_or_create(const bg_map_cell& cells_ref, int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    const uint16_t& data_ref = to_data_ref(cells_ref);
    optional<int> handle = bg_blocks_manager::find(data_ref, width, height);

    if(! handle)
    {
        handle = bg_blocks_manager::create(data_ref, width, height, aligned);
        BTN_ASSERT(handle, "Map find or create failed");
    }

    return bg_map_ptr(*handle);
}

bg_map_ptr bg_map_ptr::allocate(int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    optional<int> handle = bg_blocks_manager::allocate(width, height, aligned);
    BTN_ASSERT(handle, "Map allocate failed");

    return bg_map_ptr(*handle);
}

optional<bg_map_ptr> bg_map_ptr::optional_create(const bg_map_cell& cells_ref, int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    optional<bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::create(to_data_ref(cells_ref), width, height, aligned))
    {
        result = bg_map_ptr(*handle);
    }

    return result;
}

optional<bg_map_ptr> bg_map_ptr::optional_find_or_create(const bg_map_cell& cells_ref, int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    const uint16_t& data_ref = to_data_ref(cells_ref);
    optional<bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find(data_ref, width, height))
    {
        result = bg_map_ptr(*handle);
    }
    else if(optional<int> handle = bg_blocks_manager::create(data_ref, width, height, aligned))
    {
        result = bg_map_ptr(*handle);
    }

    return result;
}

optional<bg_map_ptr> bg_map_ptr::optional_allocate(int width, int height)
{
    BTN_ASSERT(width == 32 || width == 64, "Invalid width: ", width);
    BTN_ASSERT(height == 32 || height == 64, "Invalid height: ", height);

    optional<bg_map_ptr> result;

    if(optional<int> handle = bg_blocks_manager::allocate(width, height, aligned))
    {
        result = bg_map_ptr(*handle);
    }

    return result;
}

bg_map_ptr::bg_map_ptr(const bg_map_ptr& other) :
    bg_map_ptr(other._handle)
{
    bg_blocks_manager::increase_usages(_handle);
}

bg_map_ptr& bg_map_ptr::operator=(const bg_map_ptr& other)
{
    if(_handle != other._handle)
    {
        _destroy();
        _handle = other._handle;
        bg_blocks_manager::increase_usages(_handle);
    }

    return *this;
}

bg_map_ptr::bg_map_ptr(bg_map_ptr&& other) :
    bg_map_ptr(other._handle)
{
    other._handle = -1;
}

bg_map_ptr& bg_map_ptr::operator=(bg_map_ptr&& other)
{
    swap(_handle, other._handle);
    return *this;
}

int bg_map_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle, aligned);
}

int bg_map_ptr::width() const
{
    return bg_blocks_manager::width(_handle);
}

int bg_map_ptr::height() const
{
    return bg_blocks_manager::height(_handle);
}

const bg_map_cell* bg_map_ptr::cells_ref() const
{
    if(const uint16_t* data_ref = bg_blocks_manager::data_ref(_handle))
    {
        return reinterpret_cast<const uint16_t*>(data_ref);
    }

    return nullptr;
}

void bg_map_ptr::set_cells_ref(const bg_map_cell& cells_ref, int width, int height)
{
    bg_blocks_manager::set_data_ref(_handle, to_data_ref(cells_ref), width, height);
}

void bg_map_ptr::reload_cells_ref()
{
    bg_blocks_manager::reload_data_ref(_handle);
}

optional<span<bg_map_cell>> bg_map_ptr::vram()
{
    optional<span<bg_map_cell>> result;

    if(optional<span<uint16_t>> vram = bg_blocks_manager::vram(_handle))
    {
        result = to_cells_ref(*vram);
    }

    return result;
}

void bg_map_ptr::_destroy()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
        _handle = -1;
    }
}

}
