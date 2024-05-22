/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef COMMON_VARIABLE_8x16_SPRITE_FONT_H
#define COMMON_VARIABLE_8x16_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_common_variable_8x16_font.h"

namespace common
{

constexpr bn::utf8_character variable_8x16_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t variable_8x16_sprite_font_character_widths[] = {
    6,  // 32
    4,  // 33 !
    7,  // 34 "
    8,  // 35 #
    8,  // 36 $
    7,  // 37 %
    8,  // 38 &
    4,  // 39 '
    5,  // 40 (
    5,  // 41 )
    7,  // 42 *
    7,  // 43 +
    4,  // 44 ,
    7,  // 45 -
    4,  // 46 .
    7,  // 47 /
    7,  // 48 0
    4,  // 49 1
    7,  // 50 2
    7,  // 51 3
    7,  // 52 4
    7,  // 53 5
    7,  // 54 6
    7,  // 55 7
    7,  // 56 8
    7,  // 57 9
    4,  // 58 :
    4,  // 59 ;
    6,  // 60 <
    7,  // 61 =
    6,  // 62 >
    7,  // 63 ?
    8,  // 64 @
    7,  // 65 A
    7,  // 66 B
    7,  // 67 C
    7,  // 68 D
    7,  // 69 E
    7,  // 70 F
    7,  // 71 G
    7,  // 72 H
    5,  // 73 I
    7,  // 74 J
    7,  // 75 K
    7,  // 76 L
    8,  // 77 M
    7,  // 78 N
    7,  // 79 O
    7,  // 80 P
    8,  // 81 Q
    7,  // 82 R
    7,  // 83 S
    7,  // 84 T
    7,  // 85 U
    7,  // 86 V
    8,  // 87 W
    7,  // 88 X
    7,  // 89 Y
    7,  // 90 Z
    5,  // 91 [
    7,  // 92
    5,  // 93 ]
    4,  // 94 ^
    7,  // 95 _
    4,  // 96 `
    7,  // 97 a
    7,  // 98 b
    7,  // 99 c
    7,  // 100 d
    7,  // 101 e
    6,  // 102 f
    7,  // 103 g
    7,  // 104 h
    3,  // 105 i
    4,  // 106 j
    6,  // 107 k
    3,  // 108 l
    8,  // 109 m
    7,  // 110 n
    7,  // 111 o
    7,  // 112 p
    7,  // 113 q
    6,  // 114 r
    7,  // 115 s
    6,  // 116 t
    7,  // 117 u
    7,  // 118 v
    8,  // 119 w
    7,  // 120 x
    7,  // 121 y
    7,  // 122 z
    6,  // 123 {
    3,  // 124 |
    6,  // 125 }
    8,  // 126 ~
    7,  // Á
    7,  // É
    5,  // Í
    7,  // Ó
    7,  // Ú
    7,  // Ü
    7,  // Ñ
    7,  // á
    7,  // é
    4,  // í
    7,  // ó
    7,  // ú
    7,  // ü
    7,  // ñ
    3,  // ¡
    7,  // ¿
};

constexpr bn::span<const bn::utf8_character> variable_8x16_sprite_font_utf8_characters_span(
        variable_8x16_sprite_font_utf8_characters);

constexpr auto variable_8x16_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<variable_8x16_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font variable_8x16_sprite_font(
        bn::sprite_items::common_variable_8x16_font, variable_8x16_sprite_font_utf8_characters_map.reference(),
        variable_8x16_sprite_font_character_widths);

}

#endif
