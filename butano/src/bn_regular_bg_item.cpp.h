/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_item.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_ptr.h"

namespace bn
{

regular_bg_item regular_bg_item::decompress(
            span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref,
            regular_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const
{
    regular_bg_item result = *this;
    result._tiles_item = _tiles_item.decompress(decompressed_tiles_ref);
    result._palette_item = _palette_item.decompress(decompressed_colors_ref);
    result._map_item = _map_item.decompress(decompressed_cells_ref, decompressed_dimensions);
    return result;
}

regular_bg_ptr regular_bg_item::create_bg(fixed x, fixed y) const
{
    return regular_bg_ptr::create(x, y, *this);
}

regular_bg_ptr regular_bg_item::create_bg(const fixed_point& position) const
{
    return regular_bg_ptr::create(position, *this);
}

optional<regular_bg_ptr> regular_bg_item::create_bg_optional(fixed x, fixed y) const
{
    return regular_bg_ptr::create_optional(x, y, *this);
}

optional<regular_bg_ptr> regular_bg_item::create_bg_optional(const fixed_point& position) const
{
    return regular_bg_ptr::create_optional(position, *this);
}

optional<regular_bg_map_ptr> regular_bg_item::find_map() const
{
    return regular_bg_map_ptr::find(*this);
}

regular_bg_map_ptr regular_bg_item::create_map() const
{
    return regular_bg_map_ptr::create(*this);
}

regular_bg_map_ptr regular_bg_item::create_new_map() const
{
    return regular_bg_map_ptr::create_new(*this);
}

optional<regular_bg_map_ptr> regular_bg_item::create_map_optional() const
{
    return regular_bg_map_ptr::create_optional(*this);
}

optional<regular_bg_map_ptr> regular_bg_item::create_new_map_optional() const
{
    return regular_bg_map_ptr::create_new_optional(*this);
}

}
