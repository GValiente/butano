/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_BULLET_EVENT_H
#define BF_GAME_ENEMY_BULLET_EVENT_H

#include "btn_fixed_point.h"
#include "bf_game_enemy_bullet_type.h"

namespace bf::game
{

class enemy_bullet_event
{

public:
    btn::fixed_point delta_position;
    btn::fixed delta_speed;
    int16_t wait_frames;
    enemy_bullet_type type;

    constexpr enemy_bullet_event(enemy_bullet_type _type, const btn::fixed_point& _delta_position, int _wait_frames) :
        delta_position(_delta_position),
        wait_frames(int16_t(_wait_frames)),
        type(_type)
    {
        BTN_ASSERT(_delta_position != btn::fixed_point(), "Invalid delta position");
        BTN_ASSERT(_wait_frames >= 1 && _wait_frames < btn::numeric_limits<int16_t>::max(),
                   "Invalid wait frames: ", _wait_frames);
    }

    constexpr enemy_bullet_event(enemy_bullet_type _type, btn::fixed _delta_speed, int _wait_frames) :
        delta_speed(_delta_speed),
        wait_frames(int16_t(_wait_frames)),
        type(_type)
    {
        BTN_ASSERT(_delta_speed > 0, "Invalid delta speed: ", _delta_speed);
        BTN_ASSERT(_wait_frames >= 1 && _wait_frames < btn::numeric_limits<int16_t>::max(),
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
