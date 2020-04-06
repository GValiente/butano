#include "bf_game_stage_1.h"

#include "btn_sound_items.h"
#include "btn_stage_1_monsters_1_sprite_item.h"
#include "bf_constants.h"

namespace bf::game::stage_1
{

namespace
{
    constexpr const int start_y = -(160 + 16) / 2;


    // bat:

    constexpr const enemy_data bat(btn::sprite_items::stage_1_monsters_1, btn::sound_items::dinosaur_5,
                                   btn::fixed_size(12, 6), 0, 1, 1, 1);

    constexpr const enemy_move_event bat_move_events[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((-start_y * 2) / 0.75), true),
    };

    constexpr const enemy_move_event bat_flipped_move_events[] = {
        enemy_move_event(btn::fixed_point(0, 0.75), int((-start_y * 2) / 0.75), false),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {
        enemy_event(bat,    btn::fixed_point(-constants::play_width + 20, start_y),     bat_move_events,
                60,     enemy_event::drop_type::NONE),
        enemy_event(bat,    btn::fixed_point(-constants::play_width + 20, start_y),     bat_move_events,
                60,     enemy_event::drop_type::NONE),
        enemy_event(bat,    btn::fixed_point(-constants::play_width + 20, start_y),     bat_move_events,
                180,    enemy_event::drop_type::BOMB),

        enemy_event(bat,    btn::fixed_point(constants::play_width - 20, start_y),      bat_flipped_move_events,
                60,     enemy_event::drop_type::NONE),
        enemy_event(bat,    btn::fixed_point(constants::play_width - 20, start_y),      bat_flipped_move_events,
                60,     enemy_event::drop_type::NONE),
        enemy_event(bat,    btn::fixed_point(constants::play_width - 20, start_y),      bat_flipped_move_events,
                60,     enemy_event::drop_type::NONE),
    };


    // stage:

    constexpr const stage stage_1(enemy_events);
}

const stage& get()
{
    return stage_1;
}

}
