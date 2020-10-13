#ifndef BTN_CAMERA_ACTIONS_H
#define BTN_CAMERA_ACTIONS_H

#include "btn_camera_ptr.h"
#include "btn_fixed_point.h"
#include "btn_value_template_actions.h"

namespace btn
{

// position

class camera_position_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const camera_ptr& camera)
    {
        return camera.position();
    }

    static void set(const fixed_point& position, camera_ptr& camera)
    {
        camera.set_position(position);
    }
};


class camera_move_by_action : public by_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    camera_move_by_action(const camera_ptr& camera, fixed delta_x, fixed delta_y) :
        by_value_template_action(camera, fixed_point(delta_x, delta_y))
    {
    }

    camera_move_by_action(camera_ptr&& camera, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(camera), fixed_point(delta_x, delta_y))
    {
    }

    camera_move_by_action(const camera_ptr& camera, const fixed_point& delta_position) :
        by_value_template_action(camera, delta_position)
    {
    }

    camera_move_by_action(camera_ptr&& camera, const fixed_point& delta_position) :
        by_value_template_action(move(camera), delta_position)
    {
    }

    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


class camera_move_to_action : public to_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    camera_move_to_action(const camera_ptr& camera, int duration_frames, fixed final_x, fixed final_y) :
        to_value_template_action(camera, duration_frames, fixed_point(final_x, final_y))
    {
    }

    camera_move_to_action(camera_ptr&& camera, int duration_frames, fixed final_x, fixed final_y) :
        to_value_template_action(move(camera), duration_frames, fixed_point(final_x, final_y))
    {
    }

    camera_move_to_action(const camera_ptr& camera, int duration_frames, const fixed_point& final_position) :
        to_value_template_action(camera, duration_frames, final_position)
    {
    }

    camera_move_to_action(camera_ptr&& camera, int duration_frames, const fixed_point& final_position) :
        to_value_template_action(move(camera), duration_frames, final_position)
    {
    }

    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class camera_move_loop_action : public loop_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    camera_move_loop_action(const camera_ptr& camera, int duration_frames, fixed final_x, fixed final_y) :
        loop_value_template_action(camera, duration_frames, fixed_point(final_x, final_y))
    {
    }

    camera_move_loop_action(camera_ptr&& camera, int duration_frames, fixed final_x, fixed final_y) :
        loop_value_template_action(move(camera), duration_frames, fixed_point(final_x, final_y))
    {
    }

    camera_move_loop_action(const camera_ptr& camera, int duration_frames, const fixed_point& final_position) :
        loop_value_template_action(camera, duration_frames, final_position)
    {
    }

    camera_move_loop_action(camera_ptr&& camera, int duration_frames, const fixed_point& final_position) :
        loop_value_template_action(move(camera), duration_frames, final_position)
    {
    }

    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class camera_move_toggle_action : public toggle_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    camera_move_toggle_action(const camera_ptr& camera, int duration_frames, fixed new_x, fixed new_y) :
        toggle_value_template_action(camera, duration_frames, fixed_point(new_x, new_y))
    {
    }

    camera_move_toggle_action(camera_ptr&& camera, int duration_frames, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(camera), duration_frames, fixed_point(new_x, new_y))
    {
    }

    camera_move_toggle_action(const camera_ptr& camera, int duration_frames, const fixed_point& new_position) :
        toggle_value_template_action(camera, duration_frames, new_position)
    {
    }

    camera_move_toggle_action(camera_ptr&& camera, int duration_frames, const fixed_point& new_position) :
        toggle_value_template_action(move(camera), duration_frames, new_position)
    {
    }

    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};

}

#endif
