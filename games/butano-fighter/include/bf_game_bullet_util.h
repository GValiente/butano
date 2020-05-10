#ifndef BF_GAME_BULLET_UTIL_H
#define BF_GAME_BULLET_UTIL_H

#include "btn_math.h"
#include "btn_algorithm.h"
#include "btn_fixed_point.h"

namespace bf::game
{
    [[nodiscard]] constexpr btn::fixed_point unit_vector(btn::fixed x, btn::fixed y)
    {
        btn::fixed magnitude = btn::newton_raphson_sqrt((x * x) + (y * y));
        return btn::fixed_point(x, y) / magnitude;
    }

    [[nodiscard]] constexpr btn::fixed_point direction_vector(btn::fixed x, btn::fixed y, btn::fixed speed)
    {
        return unit_vector(x, y) * speed;
    }

    [[nodiscard]] constexpr btn::fixed_point aprox_unit_vector(btn::fixed x, btn::fixed y)
    {
        // https://en.m.wikipedia.org/wiki/Alpha_max_plus_beta_min_algorithm

        constexpr const btn::fixed alpha(0.960433870103);
        constexpr const btn::fixed beta(0.397824734759);
        btn::fixed abs_x = btn::abs(x);
        btn::fixed abs_y = btn::abs(y);
        btn::fixed max = btn::max(abs_x, abs_y);
        btn::fixed min = btn::min(abs_x, abs_y);
        btn::fixed magnitude = btn::max(max, (alpha * max) + (beta * min));
        return btn::fixed_point(x, y) / magnitude;
    }

    [[nodiscard]] constexpr btn::fixed_point aprox_direction_vector(btn::fixed x, btn::fixed y, btn::fixed speed)
    {
        return aprox_unit_vector(x, y) * speed;
    }
}

#endif
