/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_builder.h"

#include "bn_bgs.h"
#include "bn_regular_bg_ptr.h"
#include "bn_top_left_utils.h"

namespace bn
{

regular_bg_builder::regular_bg_builder(const regular_bg_item& item, int map_index) :
    _item(item),
    _map_index(map_index)
{
    BN_ASSERT(map_index >= 0 && map_index < item.map_item().maps_count(),
              "Invalid map index: ", map_index, " - ", item.map_item().maps_count());
}

fixed regular_bg_builder::top_left_x() const
{
    return to_top_left_x(x(), _dimensions().width());
}

regular_bg_builder& regular_bg_builder::set_top_left_x(fixed top_left_x)
{
    return set_x(from_top_left_x(top_left_x, _dimensions().width()));
}

fixed regular_bg_builder::top_left_y() const
{
    return to_top_left_y(y(), _dimensions().height());
}

regular_bg_builder& regular_bg_builder::set_top_left_y(fixed top_left_y)
{
    return set_y(from_top_left_y(top_left_y, _dimensions().height()));
}

fixed_point regular_bg_builder::top_left_position() const
{
    return to_top_left_position(position(), _dimensions());
}

regular_bg_builder& regular_bg_builder::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    return set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), _dimensions()));
}

regular_bg_builder& regular_bg_builder::set_top_left_position(const fixed_point& top_left_position)
{
    return set_position(from_top_left_position(top_left_position, _dimensions()));
}

regular_bg_builder& regular_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

regular_bg_builder& regular_bg_builder::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= bgs::min_z_order() && z_order <= bgs::max_z_order(), "Invalid z order: ", z_order);

    _z_order = z_order;
    return *this;
}

optional<camera_ptr> regular_bg_builder::release_camera()
{
    optional<camera_ptr> result = move(_camera);
    _camera.reset();
    return result;
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
    if(const regular_bg_item* item = _item.get())
    {
        return regular_bg_map_ptr::create(*item, _map_index);
    }

    return *_map;
}

optional<regular_bg_map_ptr> regular_bg_builder::map_optional() const
{
    optional<regular_bg_map_ptr> result;

    if(const regular_bg_item* item = _item.get())
    {
        result = regular_bg_map_ptr::create_optional(*item, _map_index);
    }
    else
    {
        result = _map;
    }

    return result;
}

regular_bg_map_ptr regular_bg_builder::release_map()
{
    const regular_bg_item* item = _item.get();
    regular_bg_map_ptr result = item ? regular_bg_map_ptr::create(*item, _map_index) : move(*_map);
    _map.reset();
    return result;
}

optional<regular_bg_map_ptr> regular_bg_builder::release_map_optional()
{
    optional<regular_bg_map_ptr> result;

    if(const regular_bg_item* item = _item.get())
    {
        result = regular_bg_map_ptr::create_optional(*item, _map_index);
    }
    else
    {
        result = move(_map);
        _map.reset();
    }

    return result;
}

size regular_bg_builder::_dimensions() const
{
    const regular_bg_item* item = _item.get();
    return item ? item->map_item().dimensions() : _map->dimensions();
}

}
