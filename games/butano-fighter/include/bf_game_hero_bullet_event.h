#ifndef BF_GAME_HERO_BULLET_EVENT_H
#define BF_GAME_HERO_BULLET_EVENT_H

#include "btn_fixed_point.h"
#include "bf_constants.h"

namespace bf::game
{

class hero_bullet_event
{

public:
    btn::fixed_point direction;
    int8_t frame;
    int8_t level;
    bool play_sound;

    constexpr hero_bullet_event(const btn::fixed_point& _direction, int _frame, int _level, bool _play_sound) :
        direction(_direction),
        frame(int8_t(_frame)),
        level(int8_t(_level)),
        play_sound(_play_sound)
    {
        BTN_ASSERT(_direction != btn::fixed_point(), "Invalid direction");
        BTN_ASSERT(_frame >= 1 && _frame < btn::numeric_limits<int8_t>::max(), "Invalid frame: ", _frame);
        BTN_ASSERT(_level >= 0 && _level < constants::hero_bullet_levels, "Invalid level: ", _level);
    }
};

}

#endif
