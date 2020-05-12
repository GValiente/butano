#ifndef BF_GAME_OBJECT_MESSAGE_H
#define BF_GAME_OBJECT_MESSAGE_H

#include "btn_sprite_actions.h"

namespace bf::game
{

class object_message
{

public:
    [[nodiscard]] static object_message create_level_up(const btn::fixed_point& position)
    {
        return object_message(position, 0);
    }

    [[nodiscard]] static object_message create_bomb(const btn::fixed_point& position)
    {
        return object_message(position, 2);
    }

    [[nodiscard]] static object_message create_experience(const btn::fixed_point& position, int experience);

    [[nodiscard]] bool done() const
    {
        return _move_action.done();
    }

    void update();

private:
    btn::sprite_move_to_action _move_action;
    btn::sprite_cached_animate_action<2> _animate_action;

    object_message(const btn::fixed_point& position, int graphics_index);
};

}

#endif
