#ifndef BF_GAME_ENEMY_DATA_H
#define BF_GAME_ENEMY_DATA_H

#include "btn_span.h"
#include "btn_fixed_size.h"
#include "btn_sound_item.h"
#include "btn_sprite_item.h"
#include "bf_game_enemy_move_event.h"

namespace bf::game
{

class enemy_data
{

public:
    const btn::sprite_item& sprite_item;
    btn::sound_item death_sound_item;
    btn::fixed_point start_position;
    btn::fixed_size size;
    int16_t graphics_index_1;
    int16_t graphics_index_2;
    btn::span<const enemy_move_event> move_events;

    constexpr enemy_data(const btn::sprite_item& _sprite_item, const btn::sound_item& _death_sound_item,
                         const btn::fixed_point& _start_position, const btn::fixed_size& _size,
                         int _graphics_index_1, int _graphics_index_2,
                         const btn::span<const enemy_move_event>& _move_events) :
        sprite_item(_sprite_item),
        death_sound_item(_death_sound_item),
        start_position(_start_position),
        size(_size),
        graphics_index_1(int16_t(_graphics_index_1)),
        graphics_index_2(int16_t(_graphics_index_2)),
        move_events(_move_events)
    {
        BTN_CONSTEXPR_ASSERT(graphics_index_1 >= 0 && graphics_index_1 < sprite_item.tiles_item().graphics_count(),
                             "Invalid graphics index 1");
        BTN_CONSTEXPR_ASSERT(graphics_index_2 >= 0 && graphics_index_2 < sprite_item.tiles_item().graphics_count(),
                             "Invalid graphics index 2");
        BTN_CONSTEXPR_ASSERT(! move_events.empty(), "Move events is empty");
    }
};

}

#endif
