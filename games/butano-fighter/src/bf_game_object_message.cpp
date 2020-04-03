#include "bf_game_object_message.h"

#include "btn_sprite_builder.h"
#include "btn_object_messages_sprite_item.h"
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

object_message::object_message(const btn::fixed_point& position, int graphics_index) :
    _move_action(_create_move_action(position, graphics_index)),
    _animate_action(btn::create_sprite_cached_animate_action_forever(
                        _move_action.sprite(), 16, btn::sprite_items::object_messages,
                        graphics_index, graphics_index + 1))
{
}

void object_message::update()
{
    _move_action.update();
    _animate_action.update();
}

}
