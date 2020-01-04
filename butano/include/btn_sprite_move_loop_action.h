#ifndef BTN_SPRITE_MOVE_LOOP_ACTION_H
#define BTN_SPRITE_MOVE_LOOP_ACTION_H

#include "btn_action.h"
#include "btn_sprite_ptr.h"
#include "btn_fixed_point.h"

namespace btn
{

class sprite_move_loop_action : public action
{

public:
    sprite_move_loop_action(sprite_ptr sprite, int duration_frames, fixed final_x, fixed final_y);

    sprite_move_loop_action(sprite_ptr sprite, int duration_frames, const fixed_point& final_position);

    void reset();

    void update() override;

    bool done() const override
    {
        return false;
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
    bool _reverse = false;
    uint16_t _current_frame = 0;
    sprite_ptr _sprite;
    fixed_point _final_position;
    fixed_point _initial_position;
    fixed_point _delta_position;
    int _duration_frames;

    [[nodiscard]] fixed_point _calculate_delta_position(int duration_frames) const;
};

}

#endif
