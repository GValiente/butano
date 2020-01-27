#ifndef BTN_BG_ACTIONS_H
#define BTN_BG_ACTIONS_H

#include "btn_bg_ptr.h"
#include "btn_fixed_point.h"
#include "btn_template_actions.h"

namespace btn
{

// position

class bg_position_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const bg_ptr& bg)
    {
        return bg.position();
    }

    static void set(const fixed_point& position, bg_ptr& bg)
    {
        bg.set_position(position);
    }
};


class bg_move_by_action :
        public by_template_action<bg_ptr, fixed_point, bg_position_manager>
{

public:
    template<class BgPtr>
    bg_move_by_action(BgPtr&& bg, fixed delta_x, fixed delta_y) :
        by_template_action(forward<BgPtr>(bg), fixed_point(delta_x, delta_y))
    {
    }

    template<class BgPtr>
    bg_move_by_action(BgPtr&& bg, const fixed_point& delta_position) :
        by_template_action(forward<BgPtr>(bg), delta_position)
    {
    }

    [[nodiscard]] const bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


class bg_move_to_action : public to_template_action<bg_ptr, fixed_point, bg_position_manager>
{

public:
    template<class BgPtr>
    bg_move_to_action(BgPtr&& bg, int duration_frames, fixed final_x, fixed final_y) :
        to_template_action(forward<BgPtr>(bg), duration_frames, fixed_point(final_x, final_y))
    {
    }

    template<class BgPtr>
    bg_move_to_action(BgPtr&& bg, int duration_frames, const fixed_point& final_position) :
        to_template_action(forward<BgPtr>(bg), duration_frames, final_position)
    {
    }

    [[nodiscard]] const bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class bg_move_loop_action : public loop_template_action<bg_ptr, fixed_point, bg_position_manager>
{

public:
    template<class BgPtr>
    bg_move_loop_action(BgPtr&& bg, int duration_frames, fixed final_x, fixed final_y) :
        loop_template_action(forward<BgPtr>(bg), duration_frames, fixed_point(final_x, final_y))
    {
    }

    template<class BgPtr>
    bg_move_loop_action(BgPtr&& bg, int duration_frames, const fixed_point& final_position) :
        loop_template_action(forward<BgPtr>(bg), duration_frames, final_position)
    {
    }

    [[nodiscard]] const bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};

}

#endif
