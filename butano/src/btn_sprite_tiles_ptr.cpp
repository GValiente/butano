#include "btn_sprite_tiles_ptr.h"

#include "btn_span.h"
#include "btn_optional.h"
#include "btn_sprite_tiles_manager.h"

namespace btn
{

optional<sprite_tiles_ptr> sprite_tiles_ptr::find(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::find(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

sprite_tiles_ptr sprite_tiles_ptr::create(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::create(tiles_ref);
    BTN_ASSERT(handle >= 0, "Tiles create failed");

    return sprite_tiles_ptr(handle);
}

sprite_tiles_ptr sprite_tiles_ptr::find_or_create(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::find(tiles_ref);

    if(handle < 0)
    {
        handle = sprite_tiles_manager::create(tiles_ref);
        BTN_ASSERT(handle >= 0, "Tiles find or create failed");
    }

    return sprite_tiles_ptr(handle);
}

sprite_tiles_ptr sprite_tiles_ptr::allocate(int tiles_count)
{
    int handle = sprite_tiles_manager::allocate(tiles_count);
    BTN_ASSERT(handle >= 0, "Tiles allocate failed");

    return sprite_tiles_ptr(handle);
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::optional_create(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::create(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::optional_find_or_create(const span<const tile>& tiles_ref)
{
    int handle = sprite_tiles_manager::find(tiles_ref);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }
    else
    {
        handle = sprite_tiles_manager::create(tiles_ref);

        if(handle >= 0)
        {
            result = sprite_tiles_ptr(handle);
        }
    }

    return result;
}

optional<sprite_tiles_ptr> sprite_tiles_ptr::optional_allocate(int tiles_count)
{
    int handle = sprite_tiles_manager::allocate(tiles_count);
    optional<sprite_tiles_ptr> result;

    if(handle >= 0)
    {
        result = sprite_tiles_ptr(handle);
    }

    return result;
}

sprite_tiles_ptr::sprite_tiles_ptr(const sprite_tiles_ptr& other) :
    sprite_tiles_ptr(other._handle)
{
    sprite_tiles_manager::increase_usages(_handle);
}

sprite_tiles_ptr& sprite_tiles_ptr::operator=(const sprite_tiles_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle >= 0)
        {
            sprite_tiles_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        sprite_tiles_manager::increase_usages(_handle);
    }

    return *this;
}

int sprite_tiles_ptr::id() const
{
    return sprite_tiles_manager::start_tile(_handle);
}

int sprite_tiles_ptr::tiles_count() const
{
    return sprite_tiles_manager::tiles_count(_handle);
}

optional<span<const tile>> sprite_tiles_ptr::tiles_ref() const
{
    return sprite_tiles_manager::tiles_ref(_handle);
}

void sprite_tiles_ptr::set_tiles_ref(const span<const tile>& tiles_ref)
{
    sprite_tiles_manager::set_tiles_ref(_handle, tiles_ref);
}

void sprite_tiles_ptr::reload_tiles_ref()
{
    sprite_tiles_manager::reload_tiles_ref(_handle);
}

optional<span<tile>> sprite_tiles_ptr::vram()
{
    return sprite_tiles_manager::vram(_handle);
}

void sprite_tiles_ptr::_destroy()
{
    sprite_tiles_manager::decrease_usages(_handle);
}

}
