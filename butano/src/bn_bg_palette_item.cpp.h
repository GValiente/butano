/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_palette_item.h"

#include "bn_optional.h"
#include "bn_bg_palette_ptr.h"

namespace bn
{

optional<bg_palette_ptr> bg_palette_item::find_palette() const
{
    return bg_palette_ptr::find(*this);
}

bg_palette_ptr bg_palette_item::create_palette() const
{
    return bg_palette_ptr::create(*this);
}

bg_palette_ptr bg_palette_item::create_new_palette() const
{
    return bg_palette_ptr::create_new(*this);
}

optional<bg_palette_ptr> bg_palette_item::create_palette_optional() const
{
    return bg_palette_ptr::create_optional(*this);
}

optional<bg_palette_ptr> bg_palette_item::create_new_palette_optional() const
{
    return bg_palette_ptr::create_new_optional(*this);
}

}
