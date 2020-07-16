#include "bf_game_stage_2.h"

#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_items_clouds.h"
#include "btn_regular_bg_items_stage_2.h"
#include "btn_sprite_items_stage_2_intro.h"
#include "btn_sprite_items_stage_2_intro_alt.h"
#include "btn_sprite_items_stage_2_monsters_1.h"
#include "btn_sprite_items_stage_2_characters_1.h"
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

    constexpr const enemy_data rat(btn::sprite_items::stage_2_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   btn::sound_items::dinosaur_5, btn::fixed_size(16, 16), _create_graphics_indexes(0),
                                   2, 2);

    constexpr const enemy_move_event rat_moves[] = {
        enemy_move_event(btn::fixed_point(0,    move_y(0.5)),   30, down_index,     false),
        enemy_move_event(btn::fixed_point(1.25, move_y(0)),     60, right_index,    false),
        enemy_move_event(btn::fixed_point(0,    move_y(-1.5)),  1,  up_index,       false),
    };


    // dog:

    constexpr const enemy_data dog(btn::sprite_items::stage_2_characters_1, enemy_data::death_anim_type::ROTATE,
                                   btn::sound_items::monster_2, btn::fixed_size(12, 10), 48, 49, 2, 4);

    constexpr const enemy_move_event dog_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr const enemy_move_event dog_flipped_moves[] = {
        dog_moves[0].flipped(),
        dog_moves[1].flipped(),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 0

        enemy_event(rat, btn::fixed_point(-45, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(rat, btn::fixed_point(-60, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(rat, btn::fixed_point(-30, start_y), rat_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(dog, btn::fixed_point(-60, start_y), dog_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(dog, btn::fixed_point(55, start_y), dog_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),
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
