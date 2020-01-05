#include "btn_sprite_tiles_ptr.h"

#include "btn_span.h"
#include "btn_utility.h"
#include "btn_optional.h"
#include "btn_sprite_tiles_manager.h"
#include "../hw/include/btn_hw_sprite_tiles.h"

namespace btn
{

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const span<const tile>& tiles_ref)
{
    optional<sprite_tiles_ptr> result;

    if(optional<item_type::list_iterator> iterator = sprite_tiles_manager::find(tiles_ref))
    {
        result = sprite_tiles_ptr(*iterator);
    }

    return result;
}

sprite_tiles_ptr sprite_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    return sprite_tiles_ptr(sprite_tiles_manager::create(tiles_ref));
}

sprite_tiles_ptr sprite_tiles_ptr::find_or_create(const span<const tile>& tiles_ref)
{
    optional<sprite_tiles_ptr> result = find(tiles_ref);

    if(! result)
    {
        result = create(tiles_ref);
    }

    return *result;
}

sprite_tiles_ptr sprite_tiles_ptr::allocate(int tiles)
{
    return sprite_tiles_ptr(sprite_tiles_manager::allocate(tiles));
}

sprite_tiles_ptr::sprite_tiles_ptr(const sprite_tiles_ptr& other) :
    sprite_tiles_ptr(other._iterator)
{
    sprite_tiles_manager::increase_usages(_iterator);
}

sprite_tiles_ptr& sprite_tiles_ptr::operator=(const sprite_tiles_ptr& other)
{
    if(_iterator != other._iterator)
    {
        _destroy();
        _iterator = other._iterator;
        sprite_tiles_manager::increase_usages(_iterator);
    }

    return *this;
}

sprite_tiles_ptr::sprite_tiles_ptr(sprite_tiles_ptr&& other) :
    sprite_tiles_ptr(other._iterator)
{
    other._iterator = sprite_tiles_manager::invalid_iterator();
}

sprite_tiles_ptr& sprite_tiles_ptr::operator=(sprite_tiles_ptr&& other)
{
    swap(_iterator, other._iterator);
    return *this;
}

optional<span<const tile>> sprite_tiles_ptr::tiles_ref() const
{
    const item_type& item = *_iterator;
    optional<span<const tile>> result;

    if(item.data)
    {
        result.emplace(item.data, item.tiles_count);
    }

    return result;
}

void sprite_tiles_ptr::set_tiles_ref(const span<const tile>& tiles_ref)
{
    sprite_tiles_manager::set_tiles_ref(_iterator, tiles_ref);
}

void sprite_tiles_ptr::reload_tiles_ref()
{
    sprite_tiles_manager::reload_tiles_ref(_iterator);
}

optional<span<tile>> sprite_tiles_ptr::vram()
{
    const item_type& item = *_iterator;
    optional<span<tile>> result;

    if(! item.data)
    {
        result.emplace(&hw::sprite_tiles::vram(item.start_tile), item.tiles_count);
    }

    return result;
}

void sprite_tiles_ptr::_destroy()
{
    if(_iterator != sprite_tiles_manager::invalid_iterator())
    {
        sprite_tiles_manager::decrease_usages(_iterator);
    }
}

}
