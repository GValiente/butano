#include "bf_game_stage_4.h"

#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_items_stars.h"
#include "btn_regular_bg_items_stage_4.h"
#include "btn_sprite_items_stage_4_intro.h"
#include "btn_sprite_items_stage_4_brain.h"
#include "btn_sprite_items_stage_4_octopus.h"
#include "btn_sprite_items_stage_4_intro_alt.h"
#include "btn_sprite_items_stage_4_enemies_1.h"
#include "btn_sprite_items_stage_4_enemies_2.h"
#include "btn_sprite_items_stage_4_enemies_4.h"
#include "btn_sprite_items_stage_4_enemies_3_1.h"
#include "btn_sprite_items_stage_4_enemies_3_2.h"
#include "btn_sprite_items_stage_4_meteors_big.h"
#include "btn_sprite_items_stage_4_meteors_small.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_4
{

namespace
{
    constexpr const int start_y_small = -(btn::display::height() + 32) / 2;

    constexpr const btn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr btn::fixed move_y(btn::fixed y)
    {
        return y + constants::background_speed;
    }

    [[nodiscard]] constexpr btn::array<enemy_data::graphics_indexes_group, 4> _create_graphics_indexes(int index)
    {
        return btn::array<enemy_data::graphics_indexes_group, 4> {
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) },
            enemy_data::graphics_indexes_group{ uint16_t(index), uint16_t(index), uint16_t(index), uint16_t(index) }
        };
    }


    // octopus:

    constexpr const enemy_data octopus(
        btn::sprite_items::stage_4_octopus, enemy_data::death_anim_type::ROTATE,
        btn::sound_items::enemy_death_ninja_14, btn::fixed_size(26, 24), _create_graphics_indexes(0), 12, 22);

    constexpr const enemy_move_event octopus_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.25)),  50, false),
        enemy_move_event(btn::fixed_point(0.75, 0),             40, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1)),    1,  false),
    };


    // white_astronaut:

    constexpr const enemy_data white_astronaut(
        btn::sprite_items::stage_4_enemies_3_2, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_4, btn::fixed_size(22, 24), _create_graphics_indexes(0), 18, 28);

    constexpr const enemy_move_event white_astronaut_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr const enemy_bullet_event white_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  32),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),  32),
    };


    // blue_astronaut:

    constexpr const enemy_data blue_astronaut(
        btn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_3, btn::fixed_size(22, 24), _create_graphics_indexes(0), 26, 36);

    constexpr const enemy_move_event blue_astronaut_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr const enemy_bullet_event blue_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  32),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_BIG,    direction_vector(0, 1, 1),  32),
    };


    // red_astronaut:

    constexpr const enemy_data red_astronaut(
        btn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_5, btn::fixed_size(22, 24), _create_graphics_indexes(1), 20, 30);

    constexpr const enemy_move_event red_astronaut_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr const enemy_bullet_event red_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0, 1, 1),      45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(-0.5, 1, 1),   1),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  direction_vector(0.5, 1, 1),    1),
    };


    // yellow_astronaut:

    constexpr const enemy_data yellow_astronaut(
        btn::sprite_items::stage_4_enemies_4, enemy_data::death_anim_type::VERTICAL_SCALE,
        btn::sound_items::scream_6, btn::fixed_size(22, 24), _create_graphics_indexes(2), 16, 26);

    constexpr const enemy_move_event yellow_astronaut_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.2)),  1,  false),
    };

    constexpr const enemy_bullet_event yellow_astronaut_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  1,  45),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  1,  32),
        enemy_bullet_event(enemy_bullet_type::ASTRONAUT_SMALL,  1,  32),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 7 slow

        enemy_event(yellow_astronaut, btn::fixed_point(-55, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 85, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, btn::fixed_point(-40, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 85, enemy_drop_type::NONE),
        enemy_event(yellow_astronaut, btn::fixed_point(-25, start_y_small), yellow_astronaut_moves, yellow_astronaut_bullets, 85, enemy_drop_type::NONE),
    };


    // stage (start with level 7, exp 5600):

    constexpr const stage stage_4(
        btn::regular_bg_items::stage_4, btn::fixed_point(0, constants::background_speed / 8),
        btn::regular_bg_items::stars, btn::fixed_point(0, constants::background_speed),
        btn::sprite_items::stage_4_intro, btn::sprite_items::stage_4_intro_alt, "STAGE 4", "FAR FAR AWAY",
        btn::music_items::galgox_snowheart_melody, 0.8, enemy_events, boss::type::WIZARD, true);
}

const stage& get()
{
    return stage_4;
}

}
