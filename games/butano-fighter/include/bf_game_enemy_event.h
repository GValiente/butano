/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_EVENT_H
#define BF_GAME_ENEMY_EVENT_H

#include "bf_game_enemy_data.h"
#include "bf_game_enemy_drop_type.h"
#include "bf_game_enemy_move_event.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

class enemy_event
{

public:

    const enemy_data& enemy;
    btn::fixed_point start_position;
    btn::span<const enemy_move_event> move_events;
    btn::span<const enemy_bullet_event> bullet_events;
    int wait_frames;
    enemy_drop_type drop;

    constexpr enemy_event(const enemy_data& _enemy, const btn::fixed_point& _start_position,
                          const btn::span<const enemy_move_event>& _move_events,
                          const btn::span<const enemy_bullet_event>& _bullet_events, int _wait_frames,
                          enemy_drop_type _drop) :
        enemy(_enemy),
        start_position(_start_position),
        move_events(_move_events),
        bullet_events(_bullet_events),
        wait_frames(_wait_frames),
        drop(_drop)
    {
        BTN_ASSERT(move_events.size() > 0 && move_events.size() < btn::numeric_limits<int8_t>::max(),
                   "Invalid move events count: ", move_events.size());
        BTN_ASSERT(bullet_events.size() < btn::numeric_limits<int8_t>::max(),
                   "Invalid bullet events count: ", bullet_events.size());
        BTN_ASSERT(_wait_frames >= 0, "Invalid wait frames: ", _wait_frames);
    }
};

}

#endif
