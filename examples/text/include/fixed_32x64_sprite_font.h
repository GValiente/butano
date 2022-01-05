/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FIXED_32x64_SPRITE_FONT_H
#define FIXED_32x64_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_sprite_items_fixed_32x64_font.h"

constexpr bn::string_view fixed_32x64_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr bn::sprite_font fixed_32x64_sprite_font(
        bn::sprite_items::fixed_32x64_font, fixed_32x64_sprite_font_utf8_characters);

#endif
