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

    constexpr enemy_move_event(const btn::fixed_point& _delta_position, int _duration_frames, bool _horizontal_flip) :
        delta_position(_delta_position),
        duration_frames(int16_t(_duration_frames)),
        horizontal_flip(_horizontal_flip)
    {
        BTN_CONSTEXPR_ASSERT(_duration_frames >= 1 && _duration_frames < btn::numeric_limits<int16_t>::max(),
                             "Invalid duration frames");
    }

    [[nodiscard]] constexpr enemy_move_event flipped() const
    {
        return enemy_move_event(btn::fixed_point(-delta_position.x(), delta_position.y()), duration_frames,
                                ! horizontal_flip);
    }
};

}

#endif
