#include "btn_sprite_move_loop_action.h"

#include "btn_utility.h"

namespace btn
{

sprite_move_loop_action::sprite_move_loop_action(sprite_ptr sprite, int duration_frames, fixed final_x, fixed final_y) :
    sprite_move_loop_action(move(sprite), duration_frames, fixed_point(final_x, final_y))
{
}

sprite_move_loop_action::sprite_move_loop_action(sprite_ptr sprite, int duration_frames,
                                                 const fixed_point& final_position) :
    _sprite(move(sprite)),
    _final_position(final_position),
    _initial_position(_sprite.position()),
    _delta_position(_calculate_delta_position(duration_frames)),
    _duration_frames(duration_frames)
{
}

void sprite_move_loop_action::reset()
{
    _sprite.set_position(_initial_position);
    _current_frame = 0;
    _reverse = false;
}

void sprite_move_loop_action::update()
{
    ++_current_frame;

    if(_current_frame == _duration_frames)
    {
        _current_frame = 0;

        if(_reverse)
        {
            _sprite.set_position(_initial_position);
            _reverse = false;
        }
        else
        {
            _sprite.set_position(_final_position);
            _reverse = true;
        }
    }
    else
    {
        if(_reverse)
        {
            _sprite.set_position(_sprite.position() - _delta_position);
        }
        else
        {
            _sprite.set_position(_sprite.position() + _delta_position);
        }
    }
}

fixed_point sprite_move_loop_action::_calculate_delta_position(int duration_frames) const
{
    BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    BTN_ASSERT(duration_frames <= integral_limits<decltype(_current_frame)>::max,
               "Too much duration frames: ", duration_frames);

    fixed_point diff = _final_position - _initial_position;
    return fixed_point(diff.x() / duration_frames, diff.y() / duration_frames);
}

}
