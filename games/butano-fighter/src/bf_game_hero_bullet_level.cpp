#include "bf_game_hero_bullet_level.h"

#include "btn_math.h"
#include "btn_sound_items.h"

namespace bf::game
{

namespace
{
    constexpr btn::fixed_point _unit_vector(btn::fixed x, btn::fixed y)
    {
        btn::fixed magnitude = btn::newton_raphson_sqrt((x * x) + (y * y));
        return btn::fixed_point(x, y) / magnitude;
    }

    constexpr btn::fixed_point _direction_vector(btn::fixed x, btn::fixed y, btn::fixed speed = 1)
    {
        return _unit_vector(x, y) * speed * btn::fixed(2.5);
    }

    constexpr const hero_bullet_event _level0_events[] = {
        hero_bullet_event(_direction_vector(0, -1),         1,  0,  true),
    };

    constexpr const hero_bullet_event _level1_events[] = {
        hero_bullet_event(_direction_vector(0, -1),         1,  1,  true),
        hero_bullet_event(_direction_vector(-0.075, -1),    11, 0,  true),
        hero_bullet_event(_direction_vector(0.075, -1),     31, 0,  true),
    };

    constexpr const hero_bullet_event _level2_events[] = {
        hero_bullet_event(_direction_vector(0, -1, 0.7),    1,  2,  true),
        hero_bullet_event(_direction_vector(-0.085, -1),    11, 0,  false),
        hero_bullet_event(_direction_vector(0.05, -1),      21, 1,  true),
        hero_bullet_event(_direction_vector(0.085, -1),     31, 0,  false),

        hero_bullet_event(_direction_vector(0, -1, 0.7),    41, 2,  true),
        hero_bullet_event(_direction_vector(-0.085, -1),    51, 0,  false),
        hero_bullet_event(_direction_vector(-0.05, -1),     61, 1,  true),
        hero_bullet_event(_direction_vector(0.085, -1),     71, 0,  false),
    };

    constexpr const hero_bullet_level _levels[] = {
        hero_bullet_level(_level0_events, btn::fixed_size(7, 7),    btn::sound_items::gun_5,  btn::color(31, 28, 5),  20,   1),
        hero_bullet_level(_level1_events, btn::fixed_size(14, 14),  btn::sound_items::gun_5,  btn::color(31, 28, 5),  40,   2),
        hero_bullet_level(_level2_events, btn::fixed_size(11, 22),  btn::sound_items::gun_1,  btn::color(13, 2, 2),   80,   4),
    };
}

btn::span<const hero_bullet_level> hero_bullet_level::all_levels()
{
    return _levels;
}

}
