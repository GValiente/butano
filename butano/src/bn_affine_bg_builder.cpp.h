/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_builder.h"

#include "bn_bgs.h"
#include "bn_affine_bg_ptr.h"
#include "bn_top_left_utils.h"

namespace bn
{

affine_bg_builder::affine_bg_builder(const affine_bg_item& item, int map_index) :
    _item(item),
    _map_index(map_index)
{
    BN_ASSERT(map_index >= 0 && map_index < item.map_item().maps_count(),
              "Invalid map index: ", map_index, " - ", item.map_item().maps_count());
}

fixed affine_bg_builder::top_left_x() const
{
    return to_top_left_x(x(), _dimensions().width());
}

affine_bg_builder& affine_bg_builder::set_top_left_x(fixed top_left_x)
{
    return set_x(from_top_left_x(top_left_x, _dimensions().width()));
}

fixed affine_bg_builder::top_left_y() const
{
    return to_top_left_y(y(), _dimensions().height());
}

affine_bg_builder& affine_bg_builder::set_top_left_y(fixed top_left_y)
{
    return set_y(from_top_left_y(top_left_y, _dimensions().height()));
}

fixed_point affine_bg_builder::top_left_position() const
{
    return to_top_left_position(position(), _dimensions());
}

affine_bg_builder& affine_bg_builder::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    return set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), _dimensions()));
}

affine_bg_builder& affine_bg_builder::set_top_left_position(const fixed_point& top_left_position)
{
    return set_position(from_top_left_position(top_left_position, _dimensions()));
}

affine_bg_builder& affine_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

affine_bg_builder& affine_bg_builder::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= bgs::min_z_order() && z_order <= bgs::max_z_order(), "Invalid z order: ", z_order);

    _z_order = z_order;
    return *this;
}

optional<camera_ptr> affine_bg_builder::release_camera()
{
    optional<camera_ptr> result = move(_camera);
    _camera.reset();
    return result;
}

affine_bg_ptr affine_bg_builder::build() const
{
    return affine_bg_ptr::create(*this);
}

affine_bg_ptr affine_bg_builder::release_build()
{
    return affine_bg_ptr::create(move(*this));
}

optional<affine_bg_ptr> affine_bg_builder::build_optional() const
{
    return affine_bg_ptr::create_optional(*this);
}

optional<affine_bg_ptr> affine_bg_builder::release_build_optional()
{
    return affine_bg_ptr::create_optional(move(*this));
}

affine_bg_map_ptr affine_bg_builder::map() const
{
    if(const affine_bg_item* item = _item.get())
    {
        return affine_bg_map_ptr::create(*item, _map_index);
    }

    return *_map;
}

optional<affine_bg_map_ptr> affine_bg_builder::map_optional() const
{
    optional<affine_bg_map_ptr> result;

    if(const affine_bg_item* item = _item.get())
    {
        result = affine_bg_map_ptr::create_optional(*item, _map_index);
    }
    else
    {
        result = _map;
    }

    return result;
}

affine_bg_map_ptr affine_bg_builder::release_map()
{
    const affine_bg_item* item = _item.get();
    affine_bg_map_ptr result = item ? affine_bg_map_ptr::create(*item, _map_index) : move(*_map);
    _map.reset();
    return result;
}

optional<affine_bg_map_ptr> affine_bg_builder::release_map_optional()
{
    optional<affine_bg_map_ptr> result;

    if(const affine_bg_item* item = _item.get())
    {
        result = affine_bg_map_ptr::create_optional(*item, _map_index);
    }
    else
    {
        result = move(_map);
        _map.reset();
    }

    return result;
}

size affine_bg_builder::_dimensions() const
{
    const affine_bg_item* item = _item.get();
    return item ? item->map_item().dimensions() : _map->dimensions();
}

}
