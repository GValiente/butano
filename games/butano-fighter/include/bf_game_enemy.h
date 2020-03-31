#ifndef BF_GAME_ENEMY_H
#define BF_GAME_ENEMY_H

#include "btn_sprite_actions.h"
#include "bf_game_enemy_move_event.h"

namespace bf::game
{

class enemy_event;

class enemy
{

public:
    explicit enemy(const enemy_event& event);

    [[nodiscard]] const btn::fixed_point& position() const
    {
        return _sprite.position();
    }

    [[nodiscard]] bool done() const
    {
        return _move_event_index == _move_events.size();
    }

    void update();

private:
    btn::span<const enemy_move_event> _move_events;
    btn::sprite_ptr _sprite;
    btn::sprite_move_by_action _move_action;
    btn::sprite_cached_animate_action<2> _animate_action;
    int _move_event_index;
    int _move_event_counter;

};

}

#endif
