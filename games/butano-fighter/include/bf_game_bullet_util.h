/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BULLET_UTIL_H
#define BF_GAME_BULLET_UTIL_H

#include "bn_math.h"
#include "bn_fixed_point.h"

namespace bf::game
{
    [[nodiscard]] constexpr bn::fixed_point unit_vector(bn::fixed x, bn::fixed y)
    {
        bn::fixed magnitude = bn::sqrt((x * x) + (y * y));
        return bn::fixed_point(x, y) / magnitude;
    }

    [[nodiscard]] constexpr bn::fixed_point direction_vector(bn::fixed x, bn::fixed y, bn::fixed speed)
    {
        return unit_vector(x, y) * speed;
    }
}

#endif
