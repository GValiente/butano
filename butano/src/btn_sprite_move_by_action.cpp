#include "btn_sprite_move_by_action.h"

#include "btn_utility.h"

namespace btn
{

sprite_move_by_action::sprite_move_by_action(sprite_ptr sprite, fixed delta_x, fixed delta_y) :
    sprite_move_by_action(move(sprite), fixed_point(delta_x, delta_y))
{
}

sprite_move_by_action::sprite_move_by_action(sprite_ptr sprite, const fixed_point& delta_position) :
    _sprite(move(sprite)),
    _delta_position(delta_position),
    _initial_position(_sprite.position())
{
}

}
