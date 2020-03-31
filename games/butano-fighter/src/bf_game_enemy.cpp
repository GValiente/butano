#include "bf_game_enemy.h"

#include "btn_sprite_builder.h"
#include "bf_game_enemy_event.h"

namespace bf::game
{

namespace
{
    btn::sprite_ptr _create_sprite(const enemy_event& event)
    {
        const enemy_data& enemy_data = event.enemy;
        btn::sprite_builder builder(enemy_data.sprite_item, enemy_data.graphics_index_1);
        builder.set_position(event.start_position);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_horizontal_flip(event.move_events[0].horizontal_flip);
        return builder.release_build();
    }

    btn::sprite_cached_animate_action<2> _create_animate_action(const btn::sprite_ptr& sprite, const enemy_event& event)
    {
        const enemy_data& enemy_data = event.enemy;
        return btn::create_sprite_cached_animate_action_forever(
                    sprite, 16, enemy_data.sprite_item, enemy_data.graphics_index_1, enemy_data.graphics_index_2);
    }
}

enemy::enemy(const enemy_event& event) :
    _move_events(event.move_events),
    _sprite(_create_sprite(event)),
    _move_action(_sprite, _move_events[0].delta_position),
    _animate_action(_create_animate_action(_sprite, event)),
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
