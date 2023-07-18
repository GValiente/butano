/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_item.h"

#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_map_ptr.h"

namespace bn
{

affine_bg_item affine_bg_item::decompress(
            span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref,
            span<affine_bg_map_cell> decompressed_cells_ref) const
{
    affine_bg_item result = *this;
    result._tiles_item = _tiles_item.decompress(decompressed_tiles_ref);
    result._palette_item = _palette_item.decompress(decompressed_colors_ref);
    result._map_item = _map_item.decompress(decompressed_cells_ref);
    return result;
}

affine_bg_item affine_bg_item::decompress(
        span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref,
        affine_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const
{
    span<affine_bg_map_cell> decompressed_cells_span(
                &decompressed_cells_ref, decompressed_dimensions.width() * decompressed_dimensions.height());
    return decompress(decompressed_tiles_ref, decompressed_colors_ref, decompressed_cells_span);
}

affine_bg_ptr affine_bg_item::create_bg(fixed x, fixed y) const
{
    return affine_bg_ptr::create(x, y, *this);
}

affine_bg_ptr affine_bg_item::create_bg(fixed x, fixed y, int map_index) const
{
    return affine_bg_ptr::create(x, y, *this, map_index);
}

affine_bg_ptr affine_bg_item::create_bg(const fixed_point& position) const
{
    return affine_bg_ptr::create(position, *this);
}

affine_bg_ptr affine_bg_item::create_bg(const fixed_point& position, int map_index) const
{
    return affine_bg_ptr::create(position, *this, map_index);
}

optional<affine_bg_ptr> affine_bg_item::create_bg_optional(fixed x, fixed y) const
{
    return affine_bg_ptr::create_optional(x, y, *this);
}

optional<affine_bg_ptr> affine_bg_item::create_bg_optional(fixed x, fixed y, int map_index) const
{
    return affine_bg_ptr::create_optional(x, y, *this, map_index);
}

optional<affine_bg_ptr> affine_bg_item::create_bg_optional(const fixed_point& position) const
{
    return affine_bg_ptr::create_optional(position, *this);
}

optional<affine_bg_ptr> affine_bg_item::create_bg_optional(const fixed_point& position, int map_index) const
{
    return affine_bg_ptr::create_optional(position, *this, map_index);
}

optional<affine_bg_map_ptr> affine_bg_item::find_map() const
{
    return affine_bg_map_ptr::find(*this);
}

optional<affine_bg_map_ptr> affine_bg_item::find_map(int map_index) const
{
    return affine_bg_map_ptr::find(*this, map_index);
}

affine_bg_map_ptr affine_bg_item::create_map() const
{
    return affine_bg_map_ptr::create(*this);
}

affine_bg_map_ptr affine_bg_item::create_map(int map_index) const
{
    return affine_bg_map_ptr::create(*this, map_index);
}

affine_bg_map_ptr affine_bg_item::create_new_map() const
{
    return affine_bg_map_ptr::create_new(*this);
}

affine_bg_map_ptr affine_bg_item::create_new_map(int map_index) const
{
    return affine_bg_map_ptr::create_new(*this, map_index);
}

optional<affine_bg_map_ptr> affine_bg_item::create_map_optional() const
{
    return affine_bg_map_ptr::create_optional(*this);
}

optional<affine_bg_map_ptr> affine_bg_item::create_map_optional(int map_index) const
{
    return affine_bg_map_ptr::create_optional(*this, map_index);
}

optional<affine_bg_map_ptr> affine_bg_item::create_new_map_optional() const
{
    return affine_bg_map_ptr::create_new_optional(*this);
}

optional<affine_bg_map_ptr> affine_bg_item::create_new_map_optional(int map_index) const
{
    return affine_bg_map_ptr::create_new_optional(*this, map_index);
}

}
