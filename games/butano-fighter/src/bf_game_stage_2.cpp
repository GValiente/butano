#include "bf_game_stage_2.h"

#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_items_clouds.h"
#include "btn_regular_bg_items_stage_2.h"
#include "btn_sprite_items_stage_2_intro.h"
#include "btn_sprite_items_stage_2_robot.h"
#include "btn_sprite_items_stage_2_intro_alt.h"
#include "btn_sprite_items_stage_2_monsters_1.h"
#include "btn_sprite_items_stage_2_monsters_2.h"
#include "btn_sprite_items_stage_2_characters_1.h"
#include "btn_sprite_items_stage_2_characters_2.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_2
{

namespace
{
    constexpr const int down_index = 0;
    constexpr const int up_index = 1;
    constexpr const int left_index = 2;
    constexpr const int right_index = 3;
    constexpr const int start_y = -(btn::display::height() + 16) / 2;

    constexpr const btn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr btn::fixed move_y(btn::fixed y)
    {
        return y + constants::background_speed;
    }

    [[nodiscard]] constexpr btn::array<enemy_data::graphics_indexes_group, 4> _create_graphics_indexes(
        int initial_index)
    {
        return btn::array<enemy_data::graphics_indexes_group, 4> {
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

    constexpr const enemy_data rat(
        btn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::dinosaur_5, btn::fixed_size(16, 16), _create_graphics_indexes(0), 2, 2);

    constexpr const enemy_move_event rat_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(btn::fixed_point(1,    move_y(0)),     70, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1.5)),  1,  up_index,       false),
    };

    constexpr const enemy_move_event rat_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(btn::fixed_point(-1,   move_y(0)),     70, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1.5)),  1,  up_index,       false),
    };


    // beaver:

    constexpr const enemy_data beaver(
        btn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::enemy_death_rpg_25, btn::fixed_size(16, 15), _create_graphics_indexes(16), 5, 5);

    constexpr const enemy_move_event beaver_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(btn::fixed_point(2,    move_y(0.5)),   10, right_index,    false),
        enemy_move_event(btn::fixed_point(2,    move_y(0.5)),   10, up_index,       false),
        enemy_move_event(btn::fixed_point(2,    move_y(0.5)),   10, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };

    constexpr const enemy_move_event beaver_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(btn::fixed_point(-2,   move_y(0.5)),   10, left_index,     false),
        enemy_move_event(btn::fixed_point(-2,   move_y(0.5)),   10, up_index,       false),
        enemy_move_event(btn::fixed_point(-2,   move_y(0.5)),   10, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };


    // fire:

    constexpr const enemy_data fire(
        btn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::HORIZONTAL_SCALE,
        btn::sound_items::enemy_death_rpg_12, btn::fixed_size(16, 14), _create_graphics_indexes(32), 7, 7);

    constexpr const enemy_move_event fire_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  down_index,     false),

        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     4,  down_index,     false),
    };

    constexpr const enemy_bullet_event fire_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, 1, 1),     44 + 20 + 20 + 20 + 4),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 1, 1),     1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, 0, 1),     1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1, -1, 1),    1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, -1, 1),    1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, -1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 0, 1),    1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, 1, 1),    1),
    };


    // eye:

    constexpr const enemy_data eye(
        btn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::dinosaur_5, btn::fixed_size(16, 14), _create_graphics_indexes(48), 6, 6);

    constexpr const enemy_move_event eye_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(btn::fixed_point(0.5,  move_y(0)),     70, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr const enemy_move_event eye_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(btn::fixed_point(-0.5, move_y(0)),     70, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr const enemy_bullet_event eye_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
    };

    constexpr const enemy_bullet_event eye_flipped_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5), 1.25),    30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,   move_y(0), 1.25),      30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1), 1),        30),
    };


    // green_ninja:

    constexpr const enemy_data green_ninja(
        btn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_4, btn::fixed_size(14, 14), _create_graphics_indexes(0), 2, 4);

    constexpr const enemy_move_event green_ninja_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  60,     down_index,     false),
        enemy_move_event(btn::fixed_point(0.5,  move_y(0)),     120,    right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  1,      down_index,     false),
    };

    constexpr const enemy_move_event green_ninja_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  60,     down_index,     false),
        enemy_move_event(btn::fixed_point(-0.5, move_y(0)),     120,    left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  1,      down_index,     false),
    };

    constexpr const enemy_bullet_event green_ninja_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 120),
    };


    // old_woman:

    constexpr const enemy_data old_woman(
        btn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::ROTATE,
        btn::sound_items::enemy_death_ninja_14, btn::fixed_size(14, 16), _create_graphics_indexes(16), 3, 5);

    constexpr const enemy_move_event old_woman_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     12, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.75)), 1,  up_index,       false),
    };

    constexpr const enemy_move_event old_woman_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     12, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.75)), 1,  up_index,       false),
    };

    constexpr const enemy_bullet_event old_woman_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG, 1, 40 + 30 + 30),
    };


    // bald:

    constexpr const enemy_data bald(
        btn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_6, btn::fixed_size(14, 15), _create_graphics_indexes(32), 12, 10);

    constexpr const enemy_move_event bald_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.3)),   60, down_index,     false),
        enemy_move_event(btn::fixed_point(0.6,  move_y(0)),     60, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.8)),  60, up_index,       false),
        enemy_move_event(btn::fixed_point(-0.6, move_y(0)),     60, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.3)),   1,  down_index,     false),
    };

    constexpr const enemy_move_event bald_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.3)),   60, down_index,     false),
        enemy_move_event(btn::fixed_point(-0.6, move_y(0)),     60, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.8)),  60, up_index,       false),
        enemy_move_event(btn::fixed_point(0.6,  move_y(0)),     60, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.3)),   1,  down_index,     false),
    };


    // dog:

    constexpr const enemy_data dog(
        btn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::ROTATE,
        btn::sound_items::monster_2, btn::fixed_size(12, 10), 48, 49, 2, 4);

    constexpr const enemy_move_event dog_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr const enemy_move_event dog_flipped_moves[] = {
        dog_moves[0].flipped(),
        dog_moves[1].flipped(),
    };


    // robot:

    constexpr const enemy_data robot(
        btn::sprite_items::stage_2_robot, enemy_data::death_anim_type::EXPLOSION,
        btn::sound_items::boss_shoot, btn::fixed_size(26, 47), 0, 1, 150, 200, 4);

    constexpr const enemy_move_event robot_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.25)), 220, false),
        enemy_move_event(btn::fixed_point(0.25, 0),          160, false),
        enemy_move_event(btn::fixed_point(-0.25, 0),         320, false),
        enemy_move_event(btn::fixed_point(0.25, 0),          160, false),
        enemy_move_event(btn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr const enemy_bullet_event robot_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),

        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 90),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
        enemy_bullet_event(enemy_bullet_type::ROBOT_DOUBLE, direction_vector(0, 1, 1), 12),
    };


    // black_ninja:

    constexpr const enemy_data black_ninja(
        btn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_5, btn::fixed_size(14, 14), _create_graphics_indexes(0), 3, 6);

    constexpr const enemy_move_event black_ninja_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.35)),  40, down_index,     false),
        enemy_move_event(btn::fixed_point(0.7,  move_y(0)),     80, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.35)),  1,  down_index,     false),
    };

    constexpr const enemy_bullet_event black_ninja_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 40),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 80),
    };


    // dog_mask:

    constexpr const enemy_data dog_mask(
        btn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::monster_2, btn::fixed_size(14, 14), _create_graphics_indexes(16), 6, 6);

    constexpr const enemy_move_event dog_mask_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  60, down_index,     false),
        enemy_move_event(btn::fixed_point(0.5,  move_y(0)),     70, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1)),    1,  up_index,       false),
    };

    constexpr const enemy_bullet_event dog_mask_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(0.5),    1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(0.5),    1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(0.5),    1),  20),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0),      1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(0.5),    1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,    move_y(-0.5),   1),  30),

        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,    move_y(-1),     1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.5,  move_y(-1),     1),  20),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.5, move_y(-1),     1),  20),
    };


    // samurai:

    constexpr const enemy_data samurai(
        btn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_2, btn::fixed_size(16, 15), _create_graphics_indexes(32), 7, 8);

    constexpr const enemy_move_event samurai_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.25)),  40, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     30, down_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  up_index,       false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     6,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0)),     1,  down_index,     false),
    };

    constexpr const enemy_bullet_event samurai_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 40 + 30 + 30),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 12),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 12),
    };


    // red_ninja:

    constexpr const enemy_data red_ninja(
        btn::sprite_items::stage_2_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_4, btn::fixed_size(12, 15), _create_graphics_indexes(48), 4, 6);

    constexpr const enemy_move_event red_ninja_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(btn::fixed_point(1.5,  move_y(0.5)),   10, right_index,    false),
        enemy_move_event(btn::fixed_point(1.5,  move_y(0.5)),   10, up_index,       false),
        enemy_move_event(btn::fixed_point(1.5,  move_y(0.5)),   10, left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),

        enemy_move_event(btn::fixed_point(-1.5, move_y(0.5)),   10, left_index,     false),
        enemy_move_event(btn::fixed_point(-1.5, move_y(0.5)),   10, up_index,       false),
        enemy_move_event(btn::fixed_point(-1.5, move_y(0.5)),   10, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   1,  down_index,     false),
    };


    // green_snake:

    constexpr const enemy_data green_snake(
        btn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::dinosaur_4, btn::fixed_size(14, 15), _create_graphics_indexes(0), 5, 7);

    constexpr const enemy_move_event green_snake_moves[] = {
        enemy_move_event(btn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(0.75,     move_y(0.25)),  30,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,        move_y(0.25)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(-0.75,    move_y(0.25)),  30,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,        move_y(0.25)),  1,   down_index,     false),
    };


    // lizard:

    constexpr const enemy_data lizard(
        btn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::ROTATE,
        btn::sound_items::dinosaur_5, btn::fixed_size(12, 15), _create_graphics_indexes(16), 3, 5);

    constexpr const enemy_move_event lizard_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.15)), 35, down_index,  false),
        enemy_move_event(btn::fixed_point(0.75, move_y(0.0)),  55, right_index, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1.1)), 1,  up_index,    false),
    };


    // red_snake:

    constexpr const enemy_data red_snake(
        btn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::dinosaur_4, btn::fixed_size(14, 15), _create_graphics_indexes(32), 4, 7);

    constexpr const enemy_move_event red_snake_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(0.5,  move_y(0.2)),  30,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.2)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(-0.5, move_y(0.2)),  30,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.2)),  1,   down_index,     false),
    };

    constexpr const enemy_bullet_event red_snake_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // ogre:

    constexpr const enemy_data ogre(
        btn::sprite_items::stage_2_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_3, btn::fixed_size(16, 16), _create_graphics_indexes(48), 14, 12);

    constexpr const enemy_move_event ogre_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.1)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(0.2,  move_y(0.1)),  30,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.1)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(-0.2, move_y(0.1)),  30,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.1)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(0.2,  move_y(0.1)),  30,  right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.1)),  30,  down_index,     false),
        enemy_move_event(btn::fixed_point(-0.2, move_y(0.1)),  30,  left_index,     false),
        enemy_move_event(btn::fixed_point(0,    move_y(0.1)),  1,   down_index,     false),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 3 slow (25s)

        /*enemy_event(rat, btn::fixed_point(-45, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(rat, btn::fixed_point(-30, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, btn::fixed_point(40, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(beaver, btn::fixed_point(55, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(beaver, btn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, btn::fixed_point(-45, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(-60, start_y), green_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(-30, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, btn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(55, start_y), green_ninja_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(old_woman, btn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, btn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, btn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, btn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(eye, btn::fixed_point(-30, start_y), eye_moves, eye_bullets, 60, enemy_drop_type::NONE),

        enemy_event(beaver, btn::fixed_point(-30, start_y), beaver_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(eye, btn::fixed_point(25, start_y), eye_flipped_moves, eye_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, btn::fixed_point(-45, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(-30, start_y), green_ninja_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_ninja, btn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(old_woman, btn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, btn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, btn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 75, enemy_drop_type::NONE),

        // level 3 fast (25s)

        enemy_event(bald, btn::fixed_point(5, start_y), bald_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, btn::fixed_point(55, start_y), bald_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, btn::fixed_point(-60, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, btn::fixed_point(-45, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, btn::fixed_point(55, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, btn::fixed_point(25, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, btn::fixed_point(-45, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, btn::fixed_point(-30, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(rat, btn::fixed_point(-45, start_y), rat_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(rat, btn::fixed_point(-30, start_y), rat_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(green_ninja, btn::fixed_point(40, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(55, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),
        enemy_event(green_ninja, btn::fixed_point(25, start_y), green_ninja_flipped_moves, green_ninja_bullets, 45, enemy_drop_type::NONE),

        enemy_event(beaver, btn::fixed_point(25, start_y), beaver_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(eye, btn::fixed_point(-30, start_y), eye_moves, eye_bullets, 45, enemy_drop_type::NONE),

        enemy_event(beaver, btn::fixed_point(-30, start_y), beaver_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(eye, btn::fixed_point(55, start_y), eye_flipped_moves, eye_flipped_bullets, 45, enemy_drop_type::NONE),

        enemy_event(bald, btn::fixed_point(-60, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(bald, btn::fixed_point(-45, start_y), bald_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, btn::fixed_point(55, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, btn::fixed_point(40, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(fire, btn::fixed_point(-60, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, btn::fixed_point(-45, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),
        enemy_event(fire, btn::fixed_point(-30, start_y), fire_moves, fire_bullets, 45, enemy_drop_type::NONE),

        enemy_event(old_woman, btn::fixed_point(-45, start_y), old_woman_moves, old_woman_bullets, 45, enemy_drop_type::NONE),
        enemy_event(dog, btn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(old_woman, btn::fixed_point(40, start_y), old_woman_flipped_moves, old_woman_bullets, 60, enemy_drop_type::NONE),

        // level 3 mid boss (12s)

        enemy_event(robot, btn::fixed_point(0, start_y - 16), robot_moves, robot_bullets, 0, enemy_drop_type::HERO_BOMB),

        // level 4 slow

        enemy_event(black_ninja, btn::fixed_point(-45, start_y), black_ninja_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, btn::fixed_point(-60, start_y), black_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_ninja, btn::fixed_point(-30, start_y), black_ninja_moves, black_ninja_bullets, 60, enemy_drop_type::NONE),

        enemy_event(dog_mask, btn::fixed_point(-45, start_y), dog_mask_moves, dog_mask_bullets, 60, enemy_drop_type::NONE),
        enemy_event(dog_mask, btn::fixed_point(-60, start_y), dog_mask_moves, dog_mask_bullets, 60, enemy_drop_type::NONE),
        enemy_event(dog_mask, btn::fixed_point(-30, start_y), dog_mask_moves, dog_mask_bullets, 60, enemy_drop_type::NONE),

        enemy_event(samurai, btn::fixed_point(-45, start_y), samurai_moves, samurai_bullets, 60, enemy_drop_type::NONE),
        enemy_event(samurai, btn::fixed_point(-60, start_y), samurai_moves, samurai_bullets, 60, enemy_drop_type::NONE),
        enemy_event(samurai, btn::fixed_point(-30, start_y), samurai_moves, samurai_bullets, 60, enemy_drop_type::NONE),

        enemy_event(red_ninja, btn::fixed_point(-45, start_y), red_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_ninja, btn::fixed_point(-60, start_y), red_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_ninja, btn::fixed_point(-30, start_y), red_ninja_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(lizard, btn::fixed_point(-60, start_y), lizard_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(red_snake, btn::fixed_point(-45, start_y), red_snake_moves, red_snake_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_snake, btn::fixed_point(-60, start_y), red_snake_moves, red_snake_bullets, 60, enemy_drop_type::NONE),
        enemy_event(red_snake, btn::fixed_point(-30, start_y), red_snake_moves, red_snake_bullets, 60, enemy_drop_type::NONE),

        enemy_event(green_snake, btn::fixed_point(-45, start_y), green_snake_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_snake, btn::fixed_point(-60, start_y), green_snake_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(green_snake, btn::fixed_point(-30, start_y), green_snake_moves, no_bullets, 60, enemy_drop_type::NONE),*/

        enemy_event(ogre, btn::fixed_point(-45, start_y), ogre_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(ogre, btn::fixed_point(-60, start_y), ogre_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(ogre, btn::fixed_point(-30, start_y), ogre_moves, no_bullets, 60, enemy_drop_type::NONE),
    };


    // stage (start with level 3, exp 1100):

    constexpr const stage stage_2(
        btn::regular_bg_items::stage_2, btn::regular_bg_items::clouds, btn::sprite_items::stage_2_intro,
        btn::sprite_items::stage_2_intro_alt, "STAGE 2", "NINJA VILLAGE", btn::music_items::soda7_xcopy_ohc,
        enemy_events, boss::type::TANK);
}

const stage& get()
{
    return stage_2;
}

}
