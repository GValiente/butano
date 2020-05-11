#include "bf_game_stage_1.h"

#include "btn_display.h"
#include "btn_sound_items.h"
#include "btn_sprite_items_stage_1_animals.h"
#include "btn_sprite_items_stage_1_monsters_1.h"
#include "bf_constants.h"

namespace bf::game::stage_1
{

namespace
{
    constexpr const int start_y = -(btn::display::height() + 16) / 2;

    constexpr const btn::span<const enemy_bullet_event> no_bullets;


    // pig:

    constexpr const enemy_data pig(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   btn::sound_items::dinosaur_2, btn::fixed_size(16, 13), 2, 3, 1, 1);

    constexpr const enemy_move_event pig_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.4), int((-start_y * 2) / 0.4), true),
    };

    constexpr const enemy_move_event pig_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.4), int((-start_y * 2) / 0.4), false),
    };


    // sheep:

    constexpr const enemy_data sheep(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     btn::sound_items::kill, btn::fixed_size(16, 12), 0, 1, 2, 2);

    constexpr const enemy_move_event sheep_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.4), int((-start_y * 2) / 0.4), true),
    };

    constexpr const enemy_move_event sheep_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.4), int((-start_y * 2) / 0.4), false),
    };


    // bat:

    constexpr const enemy_data bat(btn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::ROTATE,
                                   btn::sound_items::dinosaur_5, btn::fixed_size(28, 16), 0, 1, 1, 1);

    constexpr const enemy_move_event bat_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((-start_y * 2) / 0.75), true),
    };

    constexpr const enemy_move_event bat_flipped_moves[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((-start_y * 2) / 0.75), false),
    };

    constexpr const enemy_bullet_event bat_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,     30),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,     60),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {
        enemy_event(pig, btn::fixed_point(-45, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-60, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-30, start_y), pig_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(40, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(25, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
    };


    // stage:

    constexpr const stage stage_1(enemy_events);
}

const stage& get()
{
    return stage_1;
}

}
