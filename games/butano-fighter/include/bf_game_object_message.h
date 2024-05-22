/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_OBJECT_MESSAGE_H
#define BF_GAME_OBJECT_MESSAGE_H

#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

namespace bf::game
{

class object_message
{

public:
    [[nodiscard]] static object_message create_level_up(const bn::fixed_point& position, const bn::camera_ptr& camera)
    {
        return object_message(position, 0, camera);
    }

    [[nodiscard]] static object_message create_bomb(const bn::fixed_point& position, const bn::camera_ptr& camera)
    {
        return object_message(position, 2, camera);
    }

    [[nodiscard]] static object_message create_experience(const bn::fixed_point& position, int experience,
                                                          const bn::camera_ptr& camera);

    [[nodiscard]] bool done() const
    {
        return _move_action.done();
    }

    void update();

private:
    bn::sprite_move_to_action _move_action;
    bn::sprite_cached_animate_action<2> _animate_action;

    object_message(const bn::fixed_point& position, int graphics_index, const bn::camera_ptr& camera);
};

}

#endif
