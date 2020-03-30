#ifndef BF_GAME_ENEMY_MOVE_EVENT_H
#define BF_GAME_ENEMY_MOVE_EVENT_H

#include "btn_fixed_point.h"

namespace bf::game
{

class enemy_move_event
{

public:
    btn::fixed_point delta_position;
    int16_t duration_frames;
    bool horizontal_flip;

    constexpr enemy_move_event(const btn::fixed_point& _position_delta, int _duration, bool _flip) :
        delta_position(_position_delta),
        duration_frames(int16_t(_duration)),
        horizontal_flip(_flip)
    {
        BTN_CONSTEXPR_ASSERT(_duration >= 1 && _duration < btn::numeric_limits<int16_t>::max(), "Invalid duration");
    }
};

}

#endif
