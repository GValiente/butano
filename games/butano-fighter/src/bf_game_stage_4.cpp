/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_stage_4.h"

#include "bn_display.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_stars.h"
#include "bn_regular_bg_items_stage_4.h"
#include "bn_sprite_items_stage_4_intro.h"
#include "bn_sprite_items_stage_4_brain.h"
#include "bn_sprite_items_stage_4_octopus.h"
#include "bn_sprite_items_stage_4_intro_alt.h"
#include "bn_sprite_items_stage_4_enemies_1.h"
#include "bn_sprite_items_stage_4_enemies_2.h"
#include "bn_sprite_items_stage_4_enemies_4.h"
#include "bn_sprite_items_stage_4_enemies_3_1.h"
#include "bn_sprite_items_stage_4_enemies_3_2.h"
#include "bn_sprite_items_stage_4_meteors_big.h"
#include "bn_sprite_items_stage_4_meteors_small.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_4
{

namespace
{
    constexpr int start_y_small = -(bn::display::height() + 32) / 2;
    constexpr int start_y_big = -(bn::display::height() + 64) / 2;

    constexpr bn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr bn::fixed move_y(bn::fixed y)
    {
        return y + constants::background_speed;
    }

    [[nodiscard]] constexpr bn::array<enemy_data::graphics_indexes_group, 4> _create_graphics_indexes(int index)
    {
        return bn::array<enemy_data::graphics_indexes_group, 4> {
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) }
        };
    }


    // small_octopus:

    constexpr enemy_data small_octopus(
        bn::sprite_items::stage_4_octopus, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_14, bn::fixed_size(26, 24), _create_graphics_indexes(0), 12, 22);

    constexpr enemy_move_event small_octopus_moves[] = {
        enemy_move_event(bn::fixed_point(0.75, move_y(0.25)),  50, false),
        enemy_move_event(bn::fixed_point(0.75, 0),             40, false),
        enemy_move_event(bn::fixed_point(0.75, move_y(-1)),    1,  false),
    };

    constexpr enemy_move_event small_octopus_flipped_moves[] = {
        small_octopus_moves[0].flipped(),
        small_octopus_moves[1].flipped(),
        small_octopus_moves[2].flipped(),
    };


    // small_brown_meteor:

    constexpr enemy_data small_brown_meteor(
        bn::sprite_items::stage_4_meteors_small, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::space_shooter_1, bn::fixed_size(17, 15), _create_graphics_indexes(2), 30, 20);

    constexpr enemy_move_event small_brown_meteor_moves[] = {
        enemy_move_event(bn::fixed_point(0.2, move_y(0.15)),  1,  false),
    };

    constexpr enemy_move_event small_brown_meteor_flipped_moves[] = {
        small_brown_meteor_moves[0].flipped(),
    };


    // medium_brown_meteor:

    constexpr enemy_data medium_brown_meteor(
        bn::sprite_items::stage_4_meteors_small, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::space_shooter_3, bn::fixed_size(26, 26), _create_graphics_indexes(1), 40, 34);

    constexpr enemy_move_event medium_brown_meteor_moves[] = {
        enemy_move_event(bn::fixed_point(0.15, move_y(0.1)),  1,  false),
    };

    constexpr enemy_move_event medium_brown_meteor_flipped_moves[] = {
        medium_brown_meteor_moves[0].flipped(),
    };


    // big_brown_meteor:

    constexpr enemy_data big_brown_meteor(
        bn::sprite_items::stage_4_meteors_big, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::space_shooter_5, bn::fixed_size(30, 36), _create_graphics_indexes(1), 50, 40);

    constexpr enemy_move_event big_brown_meteor_moves[] = {
        enemy_move_event(bn::fixed_point(0.1, move_y(0.05)),  1,  false),
    };

    constexpr enemy_move_event big_brown_meteor_flipped_moves[] = {
        big_brown_meteor_moves[0].flipped(),
    };


    // small_red_meteor:

    constexpr enemy_data small_red_meteor(
        bn::sprite_items::stage_4_meteors_small, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::space_shooter_3, bn::fixed_size(22, 20), _create_graphics_indexes(0), 36, 32);

    constexpr enemy_move_event small_red_meteor_moves[] = {
        enemy_move_event(bn::fixed_point(0.25, move_y(0.15)),  1,  false),
    };

    constexpr enemy_move_event small_red_meteor_flipped_moves[] = {
        small_red_meteor_moves[0].flipped(),
    };


    // big_red_meteor:

    constexpr enemy_data big_red_meteor(
        bn::sprite_items::stage_4_meteors_big, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::space_shooter_5, bn::fixed_size(42, 38), _create_graphics_indexes(0), 56, 48);

    constexpr enemy_move_event big_red_meteor_moves[] = {
        enemy_move_event(bn::fixed_point(0.15, move_y(0.05)),  1,  false),
    };

    constexpr enemy_move_event big_red_meteor_flipped_moves[] = {
        big_red_meteor_moves[0].flipped(),
    };


    // white_astronaut:

    constexpr enemy_data white_astronaut(
        bn::sprite_items::stage_4_enemies_3_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_4, bn::fixed_size(22, 24), _create_graphics_indexes(0), 18, 28);

    constexpr enemy_move_event white_astronaut_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr enemy_bullet_event white_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  32),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  32),
    };


    // blue_astronaut:

    constexpr enemy_data blue_astronaut(
        bn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_3, bn::fixed_size(22, 24), _create_graphics_indexes(0), 26, 36);

    constexpr enemy_move_event blue_astronaut_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr enemy_bullet_event blue_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  32),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  32),
    };


    // red_astronaut:

    constexpr enemy_data red_astronaut(
        bn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_5, bn::fixed_size(22, 24), _create_graphics_indexes(1), 20, 30);

    constexpr enemy_move_event red_astronaut_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr enemy_bullet_event red_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),      45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(-0.5, 1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0.5, 1, 1),    1),
    };


    // yellow_astronaut:

    constexpr enemy_data yellow_astronaut(
        bn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        bn::sound_items::scream_6, bn::fixed_size(22, 24), _create_graphics_indexes(2), 16, 26);

    constexpr enemy_move_event yellow_astronaut_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr enemy_bullet_event yellow_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  1,  60),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  1,  60),
    };


    // yellow_spaceship:

    constexpr enemy_data yellow_spaceship(
        bn::sprite_items::stage_4_enemies_3_1, enemy_data::death_anim_type::MINI_EXPLOSION,
        bn::sound_items::explosion_3, bn::fixed_size(46, 44), _create_graphics_indexes(0), 36, 46);

    constexpr enemy_move_event yellow_spaceship_moves[] = {
        enemy_move_event(bn::fixed_point(0.1, move_y(0.1)),  1,  false),
    };

    constexpr enemy_move_event yellow_spaceship_flipped_moves[] = {
        yellow_spaceship_moves[0].flipped(),
    };

    constexpr enemy_bullet_event yellow_spaceship_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::YELLOW_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  45),
        enemy_bullet_event(enemy_bullet_type::YELLOW_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  45),
        enemy_bullet_event(enemy_bullet_type::YELLOW_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  45),
    };


    // green_spaceship:

    constexpr enemy_data green_spaceship(
        bn::sprite_items::stage_4_enemies_2, enemy_data::death_anim_type::MINI_EXPLOSION,
        bn::sound_items::explosion_2, bn::fixed_size(62, 46), _create_graphics_indexes(0), 40, 50);

    constexpr bn::fixed_point green_spaceship_start_position(-(bn::display::width() + 50) / 2, start_y_big);

    constexpr bn::fixed_point green_spaceship_flipped_start_position(
        -green_spaceship_start_position.x(), green_spaceship_start_position.y());

    constexpr enemy_move_event green_spaceship_moves[] = {
        enemy_move_event(bn::fixed_point(1, move_y(0)),  1,  false),
    };

    constexpr enemy_move_event green_spaceship_flipped_moves[] = {
        green_spaceship_moves[0].flipped(),
    };

    constexpr enemy_bullet_event green_spaceship_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::GREEN_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  60),
        enemy_bullet_event(enemy_bullet_type::GREEN_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  60),
        enemy_bullet_event(enemy_bullet_type::GREEN_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  60),
        enemy_bullet_event(enemy_bullet_type::GREEN_SPACESHIP_SMALL,   direction_vector(0, 1, 0.9),  60),
    };


    // brain:

    constexpr enemy_data brain(
        bn::sprite_items::stage_4_brain, enemy_data::death_anim_type::EXPLOSION,
        bn::sound_items::boss_shoot, bn::fixed_size(34, 54), _create_graphics_indexes(0), 500, 300);

    constexpr enemy_move_event brain_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 250, false),
        enemy_move_event(bn::fixed_point(-0.25, 0),         320, false),
        enemy_move_event(bn::fixed_point(0.25, 0),          320, false),
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr enemy_bullet_event brain_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BRAIN_BIG,    1,  100),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),

        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.3, 1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.3, 1, 1),    1),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      20),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.3, 1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.3, 1, 1),    1),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      20),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.3, 1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.3, 1, 1),    1),

        enemy_bullet_event(enemy_bullet_type::BRAIN_BIG,    1,  80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),

        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   0.9,    80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   0.9,    16),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   0.9,    16),

        enemy_bullet_event(enemy_bullet_type::BRAIN_BIG,    1,  80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),

        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.1, 1, 1),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.1, 1, 1),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.1, 1, 1),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.1, 1, 1),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.1, 1, 1),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.1, 1, 1),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.1, 1, 1),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.1, 1, 1),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0, 1, 1),      4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(-0.1, 1, 1),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  direction_vector(0.1, 1, 1),    4),

        enemy_bullet_event(enemy_bullet_type::BRAIN_BIG,    1,  80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),
        enemy_bullet_event(enemy_bullet_type::BRAIN_SMALL,  1,  6),

        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(1, 1, 0.8),    80),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(-1, 1, 0.8),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(0, 1, 0.8),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(-1, -1, 0.8),  4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(-1, 0, 0.8),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(1, -1, 0.8),   4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(1, 0, 0.8),    4),
        enemy_bullet_event(enemy_bullet_type::BRAIN_HUGE,   direction_vector(0, -1, 0.8),   4),
    };


    // pink_spaceship:

    constexpr enemy_data pink_spaceship(
        bn::sprite_items::stage_4_enemies_1, enemy_data::death_anim_type::MINI_EXPLOSION,
        bn::sound_items::explosion_1, bn::fixed_size(34, 32), _create_graphics_indexes(0), 36, 46);

    constexpr enemy_move_event pink_spaceship_moves[] = {
        enemy_move_event(bn::fixed_point(0.1, move_y(1)),  1,  false),
    };

    constexpr enemy_move_event pink_spaceship_flipped_moves[] = {
        pink_spaceship_moves[0].flipped(),
    };


    // red_spaceship:

    constexpr enemy_data red_spaceship(
        bn::sprite_items::stage_4_enemies_1, enemy_data::death_anim_type::MINI_EXPLOSION,
        bn::sound_items::explosion_3, bn::fixed_size(42, 34), _create_graphics_indexes(1), 38, 50);

    constexpr enemy_move_event red_spaceship_moves[] = {
        enemy_move_event(bn::fixed_point(0.1, move_y(0.3)),  1,  false),
    };

    constexpr enemy_move_event red_spaceship_flipped_moves[] = {
        red_spaceship_moves[0].flipped(),
    };

    constexpr enemy_bullet_event red_spaceship_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::RED_SPACESHIP_BIG,   direction_vector(0, 1, 1),  45),
        enemy_bullet_event(enemy_bullet_type::RED_SPACESHIP_BIG,   direction_vector(0, 1, 1),  75),
        enemy_bullet_event(enemy_bullet_type::RED_SPACESHIP_BIG,   direction_vector(0, 1, 1),  75),
    };


    // blue_spaceship:

    constexpr enemy_data blue_spaceship(
        bn::sprite_items::stage_4_enemies_1, enemy_data::death_anim_type::MINI_EXPLOSION,
        bn::sound_items::explosion_3, bn::fixed_size(50, 62), _create_graphics_indexes(2), 30, 60);

    constexpr bn::fixed_point blue_spaceship_start_position(-(constants::play_width + 32), -start_y_big);

    constexpr bn::fixed_point blue_spaceship_flipped_start_position(
        -blue_spaceship_start_position.x(), blue_spaceship_start_position.y());

    constexpr enemy_move_event blue_spaceship_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-1.5)),  1,  false),
    };

    constexpr enemy_move_event blue_spaceship_flipped_moves[] = {
        blue_spaceship_moves[0].flipped(),
    };

    constexpr enemy_bullet_event blue_spaceship_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BLUE_SPACESHIP_SMALL, direction_vector(1, 0, 1),  120),
        enemy_bullet_event(enemy_bullet_type::BLUE_SPACESHIP_SMALL, direction_vector(1, 0, 1),  60),
    };

    constexpr enemy_bullet_event blue_spaceship_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BLUE_SPACESHIP_FLIPPED_SMALL, direction_vector(-1, 0, 1), 120),
        enemy_bullet_event(enemy_bullet_type::BLUE_SPACESHIP_FLIPPED_SMALL, direction_vector(-1, 0, 1), 60),
    };


    // big_octopus:

    constexpr enemy_data big_octopus(
        bn::sprite_items::stage_4_enemies_2, enemy_data::death_anim_type::ROTATE,
        bn::sound_items::enemy_death_ninja_11, bn::fixed_size(54, 50), _create_graphics_indexes(1), 42, 56);

    constexpr enemy_move_event big_octopus_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.3)),  80, false),

        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  true),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  false),
        enemy_move_event(bn::fixed_point(0, move_y(0.1)),  4,  true),

        enemy_move_event(bn::fixed_point(0, move_y(0.3)),  1,  false),
    };

    constexpr enemy_bullet_event big_octopus_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG,      direction_vector(1, 1, 0.9),   80),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-1, 1, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::BIG,      direction_vector(0, 1, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(-1, -1, 0.9), 4),
        enemy_bullet_event(enemy_bullet_type::BIG,      direction_vector(-1, 0, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(1, -1, 0.9),  4),
        enemy_bullet_event(enemy_bullet_type::BIG,      direction_vector(1, 0, 0.9),   4),
        enemy_bullet_event(enemy_bullet_type::SMALL,    direction_vector(0, -1, 0.9),  4),
    };


    // enemy events:

    constexpr enemy_event enemy_events[] = {

        // level 7 (51s)

        enemy_event(small_brown_meteor, bn::fixed_point(-55, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-25, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-40, start_y_small), small_brown_meteor_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(25, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_small), medium_brown_meteor_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_red_meteor, bn::fixed_point(-50, start_y_small), small_red_meteor_moves, no_bullets, 40, enemy_drop_type::NONE),
        enemy_event(small_red_meteor, bn::fixed_point(-30, start_y_small), small_red_meteor_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(25, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(40, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 70, enemy_drop_type::NONE),

        enemy_event(white_astronaut, bn::fixed_point(-55, start_y_small), white_astronaut_moves, white_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(-25, start_y_small), white_astronaut_moves, white_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(-40, start_y_small), white_astronaut_moves, white_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blue_astronaut, bn::fixed_point(55, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(blue_astronaut, bn::fixed_point(25, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(40, start_y_small), white_astronaut_moves, white_astronaut_bullets, 40, enemy_drop_type::NONE),

        enemy_event(small_octopus, bn::fixed_point(-55, start_y_small), small_octopus_moves, no_bullets, 70, enemy_drop_type::GEM),

        enemy_event(small_brown_meteor, bn::fixed_point(-55, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-25, start_y_small), small_brown_meteor_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_red_meteor, bn::fixed_point(55, start_y_small), small_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_red_meteor, bn::fixed_point(25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(red_astronaut, bn::fixed_point(-55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(0, start_y_small), red_astronaut_moves, red_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(-55, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_spaceship, green_spaceship_start_position, green_spaceship_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_start_position, green_spaceship_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_start_position, green_spaceship_moves, green_spaceship_bullets, 120, enemy_drop_type::NONE),

        enemy_event(small_octopus, bn::fixed_point(-55, start_y_small), small_octopus_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(white_astronaut, bn::fixed_point(-55, start_y_small), white_astronaut_moves, white_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(-25, start_y_small), white_astronaut_moves, white_astronaut_bullets, 40, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(-40, start_y_small), white_astronaut_moves, white_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(25, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_small), medium_brown_meteor_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(-55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, bn::fixed_point(55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 70, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(-55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, bn::fixed_point(55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 70, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(-55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, bn::fixed_point(55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 70, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(30, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(0, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(15, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(yellow_spaceship, bn::fixed_point(-40, start_y_big), yellow_spaceship_moves, yellow_spaceship_bullets, 60, enemy_drop_type::NONE),
        enemy_event(yellow_spaceship, bn::fixed_point(40, start_y_big), yellow_spaceship_flipped_moves, yellow_spaceship_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_octopus, bn::fixed_point(-55, start_y_small), small_octopus_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(small_red_meteor, bn::fixed_point(-55, start_y_small), small_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_red_meteor, bn::fixed_point(-25, start_y_big), big_red_meteor_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(red_astronaut, bn::fixed_point(-55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(0, start_y_small), red_astronaut_moves, red_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 130, enemy_drop_type::NONE),

        enemy_event(yellow_spaceship, bn::fixed_point(40, start_y_big), yellow_spaceship_flipped_moves, yellow_spaceship_bullets, 60, enemy_drop_type::NONE),
        enemy_event(yellow_spaceship, bn::fixed_point(-40, start_y_big), yellow_spaceship_moves, yellow_spaceship_bullets, 80, enemy_drop_type::NONE),

        enemy_event(small_octopus, bn::fixed_point(55, start_y_small), small_octopus_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),

        // mid boss (17s)

        enemy_event(brain, bn::fixed_point(40, start_y_big), brain_moves, brain_bullets, 0, enemy_drop_type::HERO_BOMB),

        // level 8 (54s)

        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(25, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_small), medium_brown_meteor_flipped_moves, no_bullets, 80, enemy_drop_type::NONE),

        enemy_event(pink_spaceship, bn::fixed_point(-55, start_y_big), pink_spaceship_moves, no_bullets, 40, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(-25, start_y_big), pink_spaceship_moves, no_bullets, 40, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(-40, start_y_big), pink_spaceship_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pink_spaceship, bn::fixed_point(55, start_y_big), pink_spaceship_flipped_moves, no_bullets, 40, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(25, start_y_big), pink_spaceship_flipped_moves, no_bullets, 40, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(40, start_y_big), pink_spaceship_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blue_astronaut, bn::fixed_point(55, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blue_astronaut, bn::fixed_point(-55, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blue_astronaut, bn::fixed_point(0, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(white_astronaut, bn::fixed_point(30, start_y_small), white_astronaut_moves, white_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(-30, start_y_small), white_astronaut_moves, white_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blue_astronaut, bn::fixed_point(55, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blue_astronaut, bn::fixed_point(-55, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blue_astronaut, bn::fixed_point(0, start_y_small), blue_astronaut_moves, blue_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_start_position, blue_spaceship_moves, blue_spaceship_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(30, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(0, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(15, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 80, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(-55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, bn::fixed_point(55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(-30, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, bn::fixed_point(30, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(yellow_astronaut, bn::fixed_point(0, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(small_octopus, bn::fixed_point(55, start_y_small), small_octopus_flipped_moves, no_bullets, 80, enemy_drop_type::GEM),

        enemy_event(big_octopus, bn::fixed_point(55, start_y_big), big_octopus_moves, big_octopus_bullets, 40, enemy_drop_type::GEM),
        enemy_event(big_octopus, bn::fixed_point(-10, start_y_big), big_octopus_moves, big_octopus_bullets, 60, enemy_drop_type::GEM),

        enemy_event(blue_spaceship, blue_spaceship_flipped_start_position, blue_spaceship_flipped_moves, blue_spaceship_flipped_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_red_meteor, bn::fixed_point(-55, start_y_small), small_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_red_meteor, bn::fixed_point(-25, start_y_big), big_red_meteor_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(red_astronaut, bn::fixed_point(-55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(55, start_y_small), red_astronaut_moves, red_astronaut_bullets, 80, enemy_drop_type::NONE),

        enemy_event(red_spaceship, bn::fixed_point(-40, start_y_small), red_spaceship_moves, red_spaceship_bullets, 1, enemy_drop_type::NONE),
        enemy_event(red_spaceship, bn::fixed_point(40, start_y_small), red_spaceship_flipped_moves, red_spaceship_bullets, 100, enemy_drop_type::NONE),

        enemy_event(pink_spaceship, bn::fixed_point(-55, start_y_big), pink_spaceship_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(55, start_y_big), pink_spaceship_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pink_spaceship, bn::fixed_point(-55, start_y_big), pink_spaceship_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(55, start_y_big), pink_spaceship_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(pink_spaceship, bn::fixed_point(15, start_y_big), pink_spaceship_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(big_octopus, bn::fixed_point(-55, start_y_big), big_octopus_moves, big_octopus_bullets, 1, enemy_drop_type::GEM),
        enemy_event(big_octopus, bn::fixed_point(55, start_y_big), big_octopus_moves, big_octopus_bullets, 70, enemy_drop_type::GEM),

        enemy_event(small_octopus, bn::fixed_point(55, start_y_small), small_octopus_flipped_moves, no_bullets, 80, enemy_drop_type::GEM),

        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 65, enemy_drop_type::NONE),
        enemy_event(green_spaceship, green_spaceship_flipped_start_position, green_spaceship_flipped_moves, green_spaceship_bullets, 90, enemy_drop_type::NONE),

        enemy_event(yellow_spaceship, bn::fixed_point(40, start_y_big), yellow_spaceship_flipped_moves, yellow_spaceship_bullets, 80, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_start_position, blue_spaceship_moves, blue_spaceship_bullets, 1, enemy_drop_type::NONE),

        enemy_event(yellow_spaceship, bn::fixed_point(40, start_y_big), yellow_spaceship_flipped_moves, yellow_spaceship_bullets, 1, enemy_drop_type::NONE),
        enemy_event(yellow_spaceship, bn::fixed_point(-40, start_y_big), yellow_spaceship_moves, yellow_spaceship_bullets, 1, enemy_drop_type::NONE),
        enemy_event(red_astronaut, bn::fixed_point(0, start_y_small), red_astronaut_moves, red_astronaut_bullets, 90, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_flipped_start_position, blue_spaceship_flipped_moves, blue_spaceship_flipped_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(55, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(25, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_small), medium_brown_meteor_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(red_spaceship, bn::fixed_point(-45, start_y_big), red_spaceship_moves, red_spaceship_bullets, 50, enemy_drop_type::NONE),
        enemy_event(red_spaceship, bn::fixed_point(-15, start_y_big), red_spaceship_moves, red_spaceship_bullets, 50, enemy_drop_type::NONE),
        enemy_event(red_spaceship, bn::fixed_point(-30, start_y_big), red_spaceship_moves, red_spaceship_bullets, 60, enemy_drop_type::NONE),

        enemy_event(white_astronaut, bn::fixed_point(55, start_y_small), white_astronaut_moves, white_astronaut_bullets, 1, enemy_drop_type::NONE),
        enemy_event(white_astronaut, bn::fixed_point(25, start_y_small), white_astronaut_moves, white_astronaut_bullets, 60, enemy_drop_type::NONE),

        enemy_event(big_octopus, bn::fixed_point(-55, start_y_big), big_octopus_moves, big_octopus_bullets, 1, enemy_drop_type::GEM),
        enemy_event(big_octopus, bn::fixed_point(55, start_y_big), big_octopus_moves, big_octopus_bullets, 70, enemy_drop_type::GEM),

        enemy_event(small_octopus, bn::fixed_point(-55, start_y_small), small_octopus_moves, no_bullets, 80, enemy_drop_type::GEM),

        enemy_event(blue_spaceship, blue_spaceship_start_position, blue_spaceship_moves, blue_spaceship_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(-55, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-25, start_y_small), small_brown_meteor_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(-40, start_y_small), medium_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_flipped_start_position, blue_spaceship_flipped_moves, blue_spaceship_flipped_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_red_meteor, bn::fixed_point(55, start_y_small), small_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_red_meteor, bn::fixed_point(25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(small_red_meteor, bn::fixed_point(-50, start_y_small), small_red_meteor_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(small_red_meteor, bn::fixed_point(-10, start_y_small), small_red_meteor_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(small_red_meteor, bn::fixed_point(-30, start_y_small), small_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_start_position, blue_spaceship_moves, blue_spaceship_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(30, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(0, start_y_small), small_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(15, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(blue_spaceship, blue_spaceship_flipped_start_position, blue_spaceship_flipped_moves, blue_spaceship_flipped_bullets, 1, enemy_drop_type::NONE),

        enemy_event(small_brown_meteor, bn::fixed_point(-55, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-25, start_y_small), small_brown_meteor_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(small_brown_meteor, bn::fixed_point(-40, start_y_small), small_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(-55, start_y_big), big_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(55, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(-40, start_y_big), big_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(-25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_big), big_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(55, start_y_big), big_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(-55, start_y_big), big_brown_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(-25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(40, start_y_big), big_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(big_red_meteor, bn::fixed_point(0, start_y_big), big_red_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(medium_brown_meteor, bn::fixed_point(25, start_y_big), big_red_meteor_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(big_brown_meteor, bn::fixed_point(-40, start_y_big), big_brown_meteor_moves, no_bullets, 10, enemy_drop_type::NONE),
    };


    // stage (start with level 7, exp 5600):

    constexpr stage stage_4(
        bn::regular_bg_items::stage_4, bn::fixed_point(0, constants::background_speed / 8),
        bn::regular_bg_items::stars, bn::fixed_point(0, constants::background_speed),
        bn::sprite_items::stage_4_intro, bn::sprite_items::stage_4_intro_alt, "STAGE 4", "FAR FAR AWAY",
        bn::music_items::galgox_snowheart_melody, 0.8, enemy_events, boss::type::BUTANO, true);
}

const stage& get()
{
    return stage_4;
}

}
