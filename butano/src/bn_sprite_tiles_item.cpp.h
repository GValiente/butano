/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_tiles_item.h"

#include "bn_optional.h"
#include "bn_sprite_tiles_ptr.h"

namespace bn
{

optional<sprite_tiles_ptr> sprite_tiles_item::find_tiles() const
{
    return sprite_tiles_ptr::find(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::find_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::find(*this, graphics_index);
}

sprite_tiles_ptr sprite_tiles_item::create_tiles() const
{
    return sprite_tiles_ptr::create(*this);
}

sprite_tiles_ptr sprite_tiles_item::create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::create(*this, graphics_index);
}

sprite_tiles_ptr sprite_tiles_item::create_new_tiles() const
{
    return sprite_tiles_ptr::create_new(*this);
}

sprite_tiles_ptr sprite_tiles_item::create_new_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::create_new(*this, graphics_index);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_tiles_optional() const
{
    return sprite_tiles_ptr::create_optional(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_tiles_optional(int graphics_index) const
{
    return sprite_tiles_ptr::create_optional(*this, graphics_index);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_new_tiles_optional() const
{
    return sprite_tiles_ptr::create_new_optional(*this);
}

optional<sprite_tiles_ptr> sprite_tiles_item::create_new_tiles_optional(int graphics_index) const
{
    return sprite_tiles_ptr::create_new_optional(*this, graphics_index);
}

}
