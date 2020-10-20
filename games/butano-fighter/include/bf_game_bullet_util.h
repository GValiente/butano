/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BULLET_UTIL_H
#define BF_GAME_BULLET_UTIL_H

#include "btn_math.h"
#include "btn_fixed_point.h"

namespace bf::game
{
    [[nodiscard]] constexpr btn::fixed_point unit_vector(btn::fixed x, btn::fixed y)
    {
        btn::fixed magnitude = btn::sqrt((x * x) + (y * y));
        return btn::fixed_point(x, y) / magnitude;
    }

    [[nodiscard]] constexpr btn::fixed_point direction_vector(btn::fixed x, btn::fixed y, btn::fixed speed)
    {
        return unit_vector(x, y) * speed;
    }
}

#endif
