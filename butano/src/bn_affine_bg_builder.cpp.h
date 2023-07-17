/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_builder.h"

#include "bn_bgs.h"
#include "bn_affine_bg_ptr.h"

namespace bn
{

affine_bg_builder::affine_bg_builder(const affine_bg_item& item, int map_index) :
    _item(item),
    _map_index(map_index)
{
    BN_ASSERT(map_index >= 0 && map_index < item.map_item().maps_count(),
              "Invalid map index: ", map_index, " - ", item.map_item().maps_count());
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
        return item->create_map(_map_index);
    }

    return *_map;
}

optional<affine_bg_map_ptr> affine_bg_builder::map_optional() const
{
    optional<affine_bg_map_ptr> result;

    if(const affine_bg_item* item = _item.get())
    {
        result = item->create_map_optional(_map_index);
    }
    else
    {
        result = _map;
    }

    return result;
}

affine_bg_map_ptr affine_bg_builder::release_map()
{
    if(const affine_bg_item* item = _item.get())
    {
        return item->create_map(_map_index);
    }

    affine_bg_map_ptr result = move(*_map);
    _map.reset();
    return result;
}

optional<affine_bg_map_ptr> affine_bg_builder::release_map_optional()
{
    optional<affine_bg_map_ptr> result;

    if(const affine_bg_item* item = _item.get())
    {
        result = item->create_map_optional(_map_index);
    }
    else
    {
        if(affine_bg_map_ptr* map = _map.get())
        {
            result = move(*map);
            _map.reset();
        }
    }

    return result;
}

}
