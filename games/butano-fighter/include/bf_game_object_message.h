#ifndef BF_GAME_OBJECT_MESSAGE_H
#define BF_GAME_OBJECT_MESSAGE_H

#include "btn_sprite_actions.h"

namespace bf::game
{

class object_message
{

public:
    object_message(const btn::fixed_point& position, int graphics_index);

    [[nodiscard]] bool done() const
    {
        return _move_action.done();
    }

    void update();

private:
    btn::sprite_move_to_action _move_action;
    btn::sprite_cached_animate_action<2> _animate_action;
};

}

#endif
