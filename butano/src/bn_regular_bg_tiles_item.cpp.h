/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_tiles_item.h"

#include "bn_optional.h"
#include "bn_regular_bg_tiles_ptr.h"

namespace bn
{

optional<regular_bg_tiles_ptr> regular_bg_tiles_item::find_tiles() const
{
    return regular_bg_tiles_ptr::find(*this);
}

regular_bg_tiles_ptr regular_bg_tiles_item::create_tiles() const
{
    return regular_bg_tiles_ptr::create(*this);
}

regular_bg_tiles_ptr regular_bg_tiles_item::create_new_tiles() const
{
    return regular_bg_tiles_ptr::create_new(*this);
}

optional<regular_bg_tiles_ptr> regular_bg_tiles_item::create_tiles_optional() const
{
    return regular_bg_tiles_ptr::create_optional(*this);
}

optional<regular_bg_tiles_ptr> regular_bg_tiles_item::create_new_tiles_optional() const
{
    return regular_bg_tiles_ptr::create_new_optional(*this);
}

}
