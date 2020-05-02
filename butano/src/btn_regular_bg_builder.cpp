#include "btn_regular_bg_builder.h"

#include "btn_bgs.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_ptr.h"

namespace btn
{

regular_bg_builder& regular_bg_builder::set_priority(int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

regular_bg_builder& regular_bg_builder::set_z_order(int z_order)
{
    BTN_ASSERT(z_order >= bgs::min_z_order() && z_order <= bgs::max_z_order(), "Invalid z order: ", z_order);

    _z_order = z_order;
    return *this;
}

regular_bg_ptr regular_bg_builder::build() const
{
    return regular_bg_ptr::create(*this);
}

regular_bg_ptr regular_bg_builder::release_build()
{
    return regular_bg_ptr::create(move(*this));
}

optional<regular_bg_ptr> regular_bg_builder::optional_build() const
{
    return regular_bg_ptr::optional_create(*this);
}

optional<regular_bg_ptr> regular_bg_builder::optional_release_build()
{
    return regular_bg_ptr::optional_create(move(*this));
}

regular_bg_map_ptr regular_bg_builder::map() const
{
    if(_item)
    {
        return _item->map_item().create_map(_item->tiles_item().create_tiles(),
                                            _item->palette_item().create_palette());
    }

    BTN_ASSERT(_map, "Map has been already released");

    return *_map;
}

optional<regular_bg_map_ptr> regular_bg_builder::optional_map() const
{
    optional<regular_bg_map_ptr> result;

    if(_item)
    {
        if(optional<bg_tiles_ptr> tiles_ptr = _item->tiles_item().optional_create_tiles())
        {
            if(optional<bg_palette_ptr> palette_ptr = _item->palette_item().optional_create_palette())
            {
                result = _item->map_item().optional_create_map(move(*tiles_ptr), move(*palette_ptr));
            }
        }
    }
    else
    {
        result = _map;
    }

    return result;
}

regular_bg_map_ptr regular_bg_builder::release_map()
{
    if(_item)
    {
        return _item->map_item().create_map(_item->tiles_item().create_tiles(),
                                            _item->palette_item().create_palette());
    }

    BTN_ASSERT(_map, "Map has been already released");

    regular_bg_map_ptr result = move(*_map);
    _map.reset();
    return result;
}

optional<regular_bg_map_ptr> regular_bg_builder::optional_release_map()
{
    optional<regular_bg_map_ptr> result;

    if(_item)
    {
        if(optional<bg_tiles_ptr> tiles = _item->tiles_item().optional_create_tiles())
        {
            if(optional<bg_palette_ptr> palette = _item->palette_item().optional_create_palette())
            {
                result = _item->map_item().optional_create_map(move(*tiles), move(*palette));
            }
        }
    }
    else
    {
        result = move(_map);
    }

    return result;
}

}
