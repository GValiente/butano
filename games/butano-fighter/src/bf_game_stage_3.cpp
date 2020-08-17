#include "bf_game_stage_3.h"

#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_items_stage_3.h"
#include "btn_sprite_items_stage_3_intro.h"
#include "btn_sprite_items_stage_3_blonde.h"
#include "btn_sprite_items_stage_3_intro_alt.h"
#include "btn_regular_bg_items_mountain_clouds.h"
#include "btn_sprite_items_stage_3_characters_1.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_3
{

namespace
{
    constexpr const int start_y = -(btn::display::height() + 64) / 2;

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

    constexpr const enemy_data blonde(
        btn::sprite_items::stage_3_blonde, enemy_data::death_anim_type::ROTATE,
        btn::sound_items::enemy_death_ninja_14, btn::fixed_size(46, 62), _create_graphics_indexes(0), 6, 6);

    constexpr const enemy_move_event blonde_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr const enemy_move_event blonde_flipped_moves[] = {
        blonde_moves[0].flipped(),
        blonde_moves[1].flipped(),
    };


    // white_caveman:

    constexpr const btn::array<enemy_data::graphics_indexes_group, 4> white_caveman_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 },
        enemy_data::graphics_indexes_group{ 5, 3, 4, 5 },
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 },
        enemy_data::graphics_indexes_group{ 0, 1, 0, 2 }
    };

    constexpr const enemy_data white_caveman(
        btn::sprite_items::stage_3_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_4, btn::fixed_size(38, 52), white_caveman_graphics_indexes, 8, 6);

    constexpr const enemy_move_event white_caveman_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.5)),  30, 0,  false),
        enemy_move_event(btn::fixed_point(1, move_y(0.5)),  60, 1,  false),
        enemy_move_event(btn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };


    // black_caveman:

    constexpr const btn::array<enemy_data::graphics_indexes_group, 4> black_caveman_graphics_indexes = {
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 9, 10, 11, 11 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 },
        enemy_data::graphics_indexes_group{ 6, 7, 6, 8 }
    };

    constexpr const enemy_data black_caveman(
        btn::sprite_items::stage_3_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_4, btn::fixed_size(38, 52), black_caveman_graphics_indexes, 6, 7, 8);

    constexpr const enemy_move_event black_caveman_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.5)),  60, 0,  false),
        enemy_move_event(btn::fixed_point(0, move_y(0)),  30, 1,  false),
        enemy_move_event(btn::fixed_point(0, move_y(0.5)),  1,  0,  false),
    };

    constexpr const enemy_bullet_event black_caveman_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_BIG,      1,  60 + 10),
        enemy_bullet_event(enemy_bullet_type::CAVEMAN_SMALL,    1,  15),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 3 slow

        // enemy_event(blonde, btn::fixed_point(55, start_y), blonde_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(black_caveman, btn::fixed_point(-45, start_y), black_caveman_moves, black_caveman_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_caveman, btn::fixed_point(-60, start_y), black_caveman_moves, black_caveman_bullets, 60, enemy_drop_type::NONE),
        enemy_event(black_caveman, btn::fixed_point(-30, start_y), black_caveman_moves, black_caveman_bullets, 60, enemy_drop_type::NONE),
    };


    // stage (start with level 5, exp 2350):

    constexpr const stage stage_3(
        btn::regular_bg_items::stage_3, btn::regular_bg_items::mountain_clouds, btn::sprite_items::stage_3_intro,
        btn::sprite_items::stage_3_intro_alt, "STAGE 3", "GIANT MOUNTAIN", btn::music_items::stardstm,
        enemy_events, boss::type::GIGABAT);
}

const stage& get()
{
    return stage_3;
}

}
