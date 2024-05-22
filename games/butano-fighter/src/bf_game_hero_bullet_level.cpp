/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_hero_bullet_level.h"

#include "bn_sound_items.h"
#include "bf_game_bullet_util.h"

namespace bf::game
{

namespace
{
    // 20 frames per bullet
    constexpr hero_bullet_event _level0_events[] = {
        hero_bullet_event(direction_vector(0,       -1,     2.5),   1,  0,  true),
    };

    // 13 frames per bullet
    constexpr hero_bullet_event _level1_events[] = {
        hero_bullet_event(direction_vector(0,       -1,     2.5),   1,  1,  true),
        hero_bullet_event(direction_vector(-0.075,  -1,     2.5),   11, 0,  true),
        hero_bullet_event(direction_vector(0.075,   -1,     2.5),   31, 0,  true),
    };

    // 10 frames per bullet
    constexpr hero_bullet_event _level2_events[] = {
        hero_bullet_event(direction_vector(0,       -1,     1.75),  1,  2,  true),
        hero_bullet_event(direction_vector(-0.085,  -1,     2.5),   11, 0,  false),
        hero_bullet_event(direction_vector(0.05,    -1,     2.5),   21, 1,  true),
        hero_bullet_event(direction_vector(0.085,   -1,     2.5),   31, 0,  false),

        hero_bullet_event(direction_vector(0,       -1,     1.75),  41, 2,  true),
        hero_bullet_event(direction_vector(-0.085,  -1,     2.5),   51, 0,  false),
        hero_bullet_event(direction_vector(-0.05,   -1,     2.5),   61, 1,  true),
        hero_bullet_event(direction_vector(0.085,   -1,     2.5),   71, 0,  false),
    };

    // 8 frames per bullet
    constexpr hero_bullet_event _level3_events[] = {
        hero_bullet_event(direction_vector(-0.025,  -1,     1.75),  1,  2,  true),
        hero_bullet_event(direction_vector(-0.1,    -1,     2.5),   9,  0,  false),
        hero_bullet_event(direction_vector(0.075,   -1,     2.5),   17, 1,  false),
        hero_bullet_event(direction_vector(0,       -1,     3.5),   25, 3,  true),
        hero_bullet_event(direction_vector(0.1,     -1,     2.5),   33, 0,  false),

        hero_bullet_event(direction_vector(0.025,   -1,     1.75),  41, 2,  true),
        hero_bullet_event(direction_vector(-0.1,    -1,     2.5),   49, 0,  false),
        hero_bullet_event(direction_vector(-0.075,  -1,     2.5),   57, 1,  false),
        hero_bullet_event(direction_vector(0,       -1,     3.5),   65, 3,  true),
        hero_bullet_event(direction_vector(0.1,     -1,     2.5),   73, 0,  false),
    };

    // 7 frames per bullet
    constexpr hero_bullet_event _level4_events[] = {
        hero_bullet_event(direction_vector(-0.05,   -1,     1.75),  1,  2,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     2.5),   8,  0,  false),
        hero_bullet_event(direction_vector(0.085,   -1,     2.5),   15, 1,  false),
        hero_bullet_event(direction_vector(-0.025,  -1,     3.5),   23, 3,  true),
        hero_bullet_event(direction_vector(0.115,   -1,     2.5),   30, 0,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     37, 4,  true),

