#include "btn_bg_tiles_ptr.h"

#include "btn_size.h"
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

    [[nodiscard]] size data_size(int tiles)
    {
        return size(tiles * half_words_per_tile(), 1);
    }

    [[nodiscard]] size data_size(const span<const tile>& tiles_ref)
    {
        return size(int(tiles_ref.size() * half_words_per_tile()), 1);
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

    if(optional<int> handle = bg_blocks_manager::find(to_data_ref(tiles_ref), data_size(tiles_ref), nullptr))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

bg_tiles_ptr bg_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    optional<int> handle = bg_blocks_manager::create(to_data_ref(tiles_ref), data_size(tiles_ref), nullopt, aligned);
    BTN_ASSERT(handle, "Tiles create failed");

    return bg_tiles_ptr(*handle);
}

bg_tiles_ptr bg_tiles_ptr::find_or_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    const uint16_t& data_ref = to_data_ref(tiles_ref);
    size data_s = data_size(tiles_ref);
    optional<int> handle = bg_blocks_manager::find(data_ref, data_s, nullptr);

    if(! handle)
    {
        handle = bg_blocks_manager::create(data_ref, data_s, nullopt, aligned);
        BTN_ASSERT(handle, "Tiles find or create failed");
    }

    return bg_tiles_ptr(*handle);
}

bg_tiles_ptr bg_tiles_ptr::allocate(int tiles)
{
    BTN_ASSERT(tiles >= 0, "Invalid tiles: ", tiles);

    optional<int> handle = bg_blocks_manager::allocate(data_size(tiles), nullopt, aligned);
    BTN_ASSERT(handle, "Tiles allocate failed");

    return bg_tiles_ptr(*handle);
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::create(to_data_ref(tiles_ref), data_size(tiles_ref), nullopt, aligned))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_find_or_create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is null");

    const uint16_t& data_ref = to_data_ref(tiles_ref);
    size data_s = data_size(tiles_ref);
    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::find(data_ref, data_s, nullptr))
    {
        result = bg_tiles_ptr(*handle);
    }
    else if(optional<int> handle = bg_blocks_manager::create(data_ref, data_s, nullopt, aligned))
    {
        result = bg_tiles_ptr(*handle);
    }

    return result;
}

optional<bg_tiles_ptr> bg_tiles_ptr::optional_allocate(int tiles)
{
    BTN_ASSERT(tiles >= 0, "Invalid tiles: ", tiles);

    optional<bg_tiles_ptr> result;

    if(optional<int> handle = bg_blocks_manager::allocate(data_size(tiles), nullopt, aligned))
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
    return bg_blocks_manager::hw_id(_handle, aligned);
}

int bg_tiles_ptr::tiles_count() const
{
    return bg_blocks_manager::dimensions(_handle).width() / half_words_per_tile();
}

bool bg_tiles_ptr::valid_tiles_count(bool eight_bits_per_pixel) const
{
    int count = tiles_count();

    if(eight_bits_per_pixel)
    {
        return count > 0 && count < 2048 && (count % 2) == 0;
    }

    return count > 0 && count < 1024;
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

    bg_blocks_manager::set_data_ref(_handle, to_data_ref(tiles_ref), data_size(tiles_ref));
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
    bg_blocks_manager::decrease_usages(_handle);
}

}
