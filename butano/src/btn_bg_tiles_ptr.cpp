#include "btn_bg_tiles_ptr.h"

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional.h"
#include "btn_bg_blocks_manager.h"

namespace btn
{

namespace
{
    constexpr const bool aligned = true;

    [[nodiscard]] constexpr int half_words_per_tile()
    {
        return int(sizeof(tile) / 2);
    }

    [[nodiscard]] const uint16_t& to_data_ref(const span<const tile>& tiles_ref)
    {
        auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());
        return *data_ptr;
    }

    [[nodiscard]] int data_width(int tiles)
    {
        return tiles * half_words_per_tile();
    }

    [[nodiscard]] int data_width(const span<const tile>& tiles_ref)
    {
        return data_width(int(tiles_ref.size()));
    }

    [[nodiscard]] span<const tile> to_tiles_ref(const uint16_t* data_ref, int tiles_count)
    {
        auto tiles_ptr = reinterpret_cast<const tile*>(data_ref);
        return span<const tile>(tiles_ptr, size_t(tiles_count));
    }

    [[nodiscard]] span<tile> to_tiles_ref(span<uint16_t>& data_ref)
    {
        auto tiles_ptr = reinterpret_cast<tile*>(data_ref.data());
        return span<tile>(tiles_ptr, data_ref.size() / half_words_per_tile());
    }
}

optional<bg_tiles_ptr> bg_tiles_ptr::find(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find(to_data_ref(tiles_ref), data_width(tiles_ref), 1))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

bg_tiles_ptr bg_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    optional<int> handle = bg_blocks_manager::create(to_data_ref(tiles_ref), data_width(tiles_ref), 1, aligned);
    BTN_ASSERT(handle, "Tiles create failed");

    return bg_tiles_ptr(*handle);
}

bg_tiles_ptr bg_tiles_ptr::find_or_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    const uint16_t& data_ref = to_data_ref(tiles_ref);
    int width = data_width(tiles_ref);
    optional<int> handle = bg_blocks_manager::find(data_ref, width, 1);

    if(! handle)
    {
        handle = bg_blocks_manager::create(data_ref, width, 1, aligned);
        BTN_ASSERT(handle, "Tiles find or create failed");
    }

    return bg_tiles_ptr(*handle);
}

bg_tiles_ptr bg_tiles_ptr::allocate(int tiles)
{
    BTN_ASSERT(tiles >= 0, "Invalid tiles: ", tiles);

    optional<int> handle = bg_blocks_manager::allocate(data_width(tiles), 1, aligned);
    BTN_ASSERT(handle, "Tiles allocate failed");

    return bg_tiles_ptr(*handle);
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::create(to_data_ref(tiles_ref), data_width(tiles_ref), 1, aligned))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_find_or_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    const uint16_t& data_ref = to_data_ref(tiles_ref);
    int width = data_width(tiles_ref);
    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find(data_ref, width, 1))
    {
        result = bg_tiles_ptr(*handle);
    }
    else if(optional<int> handle = bg_blocks_manager::create(data_ref, width, 1, aligned))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_allocate(int tiles)
{
    BTN_ASSERT(tiles >= 0, "Invalid tiles: ", tiles);

    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::allocate(data_width(tiles), 1, aligned))
    {
        result = bg_tiles_ptr(*handle);
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
        _destroy();
        _handle = other._handle;
        bg_blocks_manager::increase_usages(_handle);
    }

    return *this;
}

bg_tiles_ptr::bg_tiles_ptr(bg_tiles_ptr&& other) :
    bg_tiles_ptr(other._handle)
{
    other._handle = -1;
}

bg_tiles_ptr& bg_tiles_ptr::operator=(bg_tiles_ptr&& other)
{
    swap(_handle, other._handle);
    return *this;
}

int bg_tiles_ptr::id() const
{
    return bg_blocks_manager::hw_id(_handle, aligned);
}

int bg_tiles_ptr::tiles_count() const
{
    return bg_blocks_manager::width(_handle) / half_words_per_tile();
}

optional<span<const tile>> bg_tiles_ptr::tiles_ref() const
{
    optional<span<const tile>> result;

    if(const uint16_t* data_ref = bg_blocks_manager::data_ref(_handle))
    {
        result = to_tiles_ref(data_ref, tiles_count());
    }

    return result;
}

void bg_tiles_ptr::set_tiles_ref(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    bg_blocks_manager::set_data_ref(_handle, to_data_ref(tiles_ref), data_width(tiles_ref), 1);
}

void bg_tiles_ptr::reload_tiles_ref()
{
    bg_blocks_manager::reload_data_ref(_handle);
}

optional<span<tile>> bg_tiles_ptr::vram()
{
    optional<span<tile>> result;

    if(optional<span<uint16_t>> vram = bg_blocks_manager::vram(_handle))
    {
        result = to_tiles_ref(*vram);
    }

    return result;
}

void bg_tiles_ptr::_destroy()
{
    if(_handle >= 0)
    {
        bg_blocks_manager::decrease_usages(_handle);
        _handle = -1;
    }
}

}
