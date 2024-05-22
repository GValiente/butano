/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_HERO_BULLET_EVENT_H
#define BF_GAME_HERO_BULLET_EVENT_H

#include "bn_limits.h"
#include "bn_fixed_point.h"
#include "bf_constants.h"

namespace bf::game
{

class hero_bullet_event
{

public:
    bn::fixed_point direction;
    int8_t frame;
    int8_t level;
    bool play_sound;

    constexpr hero_bullet_event(const bn::fixed_point& _direction, int _frame, int _level, bool _play_sound) :
        direction(_direction),
        frame(int8_t(_frame)),
        level(int8_t(_level)),
        play_sound(_play_sound)
    {
        BN_ASSERT(_direction != bn::fixed_point(), "Invalid direction");
        BN_ASSERT(_frame >= 1 && _frame < bn::numeric_limits<int8_t>::max(), "Invalid frame: ", _frame);
        BN_ASSERT(_level >= 0 && _level < constants::hero_bullet_levels, "Invalid level: ", _level);
    }
};

}

#endif
