/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_stage_2.h"

#include "bn_display.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_clouds.h"
#include "bn_regular_bg_items_stage_2.h"
#include "bn_sprite_items_stage_2_intro.h"
#include "bn_sprite_items_stage_2_robot.h"
#include "bn_sprite_items_stage_2_intro_alt.h"
#include "bn_sprite_items_stage_2_monsters_1.h"
#include "bn_sprite_items_stage_2_monsters_2.h"
#include "bn_sprite_items_stage_2_characters_1.h"
#include "bn_sprite_items_stage_2_characters_2.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_2
{

namespace
{
    constexpr int down_index = 0;
    constexpr int up_index = 1;
    constexpr int left_index = 2;
    constexpr int right_index = 3;
    constexpr int start_y = -(bn::display::height() + 16) / 2;

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
                        uint16_t(initial_index + 2), uint16_t(initial_index + 3) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index + 4), uint16_t(initial_index + 5),
                        uint16_t(initial_index + 6), uint16_t(initial_index + 7) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index + 8), uint16_t(initial_index + 9),
                        uint16_t(initial_index + 10), uint16_t(initial_index + 11) },
            enemy_data::graphics_indexes_group{ uint16_t(initial_index + 12), uint16_t(initial_index + 13),
                        uint16_t(initial_index + 14), uint16_t(initial_index + 15) },
        };
    }


    // rat:

    constexpr enemy_data rat(
        bn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::dinosaur_5, bn::fixed_size(16, 16), _create_graphics_indexes(0), 2, 2);

    constexpr enemy_move_event rat_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(bn::fixed_point(1,    move_y(0)),     70, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(-1.5)),  1,  up_index,       false),
    };


    // beaver:

    constexpr enemy_data beaver(
        bn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::enemy_death_rpg_25, bn::fixed_size(16, 15), _create_graphics_indexes(16), 5, 5);

    constexpr enemy_move_event beaver_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(bn::fixed_point(2,    move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(2,    move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(2,    move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };

    constexpr enemy_move_event beaver_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(bn::fixed_point(-2,   move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(-2,   move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(-2,   move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };


    // fire:

    constexpr enemy_data fire(
        bn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::HORIZONTAL_SCALE,
        bn::sound_items::enemy_death_rpg_12, bn::fixed_size(16, 14), _create_graphics_indexes(32), 7, 7);

    constexpr enemy_move_event fire_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     4,  down_index,     false),
    };

    constexpr enemy_bullet_event fire_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, 1, 0.9),   44 + 20 + 20 + 20 + 4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 1, 0.9),   1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 0, 0.9),   1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, -1, 0.9),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, -1, 0.9),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, -1, 0.9), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 0, 0.9),  1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 1, 0.9),  1),
    };


    // eye:

    constexpr enemy_data eye(
        bn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::dinosaur_5, bn::fixed_size(16, 14), _create_graphics_indexes(48), 6, 6);

    constexpr enemy_move_event eye_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(bn::fixed_point(0.5,  move_y(0)),     70, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr enemy_move_event eye_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(bn::fixed_point(-0.5, move_y(0)),     70, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr enemy_bullet_event eye_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
    };

    constexpr enemy_bullet_event eye_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
    };


    // green_ninja:

    constexpr enemy_data green_ninja(
        bn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_4, bn::fixed_size(14, 14), _create_graphics_indexes(0), 2, 4);

    constexpr enemy_move_event green_ninja_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60,     down_index,     false),
        enemy_move_event(bn::fixed_point(0.5,  move_y(0)),     120,    right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  1,      down_index,     false),
    };

    constexpr enemy_move_event green_ninja_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60,     down_index,     false),
        enemy_move_event(bn::fixed_point(-0.5, move_y(0)),     120,    left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  1,      down_index,     false),
    };

    constexpr enemy_bullet_event green_ninja_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 120),
    };


    // old_woman:

    constexpr enemy_data old_woman(
        bn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_14, bn::fixed_size(14, 16), _create_graphics_indexes(16), 3, 5);

    constexpr enemy_move_event old_woman_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     12, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(-0.75)), 1,  up_index,       false),
    };

    constexpr enemy_move_event old_woman_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     12, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(-0.75)), 1,  up_index,       false),
    };

    constexpr enemy_bullet_event old_woman_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG, 1, 40 + 30 + 30),
    };


    // bald:

    constexpr enemy_data bald(
        bn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_6, bn::fixed_size(14, 15), _create_graphics_indexes(32), 12, 10);

    constexpr enemy_move_event bald_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.3)),   60, down_index,     false),
        enemy_move_event(bn::fixed_point(0.6,  move_y(0)),     60, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(-0.8)),  60, up_index,       false),
        enemy_move_event(bn::fixed_point(-0.6, move_y(0)),     60, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.3)),   1,  down_index,     false),
    };

    constexpr enemy_move_event bald_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.3)),   60, down_index,     false),
        enemy_move_event(bn::fixed_point(-0.6, move_y(0)),     60, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(-0.8)),  60, up_index,       false),
        enemy_move_event(bn::fixed_point(0.6,  move_y(0)),     60, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.3)),   1,  down_index,     false),
    };


    // dog:

    constexpr enemy_data dog(
        bn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::monster_2, bn::fixed_size(12, 10), 48, 49, 2, 4);

    constexpr enemy_move_event dog_moves[] = {
        enemy_move_event(bn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(bn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr enemy_move_event dog_flipped_moves[] = {
        dog_moves[0].flipped(),
        dog_moves[1].flipped(),
    };


    // robot:

    constexpr enemy_data robot(
        bn::sprite_items::stage_2_robot, enemy_data::death_anim_type::EXPLOSION,
        bn::sound_items::boss_shoot, bn::fixed_size(26, 47), 0, 1, 150, 75, 4);

    constexpr enemy_move_event robot_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 220, false),
        enemy_move_event(bn::fixed_point(0.25, 0),          160, false),
        enemy_move_event(bn::fixed_point(-0.25, 0),         320, false),
        enemy_move_event(bn::fixed_point(0.25, 0),          160, false),
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr enemy_bullet_event robot_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),

        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 100),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
        enemy_bullet_event(enemy_bullet_type::ROBOT_SMALL, direction_vector(0, 1, 0.85), 14),
    };


    // black_ninja:

    constexpr enemy_data black_ninja(
        bn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_5, bn::fixed_size(14, 14), _create_graphics_indexes(0), 3, 6);

    constexpr enemy_move_event black_ninja_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.35)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0.7,  move_y(0)),     80, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.35)),  1,  down_index,     false),
    };

    constexpr enemy_move_event black_ninja_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.35)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(-0.7, move_y(0)),     80, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.35)),  1,  down_index,     false),
    };

    constexpr enemy_bullet_event black_ninja_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 40),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 80),
    };


    // dog_mask:

    constexpr enemy_data dog_mask(
        bn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::monster_2, bn::fixed_size(14, 14), _create_graphics_indexes(16), 6, 6);

    constexpr enemy_move_event dog_mask_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(bn::fixed_point(0.5,  move_y(0)),     70, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr enemy_move_event dog_mask_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(bn::fixed_point(-0.5, move_y(0)),     70, left_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr enemy_bullet_event dog_mask_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(0.5),    0.9),   20),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0),      0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(-0.5),   0.9),   30),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1),     0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(-1),     0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(-1),     0.9),   20),
    };

    constexpr enemy_bullet_event dog_mask_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(0.5),    0.9),   20),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0),      0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0.5),    0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(-0.5),   0.9),   30),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1),     0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(-1),     0.9),   20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(-1),     0.9),   20),
    };


    // samurai:

    constexpr enemy_data samurai(
        bn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_2, bn::fixed_size(16, 15), _create_graphics_indexes(32), 7, 8);

    constexpr enemy_move_event samurai_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     1,  down_index,     false),
    };

    constexpr enemy_move_event samurai_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),     1,  down_index,     false),
    };

    constexpr enemy_bullet_event samurai_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 40 + 30 + 30),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 12),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 12),
    };


    // red_ninja:

    constexpr enemy_data red_ninja(
        bn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_4, bn::fixed_size(12, 15), _create_graphics_indexes(48), 4, 6);

    constexpr enemy_move_event red_ninja_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };

    constexpr enemy_move_event red_ninja_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(-1.2, move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, right_index,    false),
        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, up_index,       false),
        enemy_move_event(bn::fixed_point(1.2,  move_y(0.5)),   10, left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };


    // green_snake:

    constexpr enemy_data green_snake(
        bn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::dinosaur_4, bn::fixed_size(14, 15), _create_graphics_indexes(0), 5, 7);

    constexpr enemy_move_event green_snake_moves[] = {
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.75,     move_y(0.25)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.75,    move_y(0.25)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  1,   down_index,     false),
    };

    constexpr enemy_move_event green_snake_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.75,    move_y(0.25)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.75,     move_y(0.25)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,        move_y(0.25)),  1,   down_index,     false),
    };


    // lizard:

    constexpr enemy_data lizard(
        bn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::dinosaur_5, bn::fixed_size(12, 15), _create_graphics_indexes(16), 3, 5);

    constexpr enemy_move_event lizard_moves[] = {
        enemy_move_event(bn::fixed_point(0.75,     move_y(0.15)), 35, down_index,  false),
        enemy_move_event(bn::fixed_point(0.75,     move_y(0.0)),  55, right_index, false),
        enemy_move_event(bn::fixed_point(0.75,     move_y(-1.1)), 1,  up_index,    false),
    };


    // red_snake:

    constexpr enemy_data red_snake(
        bn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::dinosaur_4, bn::fixed_size(14, 15), _create_graphics_indexes(32), 4, 7);

    constexpr enemy_move_event red_snake_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.5,  move_y(0.2)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.5, move_y(0.2)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  1,   down_index,     false),
    };

    constexpr enemy_move_event red_snake_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.5, move_y(0.2)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.5,  move_y(0.2)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0.2)),  1,   down_index,     false),
    };

    constexpr enemy_bullet_event red_snake_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // ogre:

    constexpr enemy_data ogre(
        bn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_3, bn::fixed_size(16, 16), _create_graphics_indexes(48), 20, 12);

    constexpr enemy_data ogre_weak(
        bn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_3, bn::fixed_size(16, 16), _create_graphics_indexes(48), 12, 10);

    constexpr enemy_move_event ogre_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.2,  move_y(0)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.2, move_y(0)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.2,  move_y(0)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.2, move_y(0)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  1,   down_index,     false),
    };

    constexpr enemy_move_event ogre_flipped_moves[] = {
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.2, move_y(0)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.2,  move_y(0)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(-0.2, move_y(0)),  30,  left_index,     false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  30,  down_index,     false),
        enemy_move_event(bn::fixed_point(0.2,  move_y(0)),  30,  right_index,    false),
        enemy_move_event(bn::fixed_point(0,    move_y(0)),  1,   down_index,     false),
    };


    // enemy events:

    constexpr enemy_event enemy_events[] = {

        // level 3 slow (25s)

        enemy_event(rat, bn::fixed_point(-45, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(rat, bn::fixed_point(-30, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, bn::fixed_point(40, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(beaver, bn::fixed_point(55, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(beaver, bn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, bn::fixed_point(-45, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(-60, start_y), green_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(-30, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, bn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(55, start_y), green_ninja_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(old_woman, bn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, bn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, bn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, bn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(eye, bn::fixed_point(-30, start_y), eye_moves, eye_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, bn::fixed_point(-30, start_y), beaver_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(eye, bn::fixed_point(25, start_y), eye_flipped_moves, eye_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, bn::fixed_point(-45, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(-30, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, bn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(old_woman, bn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, bn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, bn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 75, enemy_drop_type::NONE),

        // level 3 fast (25s)

        enemy_event(bald, bn::fixed_point(5, start_y), bald_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, bn::fixed_point(55, start_y), bald_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, bn::fixed_point(-60, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, bn::fixed_point(-45, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, bn::fixed_point(55, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, bn::fixed_point(25, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, bn::fixed_point(-45, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, bn::fixed_point(-30, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(rat, bn::fixed_point(-45, start_y), rat_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(rat, bn::fixed_point(-30, start_y), rat_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(green_ninja, bn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(55, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),
        enemy_event(green_ninja, bn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),

        enemy_event(beaver, bn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(eye, bn::fixed_point(-30, start_y), eye_moves, eye_bullets, 45, enemy_drop_type::NONE),

        enemy_event(beaver, bn::fixed_point(-30, start_y), beaver_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(eye, bn::fixed_point(55, start_y), eye_flipped_moves, eye_flipped_bullets, 45, enemy_drop_type::NONE),

        enemy_event(bald, bn::fixed_point(-60, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, bn::fixed_point(-45, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, bn::fixed_point(55, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, bn::fixed_point(40, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, bn::fixed_point(-60, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, bn::fixed_point(-45, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, bn::fixed_point(-30, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(old_woman, bn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, bn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, bn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 60, enemy_drop_type::NONE),

        // level 3 mid boss (12s)

        enemy_event(robot, bn::fixed_point(0, start_y - 16), robot_moves, robot_bullets, 0, enemy_drop_type::HERO_BOMB),

        // level 4 slow (22s)

        enemy_event(red_ninja, bn::fixed_point(-45, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(-60, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(-30, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog_mask, bn::fixed_point(-45, start_y), dog_mask_moves, dog_mask_bullets, 60, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(-60, start_y), lizard_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(red_ninja, bn::fixed_point(40, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(55, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(25, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog_mask, bn::fixed_point(40, start_y), dog_mask_flipped_moves, dog_mask_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(-60, start_y), lizard_moves, no_bullets, 75, enemy_drop_type::GEM),

        enemy_event(black_ninja, bn::fixed_point(-45, start_y), black_ninja_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(-60, start_y), black_ninja_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(-30, start_y), black_ninja_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(black_ninja, bn::fixed_point(40, start_y), black_ninja_flipped_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(55, start_y), black_ninja_flipped_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(25, start_y), black_ninja_flipped_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_snake, bn::fixed_point(-45, start_y), green_snake_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_snake, bn::fixed_point(40, start_y), red_snake_flipped_moves, red_snake_bullets, 60, enemy_drop_type::NONE),

        enemy_event(red_snake, bn::fixed_point(-30, start_y), red_snake_moves, red_snake_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_snake, bn::fixed_point(25, start_y), green_snake_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_snake, bn::fixed_point(-60, start_y), green_snake_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_snake, bn::fixed_point(55, start_y), red_snake_flipped_moves, red_snake_bullets, 60, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(-60, start_y), lizard_moves, no_bullets, 60, enemy_drop_type::GEM),

        // level 4 fast (28s)

        enemy_event(ogre, bn::fixed_point(0, start_y), ogre_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(ogre, bn::fixed_point(-30, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre, bn::fixed_point(25, start_y), ogre_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(ogre, bn::fixed_point(-60, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre, bn::fixed_point(55, start_y), ogre_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(samurai, bn::fixed_point(-45, start_y), samurai_moves, samurai_bullets, 1, enemy_drop_type::NONE),
        enemy_event(samurai, bn::fixed_point(40, start_y), samurai_flipped_moves, samurai_bullets, 45, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(-60, start_y), lizard_moves, no_bullets, 45, enemy_drop_type::GEM),

        enemy_event(red_ninja, bn::fixed_point(-45, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(-60, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(-30, start_y), red_ninja_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog_mask, bn::fixed_point(-45, start_y), dog_mask_moves, dog_mask_bullets, 45, enemy_drop_type::NONE),

        enemy_event(black_ninja, bn::fixed_point(55, start_y), black_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(25, start_y), black_ninja_flipped_moves, black_ninja_bullets, 45, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(40, start_y), black_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(black_ninja, bn::fixed_point(65, start_y), black_ninja_flipped_moves, black_ninja_bullets, 45, enemy_drop_type::NONE),

        enemy_event(red_ninja, bn::fixed_point(40, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(55, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_ninja, bn::fixed_point(25, start_y), red_ninja_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog_mask, bn::fixed_point(40, start_y), dog_mask_flipped_moves, dog_mask_flipped_bullets, 45, enemy_drop_type::NONE),

        enemy_event(green_snake, bn::fixed_point(-60, start_y), green_snake_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_snake, bn::fixed_point(-30, start_y), red_snake_moves, red_snake_bullets, 45, enemy_drop_type::NONE),
        enemy_event(green_snake, bn::fixed_point(-45, start_y), green_snake_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(red_snake, bn::fixed_point(-70, start_y), red_snake_moves, red_snake_bullets, 45, enemy_drop_type::NONE),

        enemy_event(lizard, bn::fixed_point(-60, start_y), lizard_moves, no_bullets, 180, enemy_drop_type::GEM),

        enemy_event(ogre_weak, bn::fixed_point(0, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-30, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(25, start_y), ogre_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-60, start_y), ogre_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(ogre_weak, bn::fixed_point(0 + 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-30 + 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(25 + 10, start_y), ogre_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(55 + 10, start_y), ogre_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(ogre_weak, bn::fixed_point(0 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-30 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-60 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(55 - 10, start_y), ogre_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(ogre_weak, bn::fixed_point(0, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(25, start_y), ogre_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-60, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(55, start_y), ogre_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(ogre_weak, bn::fixed_point(-30 + 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(25 + 10, start_y), ogre_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-60 + 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(55 + 10, start_y), ogre_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(ogre_weak, bn::fixed_point(0 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-30 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(25 - 10, start_y), ogre_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(-60 - 10, start_y), ogre_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(ogre_weak, bn::fixed_point(55 - 10, start_y), ogre_flipped_moves, no_bullets, 120, enemy_drop_type::NONE),

        enemy_event(samurai, bn::fixed_point(-45, start_y), samurai_moves, samurai_bullets, 1, enemy_drop_type::NONE),
        enemy_event(samurai, bn::fixed_point(40, start_y), samurai_flipped_moves, samurai_bullets, 45, enemy_drop_type::NONE),
    };


    // stage (start with level 3, exp 950):

    constexpr stage stage_3(
        bn::regular_bg_items::stage_2, bn::fixed_point(0, constants::background_speed),
        bn::regular_bg_items::clouds, bn::fixed_point(-1.0 / 16, constants::background_speed),
        bn::sprite_items::stage_2_intro, bn::sprite_items::stage_2_intro_alt, "STAGE 2", "NINJA VILLAGE",
        bn::music_items::soda7_xcopy_ohc, 0.4, enemy_events, boss::type::GIGABAT, false);
}

const stage& get()
{
    return stage_3;
}

}
