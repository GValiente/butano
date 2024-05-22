/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_BULLET_EVENT_H
#define BF_GAME_ENEMY_BULLET_EVENT_H

#include "bn_fixed_point.h"
#include "bf_game_enemy_bullet_type.h"

namespace bf::game
{

class enemy_bullet_event
{

public:
    bn::fixed_point delta_position;
    bn::fixed delta_speed;
    int16_t wait_frames;
    enemy_bullet_type type;

    constexpr enemy_bullet_event(enemy_bullet_type _type, const bn::fixed_point& _delta_position, int _wait_frames) :
        delta_position(_delta_position),
        wait_frames(int16_t(_wait_frames)),
        type(_type)
    {
        BN_ASSERT(_delta_position != bn::fixed_point(), "Invalid delta position");
        BN_ASSERT(_wait_frames >= 1 && _wait_frames < bn::numeric_limits<int16_t>::max(),
                   "Invalid wait frames: ", _wait_frames);
    }

    constexpr enemy_bullet_event(enemy_bullet_type _type, bn::fixed _delta_speed, int _wait_frames) :
        delta_speed(_delta_speed),
        wait_frames(int16_t(_wait_frames)),
        type(_type)
    {
        BN_ASSERT(_delta_speed > 0, "Invalid delta speed: ", _delta_speed);
        BN_ASSERT(_wait_frames >= 1 && _wait_frames < bn::numeric_limits<int16_t>::max(),
                   "Invalid wait frames: ", _wait_frames);
    }

    [[nodiscard]] constexpr enemy_bullet_event flipped() const
    {
        enemy_bullet_event result(type, 1, wait_frames);
        result.delta_position.set_x(-delta_position.x());
        result.delta_position.set_y(delta_position.y());
        result.delta_speed = delta_speed;
        return result;
    }
};

}

#endif
