/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_builder.h"

#include "bn_bgs.h"
#include "bn_regular_bg_ptr.h"

namespace bn
{

regular_bg_builder& regular_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

regular_bg_builder& regular_bg_builder::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= bgs::min_z_order() && z_order <= bgs::max_z_order(), "Invalid Z order: ", z_order);

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

optional<regular_bg_ptr> regular_bg_builder::build_optional() const
{
    return regular_bg_ptr::create_optional(*this);
}

optional<regular_bg_ptr> regular_bg_builder::release_build_optional()
{
    return regular_bg_ptr::create_optional(move(*this));
}

regular_bg_map_ptr regular_bg_builder::map() const
{
    if(_item)
    {
        return _item->create_map();
    }

    BN_ASSERT(_map, "Map has been already released");

    return *_map;
}

optional<regular_bg_map_ptr> regular_bg_builder::map_optional() const
{
    optional<regular_bg_map_ptr> result;

    if(_item)
    {
        result = _item->create_map_optional();
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
        return _item->create_map();
    }

    BN_ASSERT(_map, "Map has been already released");

    regular_bg_map_ptr result = move(*_map);
    _map.reset();
    return result;
}

optional<regular_bg_map_ptr> regular_bg_builder::release_map_optional()
{
    optional<regular_bg_map_ptr> result;

    if(_item)
    {
        result = _item->create_map_optional();
    }
    else
    {
        if(_map)
        {
            result = move(*_map);
            _map.reset();
        }
    }

    return result;
}

}
