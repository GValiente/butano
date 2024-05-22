/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_stage_3.h"

#include "bn_display.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_stage_3.h"
#include "bn_sprite_items_stage_3_intro.h"
#include "bn_sprite_items_stage_3_blonde.h"
#include "bn_sprite_items_stage_3_intro_alt.h"
#include "bn_sprite_items_stage_3_monsters_1.h"
#include "bn_sprite_items_stage_3_monsters_2.h"
#include "bn_sprite_items_stage_3_dragon_man.h"
#include "bn_regular_bg_items_mountain_clouds.h"
#include "bn_sprite_items_stage_3_characters_1.h"
#include "bn_sprite_items_stage_3_characters_2.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_3
{

namespace
{
    constexpr int start_y = -(bn::display::height() + 64) / 2;

    constexpr bn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr bn::fixed move_y(bn::fixed y)
    {
        return y + constants::background_speed;
    }

    [[nodiscard]] constexpr bn::array<enemy_data::graphics_indexes_group, 4> _create_graphics_indexes(
        int initial_index)
    {
        return bn::array<enemy_data::graphics_indexes_group, 4> {
            enemy_data::graphics_indexes_group{ uint16_t(initial_index), uint16_t(initial_index + 1),
                        uint16_t(initial_index), uint16_t(initial_index + 2) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index), uint16_t(initial_index + 1),
                        uint16_t(initial_index), uint16_t(initial_index + 2) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index), uint16_t(initial_index + 1),
                        uint16_t(initial_index), uint16_t(initial_index + 2) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index), uint16_t(initial_index + 1),
                        uint16_t(initial_index), uint16_t(initial_index + 2) }
        };
    }


    // blonde:

    constexpr enemy_data blonde(
        bn::sprite_items::stage_3_blonde, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_14, bn::fixed_size(46, 62), _create_graphics_indexes(0), 10, 10);

    constexpr enemy_move_event blonde_moves[] = {
        enemy_move_event(bn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(bn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr enemy_move_event blonde_flipped_moves[] = {
        blonde_moves[0].flipped(),
        blonde_moves[1].flipped(),
    };


    // white_caveman:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> white_caveman_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 },
        enemy_data::graphics_indexes_group{ 5, 5, 5, 5 },
        enemy_data::graphics_indexes_group{ 3, 3, 4, 4 },
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 }
    };

    constexpr enemy_data white_caveman(
        bn::sprite_items::stage_3_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_4, bn::fixed_size(38, 52), white_caveman_graphics_indexes, 22, 22);

    constexpr enemy_move_event white_caveman_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  30, 0,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    5,  1,  false),
        enemy_move_event(bn::fixed_point(1, move_y(0.5)),  60, 2,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    5,  1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };

    constexpr enemy_move_event white_caveman_flipped_moves[] = {
        white_caveman_moves[0].flipped(),
        white_caveman_moves[1].flipped(),
        white_caveman_moves[2].flipped(),
        white_caveman_moves[3].flipped(),
        white_caveman_moves[4].flipped(),
    };


    // black_caveman:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> black_caveman_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 9, 10, 11, 11 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 }
    };

    constexpr enemy_data black_caveman(
        bn::sprite_items::stage_3_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_5, bn::fixed_size(38, 52), black_caveman_graphics_indexes, 12, 16);

    constexpr enemy_move_event black_caveman_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  45, 0,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    30, 1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };

    constexpr enemy_move_event black_caveman_flipped_moves[] = {
        black_caveman_moves[0].flipped(),
        black_caveman_moves[1].flipped(),
        black_caveman_moves[2].flipped(),
    };

    constexpr enemy_bullet_event black_caveman_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_BIG,      0.9,    45 + 15),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,    0.9,    8),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,    0.9,    8),
    };

    constexpr enemy_bullet_event black_caveman_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_BIG,      0.9,    45 + 15),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    0.9,    8),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    0.9,    8),
    };


    // green_cavegirl:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> green_cavegirl_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 12, 13, 12, 14 },
        enemy_data::graphics_indexes_group{ 15, 15, 15, 15 },
        enemy_data::graphics_indexes_group{ 12, 13, 12, 14 },
        enemy_data::graphics_indexes_group{ 12, 13, 12, 14 },
    };

    constexpr enemy_data green_cavegirl(
        bn::sprite_items::stage_3_characters_1, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_11, bn::fixed_size(46, 60), green_cavegirl_graphics_indexes, 12, 12);

    constexpr enemy_move_event green_cavegirl_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  60, 0,  false),

        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  1,  true),

        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };

    constexpr enemy_move_event green_cavegirl_flipped_moves[] = {
        green_cavegirl_moves[0].flipped(),

        green_cavegirl_moves[1].flipped(),
        green_cavegirl_moves[2].flipped(),
        green_cavegirl_moves[3].flipped(),
        green_cavegirl_moves[4].flipped(),
        green_cavegirl_moves[5].flipped(),
        green_cavegirl_moves[6].flipped(),
        green_cavegirl_moves[7].flipped(),
        green_cavegirl_moves[8].flipped(),

        green_cavegirl_moves[9].flipped(),
    };

    constexpr enemy_bullet_event green_cavegirl_few_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 1, 0.9),   60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, 1, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 0, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 0, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, -1, 0.9),  4),
    };

    constexpr enemy_bullet_event green_cavegirl_more_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 1, 0.9),   60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 1, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, 1, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, -1, 0.9), 4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 0, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, -1, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 0, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, -1, 0.9),  4),
    };


    // cavebat:

    constexpr enemy_data cavebat(bn::sprite_items::stage_3_monsters_1, enemy_data::death_anim_type::ROTATE,
                                       bn::sound_items::dinosaur_5, bn::fixed_size(26, 18), 0, 1, 8, 10);

    constexpr enemy_move_event cavebat_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.25)), 1, false),
    };

    constexpr enemy_move_event cavebat_flipped_moves[] = {
        cavebat_moves[0].flipped()
    };

    constexpr enemy_bullet_event cavebat_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 0.9,                           60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, 1, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 1, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 1, 0.9),   4),
    };


    // grasshopper:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> grasshopper_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 2, 2, 2, 2 },
        enemy_data::graphics_indexes_group{ 3, 4, 3, 4 },
        enemy_data::graphics_indexes_group{ 2, 2, 2, 2 },
        enemy_data::graphics_indexes_group{ 2, 2, 2, 2 },
    };

    constexpr enemy_data grasshopper(
        bn::sprite_items::stage_3_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::dinosaur_4, bn::fixed_size(56, 22), grasshopper_graphics_indexes, 22, 22);

    constexpr enemy_move_event grasshopper_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, 0,  false),

        enemy_move_event(bn::fixed_point(1.5,  move_y(0.7)),   30, 1,  false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, 0,  false),

        enemy_move_event(bn::fixed_point(-1.5, move_y(0.7)),   30, 1,  true),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, 0,  true),

        enemy_move_event(bn::fixed_point(1.5,  move_y(0.7)),   30, 1,  false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, 0,  false),

        enemy_move_event(bn::fixed_point(-1.5, move_y(0.7)),   30, 1,  true),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, 0,  true),
    };

    constexpr enemy_move_event grasshopper_flipped_moves[] = {
        grasshopper_moves[0].flipped(),

        grasshopper_moves[1].flipped(),
        grasshopper_moves[2].flipped(),

        grasshopper_moves[3].flipped(),
        grasshopper_moves[4].flipped(),

        grasshopper_moves[5].flipped(),
        grasshopper_moves[6].flipped(),

        grasshopper_moves[7].flipped(),
        grasshopper_moves[8].flipped(),
    };


    // lizard:

    constexpr enemy_data lizard(
        bn::sprite_items::stage_3_monsters_1, enemy_data::death_anim_type::HORIZONTAL_SCALE,
        bn::sound_items::dinosaur_3, bn::fixed_size(62, 42), _create_graphics_indexes(5), 28, 28);

    constexpr enemy_move_event lizard_moves[] = {
        enemy_move_event(bn::fixed_point(0.3,  move_y(0.5)),   1,  false),
    };

    constexpr enemy_move_event lizard_flipped_moves[] = {
        lizard_moves[0].flipped()
    };


    // dragon_man:

    constexpr enemy_data dragon_man(
        bn::sprite_items::stage_3_dragon_man, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::dinosaur_2, bn::fixed_size(42, 62), _create_graphics_indexes(0), 280, 200);

    constexpr enemy_move_event dragon_man_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 270, true),
        enemy_move_event(bn::fixed_point(-0.2, 0),          240, true),
        enemy_move_event(bn::fixed_point(-0.2, 0),          240, false),
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr enemy_bullet_event dragon_man_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0, 1, 1),     100),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 1, 1),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 1, 1),   1),

        enemy_bullet_event(enemy_bullet_type::BIG,      1,  90),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,  10),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,  10),

        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0, 1, 1),     90),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 1, 1),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 1, 1),   1),

        enemy_bullet_event(enemy_bullet_type::HUGE,      0.9,  90),

        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0, 1, 1),     90),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 1, 1),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 1, 1),   1),

        enemy_bullet_event(enemy_bullet_type::BIG,      1,  90),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,  10),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,  10),

        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0, 1, 1),     90),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 1, 1),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 1, 1),   1),
    };

    // red_cavegirl:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> red_cavegirl_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 },
        enemy_data::graphics_indexes_group{ 4, 4, 4, 4 },
        enemy_data::graphics_indexes_group{ 5, 5, 5, 5 },
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 },
    };

    constexpr enemy_data red_cavegirl(
        bn::sprite_items::stage_3_characters_2, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_14, bn::fixed_size(46, 60), red_cavegirl_graphics_indexes, 14, 16);

    constexpr enemy_move_event red_cavegirl_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  60, 0,  false),

        enemy_move_event(bn::fixed_point(0, move_y(0)),    10, 2,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  50, 0,  false),

        enemy_move_event(bn::fixed_point(0, move_y(0)),    10, 1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  50, 0,  false),
    };

    constexpr enemy_move_event red_cavegirl_flipped_moves[] = {
        red_cavegirl_moves[0].flipped(),

        red_cavegirl_moves[1].flipped(),
        red_cavegirl_moves[2].flipped(),

        red_cavegirl_moves[3].flipped(),
        red_cavegirl_moves[4].flipped(),
    };

    constexpr enemy_bullet_event red_cavegirl_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    1, 60),
        enemy_bullet_event(enemy_bullet_type::BIG,      1, 60),
    };

    // egg_caveman:

    constexpr bn::array<enemy_data::graphics_indexes_group, 4> egg_caveman_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 9, 10, 11, 11 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 }
    };

    constexpr enemy_data egg_caveman(
        bn::sprite_items::stage_3_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_6, bn::fixed_size(42, 62), egg_caveman_graphics_indexes, 18, 18);

    constexpr enemy_move_event egg_caveman_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  45, 0,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    30, 1,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };

    constexpr enemy_move_event egg_caveman_flipped_moves[] = {
        egg_caveman_moves[0].flipped(),
        egg_caveman_moves[1].flipped(),
        egg_caveman_moves[2].flipped(),
    };

    constexpr enemy_bullet_event egg_caveman_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,            direction_vector(1, 0, 0.9),        45 + 15),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,            direction_vector(0.5, 0.5, 0.9),    1),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,            direction_vector(0, 1, 0.9),        1),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,            0.9,                                1),
    };

    constexpr enemy_bullet_event egg_caveman_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    direction_vector(-1, 0, 0.9),       45 + 15),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    direction_vector(-0.5, 0.5, 0.9),   1),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    direction_vector(0, 1, 0.9),        1),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_FLIPPED_SMALL,    0.9,                                1),
    };


    // minidino:

    constexpr enemy_data minidino(bn::sprite_items::stage_3_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                        bn::sound_items::dinosaur_2, bn::fixed_size(58, 46), _create_graphics_indexes(0), 23, 26);

    constexpr enemy_move_event minidino_moves[] = {
        enemy_move_event(bn::fixed_point(1.2, move_y(0.6)), 60, false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.6)), 60, true),
        enemy_move_event(bn::fixed_point(1.2, move_y(0.6)), 60, false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.6)), 1, true),
    };

    constexpr enemy_move_event minidino_flipped_moves[] = {
        minidino_moves[0].flipped(),
        minidino_moves[1].flipped(),
        minidino_moves[2].flipped(),
        minidino_moves[3].flipped(),
    };

    constexpr enemy_bullet_event minidino_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 0.5, 0.9),     60),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-1, 0, 0.9),         1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, -0.5, 0.9),    1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    0.9,                                  1),

        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 0.5, 0.9),      57),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(1, 0, 0.9),          1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, -0.5, 0.9),     1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    0.9,                                  1),
    };

    constexpr enemy_bullet_event minidino_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, 0.5, 0.9),      60),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(1, 0, 0.9),          1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0.5, -0.5, 0.9),     1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    0.9,                                  1),

        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, 0.5, 0.9),     57),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-1, 0, 0.9),         1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-0.5, -0.5, 0.9),    1),
        enemy_bullet_event(enemy_bullet_type::SMALL,    0.9,                                  1),
    };


    // turtle:

    constexpr enemy_data turtle(bn::sprite_items::stage_3_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      bn::sound_items::enemy_death_rpg_12, bn::fixed_size(52, 30), _create_graphics_indexes(3), 22, 22);

    constexpr enemy_move_event turtle_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.4)), 1, false),
    };

    constexpr enemy_move_event turtle_flipped_moves[] = {
        turtle_moves[0].flipped()
    };


    // enemy events:

    constexpr enemy_event enemy_events[] = {

        // level 5 slow (22s)

        enemy_event(grasshopper, bn::fixed_point(-20, start_y), grasshopper_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(grasshopper, bn::fixed_point(50, start_y), grasshopper_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(cavebat, bn::fixed_point(-35, start_y), cavebat_moves, cavebat_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(35, start_y), cavebat_flipped_moves, cavebat_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(-55, start_y), blonde_moves, no_bullets, 85, enemy_drop_type::GEM),

        enemy_event(white_caveman, bn::fixed_point(-35, start_y), white_caveman_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(white_caveman, bn::fixed_point(35, start_y), white_caveman_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(-55, start_y), blonde_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(cavebat, bn::fixed_point(-50, start_y), cavebat_moves, cavebat_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(0, start_y), cavebat_flipped_moves, cavebat_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(50, start_y), cavebat_flipped_moves, cavebat_bullets, 70, enemy_drop_type::NONE),

        enemy_event(grasshopper, bn::fixed_point(-20, start_y), grasshopper_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(grasshopper, bn::fixed_point(50, start_y), grasshopper_flipped_moves, no_bullets, 70, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 70, enemy_drop_type::NONE),
        enemy_event(white_caveman, bn::fixed_point(-35, start_y), white_caveman_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 70, enemy_drop_type::NONE),
        enemy_event(white_caveman, bn::fixed_point(35, start_y), white_caveman_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 90, enemy_drop_type::GEM),

        // level 5 fast (23s)

        enemy_event(lizard, bn::fixed_point(35, start_y), lizard_flipped_moves, no_bullets, 50, enemy_drop_type::NONE),
        enemy_event(lizard, bn::fixed_point(-35, start_y), lizard_moves, no_bullets, 50, enemy_drop_type::NONE),

        enemy_event(cavebat, bn::fixed_point(-50, start_y), cavebat_moves, cavebat_bullets, 50, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(50, start_y), cavebat_flipped_moves, cavebat_bullets, 50, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(-20, start_y), cavebat_flipped_moves, cavebat_bullets, 70, enemy_drop_type::NONE),

        enemy_event(green_cavegirl, bn::fixed_point(35, start_y), green_cavegirl_flipped_moves, green_cavegirl_more_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(-35, start_y), green_cavegirl_moves, green_cavegirl_more_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 80, enemy_drop_type::GEM),

        enemy_event(grasshopper, bn::fixed_point(50, start_y), grasshopper_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(grasshopper, bn::fixed_point(-50, start_y), grasshopper_moves, no_bullets, 80, enemy_drop_type::NONE),

        enemy_event(green_cavegirl, bn::fixed_point(35, start_y), green_cavegirl_flipped_moves, green_cavegirl_more_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(-35, start_y), green_cavegirl_moves, green_cavegirl_few_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(50, start_y), green_cavegirl_flipped_moves, green_cavegirl_few_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(-50, start_y), green_cavegirl_moves, green_cavegirl_more_bullets, 70, enemy_drop_type::NONE),

        enemy_event(white_caveman, bn::fixed_point(35, start_y), white_caveman_flipped_moves, no_bullets, 55, enemy_drop_type::NONE),
        enemy_event(white_caveman, bn::fixed_point(-35, start_y), white_caveman_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(35, start_y), lizard_flipped_moves, no_bullets, 50, enemy_drop_type::NONE),
        enemy_event(lizard, bn::fixed_point(-35, start_y), lizard_moves, no_bullets, 50, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(0, start_y), green_cavegirl_moves, green_cavegirl_few_bullets, 50, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 50, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(0, start_y), green_cavegirl_moves, green_cavegirl_few_bullets, 50, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 50, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 70, enemy_drop_type::GEM),

        // level 5 mid boss (15s)

        enemy_event(dragon_man, bn::fixed_point(50, start_y), dragon_man_moves, dragon_man_bullets, 0, enemy_drop_type::HERO_BOMB),

        // level 6 slow (25s)

        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-35, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(35, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-50, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(50, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 80, enemy_drop_type::NONE),

        enemy_event(minidino, bn::fixed_point(-35, start_y), minidino_moves, minidino_bullets, 60, enemy_drop_type::NONE),
        enemy_event(minidino, bn::fixed_point(35, start_y), minidino_flipped_moves, minidino_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 90, enemy_drop_type::GEM),

        enemy_event(lizard, bn::fixed_point(35, start_y), lizard_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(lizard, bn::fixed_point(-35, start_y), lizard_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-35, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(35, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_cavegirl, bn::fixed_point(50, start_y), green_cavegirl_flipped_moves, green_cavegirl_more_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(-50, start_y), green_cavegirl_moves, green_cavegirl_more_bullets, 80, enemy_drop_type::NONE),

        enemy_event(minidino, bn::fixed_point(35, start_y), minidino_flipped_moves, minidino_flipped_bullets, 60, enemy_drop_type::NONE),
        enemy_event(minidino, bn::fixed_point(-35, start_y), minidino_moves, minidino_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 70, enemy_drop_type::GEM),

        enemy_event(grasshopper, bn::fixed_point(50, start_y), grasshopper_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(grasshopper, bn::fixed_point(-50, start_y), grasshopper_moves, no_bullets, 70, enemy_drop_type::NONE),

        enemy_event(cavebat, bn::fixed_point(-50, start_y), cavebat_moves, cavebat_bullets, 1, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(50, start_y), cavebat_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(cavebat, bn::fixed_point(-35, start_y), cavebat_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(cavebat, bn::fixed_point(35, start_y), cavebat_flipped_moves, cavebat_bullets, 60, enemy_drop_type::NONE),

        enemy_event(cavebat, bn::fixed_point(50, start_y), cavebat_flipped_moves, cavebat_bullets, 80, enemy_drop_type::NONE),

        enemy_event(minidino, bn::fixed_point(-50, start_y), minidino_moves, minidino_bullets, 60, enemy_drop_type::NONE),
        enemy_event(minidino, bn::fixed_point(50, start_y), minidino_flipped_moves, minidino_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 70, enemy_drop_type::GEM),

        // level 6 fast (27s)

        enemy_event(egg_caveman, bn::fixed_point(50, start_y), egg_caveman_flipped_moves, egg_caveman_flipped_bullets, 50, enemy_drop_type::NONE),

        enemy_event(egg_caveman, bn::fixed_point(-50, start_y), egg_caveman_moves, egg_caveman_bullets, 50, enemy_drop_type::NONE),
        enemy_event(egg_caveman, bn::fixed_point(50, start_y), egg_caveman_flipped_moves, egg_caveman_flipped_bullets, 70, enemy_drop_type::NONE),

        enemy_event(white_caveman, bn::fixed_point(-35, start_y), white_caveman_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(white_caveman, bn::fixed_point(35, start_y), white_caveman_flipped_moves, no_bullets, 70, enemy_drop_type::NONE),

        enemy_event(green_cavegirl, bn::fixed_point(-50, start_y), green_cavegirl_moves, green_cavegirl_more_bullets, 50, enemy_drop_type::NONE),
        enemy_event(green_cavegirl, bn::fixed_point(50, start_y), green_cavegirl_flipped_moves, green_cavegirl_more_bullets, 50, enemy_drop_type::NONE),

        enemy_event(red_cavegirl, bn::fixed_point(-35, start_y), red_cavegirl_moves, red_cavegirl_bullets, 50, enemy_drop_type::NONE),
        enemy_event(red_cavegirl, bn::fixed_point(35, start_y), red_cavegirl_flipped_moves, red_cavegirl_bullets, 70, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 80, enemy_drop_type::GEM),

        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-55, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(black_caveman, bn::fixed_point(-50, start_y), black_caveman_moves, black_caveman_bullets, 50, enemy_drop_type::NONE),
        enemy_event(egg_caveman, bn::fixed_point(50, start_y), egg_caveman_flipped_moves, egg_caveman_flipped_bullets, 50, enemy_drop_type::NONE),

        enemy_event(egg_caveman, bn::fixed_point(-50, start_y), egg_caveman_moves, egg_caveman_bullets, 50, enemy_drop_type::NONE),
        enemy_event(black_caveman, bn::fixed_point(50, start_y), black_caveman_flipped_moves, black_caveman_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(grasshopper, bn::fixed_point(50, start_y), grasshopper_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(grasshopper, bn::fixed_point(-50, start_y), grasshopper_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(minidino, bn::fixed_point(35, start_y), minidino_flipped_moves, minidino_flipped_bullets, 50, enemy_drop_type::NONE),
        enemy_event(minidino, bn::fixed_point(-35, start_y), minidino_moves, minidino_bullets, 50, enemy_drop_type::NONE),

        enemy_event(red_cavegirl, bn::fixed_point(50, start_y), red_cavegirl_flipped_moves, red_cavegirl_bullets, 50, enemy_drop_type::NONE),
        enemy_event(red_cavegirl, bn::fixed_point(-50, start_y), red_cavegirl_moves, red_cavegirl_bullets, 60, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-55, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 50, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-55, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 50, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-55, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 50, enemy_drop_type::NONE),

        enemy_event(turtle, bn::fixed_point(-55, start_y), turtle_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(0, start_y), turtle_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(turtle, bn::fixed_point(55, start_y), turtle_flipped_moves, no_bullets, 70, enemy_drop_type::NONE),

        enemy_event(blonde, bn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 50, enemy_drop_type::GEM),
    };


    // stage (start with level 5, exp 2350):

    constexpr stage stage_3(
        bn::regular_bg_items::stage_3, bn::fixed_point(0, constants::background_speed),
        bn::regular_bg_items::mountain_clouds, bn::fixed_point(-1.0 / 16, constants::background_speed),
        bn::sprite_items::stage_3_intro, bn::sprite_items::stage_3_intro_alt, "STAGE 3", "GIANT MOUNTAIN",
        bn::music_items::stardstm, 0.4, enemy_events, boss::type::WIZARD, false);
}

const stage& get()
{
    return stage_3;
}

}
