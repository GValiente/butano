/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SMALL_FIXED_SPRITE_FONT_H
#define FR_SMALL_FIXED_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"

#include "bn_sprite_items_small_fixed_font.h"

namespace fr
{
    constexpr bn::utf8_character small_fixed_sprite_font_utf8_characters[] = {
        "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
    };

    constexpr bn::span<const bn::utf8_character> small_fixed_sprite_font_utf8_characters_span(
            small_fixed_sprite_font_utf8_characters);

    constexpr auto small_fixed_sprite_font_utf8_characters_map =
            bn::utf8_characters_map<small_fixed_sprite_font_utf8_characters_span>();

    constexpr bn::sprite_font small_fixed_sprite_font(
        bn::sprite_items::small_fixed_font, small_fixed_sprite_font_utf8_characters_map.reference());
}

#endif
