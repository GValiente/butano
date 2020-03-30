#include "bf_game_enemy.h"

#include "btn_sprite_builder.h"
#include "bf_constants.h"
#include "bf_game_enemy_data.h"

namespace bf::game
{

namespace
{
    btn::sprite_ptr _create_sprite(const enemy_data& data)
    {
        btn::sprite_builder builder(data.sprite_item, data.graphics_index_1);
        builder.set_position(data.start_position);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_horizontal_flip(data.move_events[0].horizontal_flip);
        return builder.release_build();
    }
}

enemy::enemy(const enemy_data& data) :
    _move_events(data.move_events),
    _sprite(_create_sprite(data)),
    _move_action(_sprite, _move_events[0].delta_position),
    _animate_action(btn::create_sprite_cached_animate_action_forever(
                        _sprite, 16, data.sprite_item, data.graphics_index_1, data.graphics_index_2)),
    _move_event_index(0),
    _move_event_counter(_move_events[0].duration_frames)
{
}

void enemy::update()
{
    BTN_ASSERT(! done(), "Enemy is done");

    --_move_event_counter;

    if(! _move_event_counter)
    {
        ++_move_event_index;

        if(done())
        {
            return;
        }

        const enemy_move_event& move_event = _move_events[_move_event_index];
        _move_action = btn::sprite_move_by_action(_sprite, move_event.delta_position);
        _move_event_counter = move_event.duration_frames;

        if(move_event.horizontal_flip)
        {
            _sprite.set_horizontal_flip(! _sprite.horizontal_flip());
        }
    }

    _move_action.update();
    _animate_action.update();
}

}
