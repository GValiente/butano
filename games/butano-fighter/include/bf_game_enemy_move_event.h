/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_MOVE_EVENT_H
#define BF_GAME_ENEMY_MOVE_EVENT_H

#include "bn_fixed_point.h"

namespace bf::game
{

class enemy_move_event
{

public:
    bn::fixed_point delta_position;
    int16_t duration_frames;
    int8_t animation_index;
    bool horizontal_flip;

    constexpr enemy_move_event(const bn::fixed_point& _delta_position, int _duration_frames, bool _horizontal_flip) :
        enemy_move_event(_delta_position, _duration_frames, 0, _horizontal_flip)
    {
    }

    constexpr enemy_move_event(const bn::fixed_point& _delta_position, int _duration_frames, int _animation_index,
                               bool _horizontal_flip) :
        delta_position(_delta_position),
        duration_frames(int16_t(_duration_frames)),
        animation_index(int8_t(_animation_index)),
        horizontal_flip(_horizontal_flip)
    {
        BN_ASSERT(_duration_frames >= 1 && _duration_frames < bn::numeric_limits<int16_t>::max(),
                   "Invalid duration frames: ", _duration_frames);
        BN_ASSERT(_animation_index >= 0 && _animation_index < 4, "Invalid animation index: ", _animation_index);
    }

    [[nodiscard]] constexpr enemy_move_event flipped() const
    {
        return enemy_move_event(bn::fixed_point(-delta_position.x(), delta_position.y()), duration_frames,
                                animation_index, ! horizontal_flip);
    }
};

}

#endif
