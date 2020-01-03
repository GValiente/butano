#ifndef BTN_SPRITE_MOVE_TO_ACTION_H
#define BTN_SPRITE_MOVE_TO_ACTION_H

#include "btn_action.h"
#include "btn_utility.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"

namespace btn
{

class sprite_move_to_action : public action
{

public:
    sprite_move_to_action(sprite_ptr sprite, int duration_frames, fixed final_x, fixed final_y) :
        sprite_move_to_action(move(sprite), duration_frames, fixed_point(final_x, final_y))
    {
    }

    sprite_move_to_action(sprite_ptr sprite, int duration_frames, const fixed_point& final_position) :
        _sprite(move(sprite)),
        _final_position(final_position),
        _initial_position(_sprite.position()),
        _delta_position(_calculate_delta_position(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    void reset()
    {
        _sprite.set_position(_initial_position);
        _current_frame = 0;
    }

    void update() override
    {
        BTN_ASSERT(! done(), "Action is done");

        ++_current_frame;

        if(_current_frame == _duration_frames)
        {
            _sprite.set_position(_final_position);
        }
        else
        {
            _sprite.set_position(_sprite.position() + _delta_position);
        }
    }

    bool done() const override
    {
        return _current_frame == _duration_frames;
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return _final_position;
    }

protected:
    sprite_ptr _sprite;
    fixed_point _final_position;
    fixed_point _initial_position;
    fixed_point _delta_position;
    int _duration_frames;
    int _current_frame = 0;

    [[nodiscard]] fixed_point _calculate_delta_position(int duration_frames) const
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration_frames: ", duration_frames);

        fixed_point diff = _final_position - _initial_position;
        return fixed_point(diff.x() / duration_frames, diff.y() / duration_frames);
    }
};

}

#endif