        hero_bullet_event(direction_vector(0.05,    -1,     1.75),  44, 2,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     2.5),   51, 0,  false),
        hero_bullet_event(direction_vector(-0.085,  -1,     2.5),   58, 1,  false),
        hero_bullet_event(direction_vector(0.025,   -1,     3.5),   65, 3,  true),
        hero_bullet_event(direction_vector(0.115,   -1,     2.5),   72, 0,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     79, 4,  true),
    };

    // 6 frames per bullet
    constexpr hero_bullet_event _level5_events[] = {
        hero_bullet_event(direction_vector(-0.05,   -1,     1.75),  1,  2,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     2.5),   7,  0,  false),
        hero_bullet_event(direction_vector(0.115,   -1,     2.5),   13, 5,  true),
        hero_bullet_event(direction_vector(0.085,   -1,     2.5),   19, 1,  false),
        hero_bullet_event(direction_vector(-0.025,  -1,     3.5),   25, 3,  true),
        hero_bullet_event(direction_vector(0.13,    -1,     2.5),   31, 0,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     2.5),   37, 5,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     43, 4,  true),

        hero_bullet_event(direction_vector(0.05,    -1,     1.75),  49, 2,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     2.5),   55, 0,  false),
        hero_bullet_event(direction_vector(0.115,   -1,     2.5),   61, 5,  true),
        hero_bullet_event(direction_vector(-0.085,  -1,     2.5),   67, 1,  false),
        hero_bullet_event(direction_vector(0.025,   -1,     3.5),   73, 3,  true),
        hero_bullet_event(direction_vector(0.13,    -1,     2.5),   79, 0,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     2.5),   85, 5,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     91, 4,  true),
    };

    // 5 frames per bullet
    constexpr hero_bullet_event _level6_events[] = {
        hero_bullet_event(direction_vector(-0.05,   -1,     1.75),  1,  2,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     2.5),   6,  0,  false),
        hero_bullet_event(direction_vector(0.13,    -1,     2.5),   11, 5,  true),
        hero_bullet_event(direction_vector(0.085,   -1,     2.5),   16, 1,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     3),     21, 6,  true),
        hero_bullet_event(direction_vector(-0.025,  -1,     3.5),   26, 3,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     2.5),   31, 0,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     2.5),   36, 5,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     41, 4,  false),
        hero_bullet_event(direction_vector(0.115,   -1,     3),     46, 6,  true),

        hero_bullet_event(direction_vector(0.05,    -1,     1.75),  51, 2,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     2.5),   56, 0,  false),
        hero_bullet_event(direction_vector(0.13,    -1,     2.5),   61, 5,  true),
        hero_bullet_event(direction_vector(-0.085,  -1,     2.5),   66, 1,  false),
        hero_bullet_event(direction_vector(-0.115,  -1,     3),     71, 6,  true),
        hero_bullet_event(direction_vector(0.025,   -1,     3.5),   76, 3,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     2.5),   81, 0,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     2.5),   86, 5,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     91, 4,  false),
        hero_bullet_event(direction_vector(0.115,   -1,     3),     96, 6,  true),
    };

    // 4 frames per bullet
    constexpr hero_bullet_event _level7_events[] = {
        hero_bullet_event(direction_vector(0,       -1,     2.75),  1,  7,  true),
        hero_bullet_event(direction_vector(-0.075,  -1,     1.75),  5,  2,  false),
        hero_bullet_event(direction_vector(-0.16,   -1,     2.5),   9,  0,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     2.5),   13, 5,  true),
        hero_bullet_event(direction_vector(0.1,     -1,     2.5),   17, 1,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     3),     21, 6,  true),
        hero_bullet_event(direction_vector(-0.05,   -1,     3.5),   25, 3,  false),
        hero_bullet_event(direction_vector(0.16,    -1,     2.5),   29, 0,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     2.5),   33, 5,  false),
        hero_bullet_event(direction_vector(0.025,   -1,     2),     37, 4,  false),
        hero_bullet_event(direction_vector(0.13,    -1,     3),     41, 6,  false),

        hero_bullet_event(direction_vector(0,       -1,     2.75),  45, 7,  true),
        hero_bullet_event(direction_vector(0.075,   -1,     1.75),  49, 2,  false),
        hero_bullet_event(direction_vector(-0.16,   -1,     2.5),   53, 0,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     2.5),   57, 5,  true),
        hero_bullet_event(direction_vector(-0.1,    -1,     2.5),   61, 1,  false),
        hero_bullet_event(direction_vector(-0.13,   -1,     3),     65, 6,  true),
        hero_bullet_event(direction_vector(0.05,    -1,     3.5),   69, 3,  false),
        hero_bullet_event(direction_vector(0.16,    -1,     2.5),   73, 0,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     2.5),   77, 5,  false),
        hero_bullet_event(direction_vector(-0.025,  -1,     2),     81, 4,  false),
        hero_bullet_event(direction_vector(0.13,    -1,     3),     85, 6,  false),
    };

    // 3 frames per bullet
    constexpr hero_bullet_event _level8_events[] = {
        hero_bullet_event(direction_vector(0.025,   -1,     2.75),  1,  7,  true),
        hero_bullet_event(direction_vector(-0.1,    -1,     1.75),  4,  2,  false),
        hero_bullet_event(direction_vector(-0.175,  -1,     2.5),   7,  0,  false),
        hero_bullet_event(direction_vector(0.16,    -1,     2.5),   10, 5,  true),
        hero_bullet_event(direction_vector(0.115,   -1,     2.5),   13, 1,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     3),     16, 6,  false),
        hero_bullet_event(direction_vector(-0.075,  -1,     3.5),   19, 3,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     22, 8,  true),
        hero_bullet_event(direction_vector(0.175,   -1,     2.5),   25, 0,  false),
        hero_bullet_event(direction_vector(-0.16,   -1,     2.5),   28, 5,  false),
        hero_bullet_event(direction_vector(0.025,   -1,     2),     31, 4,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     3),     34, 6,  false),

        hero_bullet_event(direction_vector(-0.025,  -1,     2.75),  37, 7,  true),
        hero_bullet_event(direction_vector(0.1,     -1,     1.75),  40, 2,  false),
        hero_bullet_event(direction_vector(-0.175,  -1,     2.5),   43, 0,  false),
        hero_bullet_event(direction_vector(0.16,    -1,     2.5),   46, 5,  true),
        hero_bullet_event(direction_vector(-0.115,  -1,     2.5),   49, 1,  false),
        hero_bullet_event(direction_vector(-0.145,  -1,     3),     52, 6,  false),
        hero_bullet_event(direction_vector(0.075,   -1,     3.5),   55, 3,  false),
        hero_bullet_event(direction_vector(0,       -1,     2),     58, 8,  true),
        hero_bullet_event(direction_vector(0.175,   -1,     2.5),   61, 0,  false),
        hero_bullet_event(direction_vector(-0.16,   -1,     2.5),   64, 5,  false),
        hero_bullet_event(direction_vector(-0.025,  -1,     2),     67, 4,  false),
        hero_bullet_event(direction_vector(0.145,   -1,     3),     70, 6,  false),
    };

    constexpr hero_bullet_level _levels[] = {
        hero_bullet_level(_level0_events,   bn::fixed_size(9, 9),      bn::sound_items::gun_5,            4,
                bn::color(31, 28, 5),  20,     1),
        hero_bullet_level(_level1_events,   bn::fixed_size(16, 16),    bn::sound_items::gun_5,            250,
                bn::color(31, 28, 5),  40,     2),
        hero_bullet_level(_level2_events,   bn::fixed_size(13, 14),    bn::sound_items::gun_1,            850,
                bn::color(13, 2, 2),   80,     4),
        hero_bullet_level(_level3_events,   bn::fixed_size(11, 15),    bn::sound_items::gun_3,            1200,
                bn::color(31, 0, 6),   80,     3),
        hero_bullet_level(_level4_events,   bn::fixed_size(16, 16),    bn::sound_items::space_shooter_3,  2100,
                bn::color(18, 25, 27), 86,     5),
        hero_bullet_level(_level5_events,   bn::fixed_size(16, 16),    bn::sound_items::gun_3,            3300,
                bn::color(31, 0, 0),   97,     3),
        hero_bullet_level(_level6_events,   bn::fixed_size(15, 15),    bn::sound_items::gun_2,            5300,
                bn::color(9, 20, 24),  101,    4),
        hero_bullet_level(_level7_events,   bn::fixed_size(10, 16),    bn::sound_items::space_shooter_5,  7200,
                bn::color(31, 0, 0),   89,     6),
        hero_bullet_level(_level8_events,   bn::fixed_size(16, 16),    bn::sound_items::gun_1,            8800,
                bn::color(9, 20, 24),  73,     8),
    };

    [[nodiscard]] int _multiplier(int level)
    {
        switch(level)
        {

        case 0:
            return 1;

        case 1:
            return 5;

        case 2:
            return 10;

        case 3:
            return 15;

        case 4:
            return 20;

        case 5:
            return 25;

        case 6:
            return 30;

        case 7:
            return 35;

        default:
            return 40;
        }
    }
}

bn::span<const hero_bullet_level> hero_bullet_level::all_levels()
{
    return _levels;
}

int hero_bullet_level::gem_experience(int level, bn::fixed y)
{
    int height = y.right_shift_integer() + constants::play_height;
    int height_level = bn::clamp(height / 16, 0, 4);
    height_level = 4 + 1 - height_level;
    return height_level * _multiplier(level);
}

}
