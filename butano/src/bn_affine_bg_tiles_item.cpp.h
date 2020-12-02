/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_tiles_item.h"

#include "bn_optional.h"
#include "bn_affine_bg_tiles_ptr.h"

namespace bn
{

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::find_tiles() const
{
    return affine_bg_tiles_ptr::find(*this);
}

affine_bg_tiles_ptr affine_bg_tiles_item::create_tiles() const
{
    return affine_bg_tiles_ptr::create(*this);
}

affine_bg_tiles_ptr affine_bg_tiles_item::create_new_tiles() const
{
    return affine_bg_tiles_ptr::create_new(*this);
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::create_tiles_optional() const
{
    return affine_bg_tiles_ptr::create_optional(*this);
}

optional<affine_bg_tiles_ptr> affine_bg_tiles_item::create_new_tiles_optional() const
{
    return affine_bg_tiles_ptr::create_new_optional(*this);
}

}
