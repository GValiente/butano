/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_third_attributes.h"

#include "bn_sprites.h"
#include "bn_assert.h"

namespace bn
{

sprite_third_attributes::sprite_third_attributes(sprite_tiles_ptr tiles, sprite_palette_ptr palette, int bg_priority) :
    _tiles(move(tiles)),
    _palette(move(palette)),
    _bg_priority(int8_t(bg_priority))
{
    BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid BG priority: ", bg_priority);
}

void sprite_third_attributes::set_bg_priority(int bg_priority)
{
    BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid BG priority: ", bg_priority);

    _bg_priority = int8_t(bg_priority);
}

}
