#include "bf_game_object_message.h"

#include "btn_sprite_builder.h"
#include "btn_sprite_items_object_messages.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const int wait_frames = 180;
    constexpr const int move_y = -wait_frames / 4;

    btn::sprite_move_to_action _create_move_action(const btn::fixed_point& position, int graphics_index)
    {
        btn::sprite_builder builder(btn::sprite_items::object_messages, graphics_index);
        builder.set_position(position);
        builder.set_z_order(constants::object_messages_z_order);
        return btn::sprite_move_to_action(builder.release_build(), wait_frames, position.x(), position.y() + move_y);
    }
}

object_message object_message::create_experience(const btn::fixed_point& position, int experience)
{
    int graphics_index = 0;

    switch(experience)
    {

    case 1:
        graphics_index = 4;
        break;

    case 2:
        graphics_index = 6;
        break;

    case 3:
        graphics_index = 8;
        break;

    case 4:
        graphics_index = 10;
        break;

    case 5:
        graphics_index = 12;
        break;

    case 10:
        graphics_index = 14;
        break;

    case 15:
        graphics_index = 16;
        break;

    case 20:
        graphics_index = 18;
        break;

    case 25:
        graphics_index = 20;
        break;

    case 30:
        graphics_index = 22;
        break;

    case 40:
        graphics_index = 24;
        break;

    case 45:
        graphics_index = 26;
        break;

    case 50:
        graphics_index = 28;
        break;

    case 60:
        graphics_index = 30;
        break;

    case 75:
        graphics_index = 32;
        break;

    default:
        BTN_ERROR("Invalid experience: ", experience);
        break;
    }

    return object_message(position, graphics_index);
}

void object_message::update()
{
    _move_action.update();
    _animate_action.update();
}

object_message::object_message(const btn::fixed_point& position, int graphics_index) :
    _move_action(_create_move_action(position, graphics_index)),
    _animate_action(btn::create_sprite_cached_animate_action_forever(
                        _move_action.sprite(), 16, btn::sprite_items::object_messages,
                        graphics_index, graphics_index + 1))
{
}

}
