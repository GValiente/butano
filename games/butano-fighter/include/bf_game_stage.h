#ifndef BF_GAME_STAGE_H
#define BF_GAME_STAGE_H

#include "bf_game_enemy_event.h"

namespace bf::game
{

class stage
{

public:
    btn::span<const enemy_event> enemy_events;

    constexpr stage(const btn::span<const enemy_event>& _enemy_events) :
        enemy_events(_enemy_events)
    {
        BTN_CONSTEXPR_ASSERT(! enemy_events.empty(), "Enemy events is empty");
    }
};

}

#endif
