/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Tempest https://soundcloud.com/janne-suni
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_COLORS_H
#define FR_MODEL_COLORS_H

#include "bn_color.h"

namespace fr
{

constexpr bn::color default_model_colors[] = {
    bn::color(21, 6, 4), bn::color(13, 18, 22), bn::color(31, 31, 31), bn::color(16, 16, 16),
    bn::color(19, 9, 1), bn::color(0, 12, 0), bn::color(6, 6, 6), bn::color(31, 22, 0),
};

constexpr bn::color dawn_model_colors[] = {
    bn::color(25, 3, 5), bn::color(14, 19, 22), bn::color(31, 30, 28), bn::color(16, 15, 13),
    bn::color(13, 10, 9), bn::color(9, 11, 10), bn::color(6, 5, 3), bn::color(29, 24, 17),
};

constexpr bn::color snow_model_colors[] = {
    bn::color(27, 23, 22), bn::color(16, 21, 24), bn::color(31, 31, 31), bn::color(16, 16, 18),
    bn::color(28, 21, 16), bn::color(22, 26, 27), bn::color(10, 10, 11), bn::color(30, 25, 19),
};

constexpr bn::color desert_model_colors[] = {
    bn::color(24, 7, 4), bn::color(15, 19, 24), bn::color(30, 29, 27), bn::color(19, 15, 14),
    bn::color(21, 11, 5), bn::color(11, 13, 3), bn::color(7, 5, 3), bn::color(31, 20, 5),
};

}

#endif
