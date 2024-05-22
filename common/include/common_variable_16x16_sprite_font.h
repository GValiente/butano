/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef COMMON_VARIABLE_16x16_SPRITE_FONT_H
#define COMMON_VARIABLE_16x16_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_common_variable_16x16_font.h"

namespace common
{

constexpr bn::utf8_character variable_16x16_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t variable_16x16_sprite_font_character_widths[] = {
    6 * 2,  // 32
    4 * 2,  // 33 !
    7 * 2,  // 34 "
    8 * 2,  // 35 #
    8 * 2,  // 36 $
    7 * 2,  // 37 %
    8 * 2,  // 38 &
    4 * 2,  // 39 '
    5 * 2,  // 40 (
    5 * 2,  // 41 )
    7 * 2,  // 42 *
    7 * 2,  // 43 +
    4 * 2,  // 44 ,
    7 * 2,  // 45 -
    4 * 2,  // 46 .
    7 * 2,  // 47 /
    7 * 2,  // 48 0
    4 * 2,  // 49 1
    7 * 2,  // 50 2
    7 * 2,  // 51 3
    7 * 2,  // 52 4
    7 * 2,  // 53 5
    7 * 2,  // 54 6
    7 * 2,  // 55 7
    7 * 2,  // 56 8
    7 * 2,  // 57 9
    4 * 2,  // 58 :
    4 * 2,  // 59 ;
    6 * 2,  // 60 <
    7 * 2,  // 61 =
    6 * 2,  // 62 >
    7 * 2,  // 63 ?
    8 * 2,  // 64 @
    7 * 2,  // 65 A
    7 * 2,  // 66 B
    7 * 2,  // 67 C
    7 * 2,  // 68 D
    7 * 2,  // 69 E
    7 * 2,  // 70 F
    7 * 2,  // 71 G
    7 * 2,  // 72 H
    5 * 2,  // 73 I
    7 * 2,  // 74 J
    7 * 2,  // 75 K
    7 * 2,  // 76 L
    8 * 2,  // 77 M
    7 * 2,  // 78 N
    7 * 2,  // 79 O
    7 * 2,  // 80 P
    8 * 2,  // 81 Q
    7 * 2,  // 82 R
    7 * 2,  // 83 S
    7 * 2,  // 84 T
    7 * 2,  // 85 U
    7 * 2,  // 86 V
    8 * 2,  // 87 W
    7 * 2,  // 88 X
    7 * 2,  // 89 Y
    7 * 2,  // 90 Z
    5 * 2,  // 91 [
    7 * 2,  // 92
    5 * 2,  // 93 ]
    4 * 2,  // 94 ^
    7 * 2,  // 95 _
    4 * 2,  // 96 `
    7 * 2,  // 97 a
    7 * 2,  // 98 b
    7 * 2,  // 99 c
    7 * 2,  // 100 d
    7 * 2,  // 101 e
    6 * 2,  // 102 f
    7 * 2,  // 103 g
    7 * 2,  // 104 h
    3 * 2,  // 105 i
    4 * 2,  // 106 j
    6 * 2,  // 107 k
    3 * 2,  // 108 l
    8 * 2,  // 109 m
    7 * 2,  // 110 n
    7 * 2,  // 111 o
    7 * 2,  // 112 p
    7 * 2,  // 113 q
    6 * 2,  // 114 r
    7 * 2,  // 115 s
    6 * 2,  // 116 t
    7 * 2,  // 117 u
    7 * 2,  // 118 v
    8 * 2,  // 119 w
    7 * 2,  // 120 x
    7 * 2,  // 121 y
    7 * 2,  // 122 z
    6 * 2,  // 123 {
    3 * 2,  // 124 |
    6 * 2,  // 125 }
    8 * 2,  // 126 ~
    7 * 2,  // Á
    7 * 2,  // É
    5 * 2,  // Í
    7 * 2,  // Ó
    7 * 2,  // Ú
    7 * 2,  // Ü
    7 * 2,  // Ñ
    7 * 2,  // á
    7 * 2,  // é
    4 * 2,  // í
    7 * 2,  // ó
    7 * 2,  // ú
    7 * 2,  // ü
    7 * 2,  // ñ
    3 * 2,  // ¡
    7 * 2,  // ¿
};

constexpr bn::span<const bn::utf8_character> variable_16x16_sprite_font_utf8_characters_span(
        variable_16x16_sprite_font_utf8_characters);

constexpr auto variable_16x16_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<variable_16x16_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font variable_16x16_sprite_font(
        bn::sprite_items::common_variable_16x16_font, variable_16x16_sprite_font_utf8_characters_map.reference(),
        variable_16x16_sprite_font_character_widths);

}

#endif
