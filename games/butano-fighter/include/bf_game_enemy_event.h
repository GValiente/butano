/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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
    bn::fixed_point start_position;
    bn::span<const enemy_move_event> move_events;
    bn::span<const enemy_bullet_event> bullet_events;
    int wait_frames;
    enemy_drop_type drop;

    constexpr enemy_event(const enemy_data& _enemy, const bn::fixed_point& _start_position,
                          const bn::span<const enemy_move_event>& _move_events,
                          const bn::span<const enemy_bullet_event>& _bullet_events, int _wait_frames,
                          enemy_drop_type _drop) :
        enemy(_enemy),
        start_position(_start_position),
        move_events(_move_events),
        bullet_events(_bullet_events),
        wait_frames(_wait_frames),
        drop(_drop)
    {
        BN_ASSERT(move_events.size() > 0 && move_events.size() < bn::numeric_limits<int8_t>::max(),
                   "Invalid move events count: ", move_events.size());
        BN_ASSERT(bullet_events.size() < bn::numeric_limits<int8_t>::max(),
                   "Invalid bullet events count: ", bullet_events.size());
        BN_ASSERT(_wait_frames >= 0, "Invalid wait frames: ", _wait_frames);
    }
};

}

#endif
