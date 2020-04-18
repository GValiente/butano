#ifndef FIXED_8x16_SPRITE_FONT_H
#define FIXED_8x16_SPRITE_FONT_H

#include "btn_sprite_font.h"
#include "btn_sprite_items_fixed_8x16_font.h"

constexpr const btn::string_view fixed_8x16_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr const btn::sprite_font fixed_8x16_sprite_font(
        btn::sprite_items::fixed_8x16_font, fixed_8x16_sprite_font_utf8_characters);

#endif
