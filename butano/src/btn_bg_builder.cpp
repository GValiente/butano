#include "btn_bg_builder.h"

#include "btn_bg_ptr.h"
#include "btn_bgs_manager.h"

namespace btn
{

bg_builder& bg_builder::set_priority(int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs_manager::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

bg_ptr bg_builder::build() const
{
    return bg_ptr::create(*this);
}

bg_ptr bg_builder::build_and_release()
{
    return bg_ptr::create(move(*this));
}

optional<bg_ptr> bg_builder::optional_build() const
{
    return bg_ptr::optional_create(*this);
}

optional<bg_ptr> bg_builder::optional_build_and_release()
{
    return bg_ptr::optional_create(move(*this));
}

optional<bg_tiles_ptr> bg_builder::tiles() const
{
    optional<bg_tiles_ptr> result;

    if(_tiles_ptr)
    {
        result = _tiles_ptr;
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->tiles_item().create_tiles(_tiles_create_mode);
    }

    return result;
}

optional<bg_map_ptr> bg_builder::map() const
{
    optional<bg_map_ptr> result;

    if(_map_ptr)
    {
        result = _map_ptr;
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->map_item().create_map(_map_create_mode);
    }

    return result;
}

optional<bg_palette_ptr> bg_builder::palette() const
{
    optional<bg_palette_ptr> result;

    if(_palette_ptr)
    {
        result = _palette_ptr;
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->palette_item().create_bg_palette(_palette_create_mode);
    }

    return result;
}

optional<bg_tiles_ptr> bg_builder::release_tiles()
{
    optional<bg_tiles_ptr> result;

    if(_tiles_ptr)
    {
        result = move(_tiles_ptr);
        _tiles_ptr.reset();
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->tiles_item().create_tiles(_tiles_create_mode);
    }

    return result;
}

optional<bg_map_ptr> bg_builder::release_map()
{
    optional<bg_map_ptr> result;

    if(_map_ptr)
    {
        result = move(_map_ptr);
        _map_ptr.reset();
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->map_item().create_map(_map_create_mode);
    }

    return result;
}

optional<bg_palette_ptr> bg_builder::release_palette()
{
    optional<bg_palette_ptr> result;

    if(_palette_ptr)
    {
        result = move(_palette_ptr);
        _palette_ptr.reset();
    }
    else
    {
        BTN_ASSERT(_item, "Item is null");

        result = _item->palette_item().create_bg_palette(_palette_create_mode);
    }

    return result;
}

}
