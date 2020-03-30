#include "bf_game_stage_1.h"

#include "btn_sound_items.h"
#include "btn_stage_1_monsters_1_sprite_item.h"
#include "bf_constants.h"

namespace bf::game::stage_1
{

namespace
{
    // bat:

    constexpr const enemy_move_event bat_move_events[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((constants::view_height * 2) / 0.75), false),
    };

    constexpr const enemy_data bat(
        btn::sprite_items::stage_1_monsters_1, btn::sound_items::dinosaur_5,
        btn::fixed_point(-constants::play_width + 20, -constants::view_height),
        btn::fixed_size(12, 6), 0, 1, bat_move_events);


    // flipped bat:

    constexpr const enemy_move_event flipped_bat_move_events[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((constants::view_height * 2) / 0.75), true),
    };

    constexpr const enemy_data flipped_bat(
        btn::sprite_items::stage_1_monsters_1, btn::sound_items::dinosaur_5,
        btn::fixed_point(constants::play_width - 20, -constants::view_height),
        btn::fixed_size(12, 6), 0, 1, bat_move_events);


    // enemy events:

    constexpr const enemy_event enemy_events[] = {
        enemy_event(bat, 60),
        enemy_event(bat, 60),
        enemy_event(bat, 180),

        enemy_event(flipped_bat, 60),
        enemy_event(flipped_bat, 60),
        enemy_event(flipped_bat, 60),
    };


    // stage:

    constexpr const stage stage_1(enemy_events);
}

const stage& get()
{
    return stage_1;
}

}
