/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_object_message.h"

#include "bn_sprite_builder.h"
#include "bn_sprite_items_object_messages.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr int wait_frames = 180;
    constexpr int move_y = -wait_frames / 4;

    [[nodiscard]] bn::sprite_move_to_action _create_move_action(const bn::fixed_point& position, int graphics_index,
                                                                 const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::object_messages, graphics_index);
        builder.set_position(position);
        builder.set_z_order(constants::object_messages_z_order);
        builder.set_camera(camera);
        return bn::sprite_move_to_action(builder.release_build(), wait_frames, position.x(), position.y() + move_y);
    }

    [[nodiscard]] int _graphics_index(int experience)
    {
        switch(experience)
        {

        case 1:
            return 4;

        case 2:
            return 6;

        case 3:
            return 8;

        case 4:
            return 10;

        case 5:
            return 12;

        case 10:
            return 14;

        case 15:
            return 16;

        case 20:
            return 18;

        case 25:
            return 20;

        case 30:
            return 22;

        case 35:
            return 24;

        case 40:
            return 26;

        case 45:
            return 28;

        case 50:
            return 30;

        case 60:
            return 32;

        case 70:
            return 34;

        case 75:
            return 36;

        case 80:
            return 38;

        case 90:
            return 40;

        case 100:
            return 42;

        case 105:
            return 44;

        case 120:
            return 46;

        case 125:
            return 48;

        case 140:
            return 50;

        case 150:
            return 52;

        case 160:
            return 54;

        case 175:
            return 56;

        case 200:
            return 58;

        default:
            BN_ERROR("Invalid experience: ", experience);
            return 0;
        }
    }
}

object_message object_message::create_experience(const bn::fixed_point& position, int experience,
                                                 const bn::camera_ptr& camera)
{
    return object_message(position, _graphics_index(experience), camera);
}

void object_message::update()
{
    _move_action.update();
    _animate_action.update();
}

object_message::object_message(const bn::fixed_point& position, int graphics_index, const bn::camera_ptr& camera) :
    _move_action(_create_move_action(position, graphics_index, camera)),
    _animate_action(bn::create_sprite_cached_animate_action_forever(
                        _move_action.sprite(), 16, bn::sprite_items::object_messages.tiles_item(),
                        graphics_index, graphics_index + 1))
{
}

}
