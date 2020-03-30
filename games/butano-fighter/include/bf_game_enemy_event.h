#ifndef BF_GAME_ENEMY_EVENT_H
#define BF_GAME_ENEMY_EVENT_H

#include "bf_game_enemy_data.h"

namespace bf::game
{

class enemy_event
{

public:
    const enemy_data& enemy;
    int wait_frames;

    constexpr enemy_event(const enemy_data& _enemy, int _wait_frames) :
        enemy(_enemy),
        wait_frames(_wait_frames)
    {
        BTN_CONSTEXPR_ASSERT(_wait_frames >= 1, "Invalid wait frames");
    }
};

}

#endif
