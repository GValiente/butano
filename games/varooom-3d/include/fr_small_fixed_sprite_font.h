/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SMALL_FIXED_SPRITE_FONT_H
#define FR_SMALL_FIXED_SPRITE_FONT_H

#include "bn_sprite_font.h"

#include "bn_sprite_items_small_fixed_font.h"

namespace fr
{
    constexpr bn::string_view small_fixed_sprite_font_utf8_characters[] = {
        "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
    };

    constexpr bn::sprite_font small_fixed_sprite_font(
        bn::sprite_items::small_fixed_font, small_fixed_sprite_font_utf8_characters);
}

#endif
