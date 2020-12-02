/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_map_item.h"

#include "bn_optional.h"
#include "bn_bg_palette_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_tiles_ptr.h"

namespace bn
{

optional<affine_bg_map_ptr> affine_bg_map_item::find_map(
        const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const
{
    return affine_bg_map_ptr::find(*this, tiles, palette);
}

affine_bg_map_ptr affine_bg_map_item::create_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create(*this, move(tiles), move(palette));
}

affine_bg_map_ptr affine_bg_map_item::create_new_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_new(*this, move(tiles), move(palette));
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_optional(*this, move(tiles), move(palette));
}

optional<affine_bg_map_ptr> affine_bg_map_item::create_new_map_optional(
        affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return affine_bg_map_ptr::create_new_optional(*this, move(tiles), move(palette));
}

}
