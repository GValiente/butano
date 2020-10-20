/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_RECT_WINDOW_ACTIONS_H
#define BTN_RECT_WINDOW_ACTIONS_H

#include "btn_fixed_point.h"
#include "btn_rect_window.h"
#include "btn_value_template_actions.h"

namespace btn
{

// top

class rect_window_top_manager
{

public:
    [[nodiscard]] static fixed get(const rect_window& rect_window)
    {
        return rect_window.top();
    }

    static void set(fixed top, rect_window& rect_window)
    {
        rect_window.set_top(top);
    }
};


class rect_window_move_top_by_action : public by_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    rect_window_move_top_by_action(rect_window rect_window, fixed delta_top) :
        by_value_template_action(rect_window, delta_top)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_top() const
    {
        return delta_property();
    }
};


class rect_window_move_top_to_action : public to_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    rect_window_move_top_to_action(rect_window rect_window, int duration_frames, fixed final_top) :
        to_value_template_action(rect_window, duration_frames, final_top)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_top() const
    {
        return final_property();
    }
};


class rect_window_move_top_loop_action : public loop_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    rect_window_move_top_loop_action(rect_window rect_window, int duration_frames, fixed final_top) :
        loop_value_template_action(rect_window, duration_frames, final_top)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_top() const
    {
        return final_property();
    }
};


class rect_window_move_top_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    rect_window_move_top_toggle_action(rect_window rect_window, int duration_frames, fixed new_top) :
        toggle_value_template_action(rect_window, duration_frames, new_top)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_top() const
    {
        return new_property();
    }
};


// left

class rect_window_left_manager
{

public:
    [[nodiscard]] static fixed get(const rect_window& rect_window)
    {
        return rect_window.left();
    }

    static void set(fixed left, rect_window& rect_window)
    {
        rect_window.set_left(left);
    }
};


class rect_window_move_left_by_action : public by_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    rect_window_move_left_by_action(rect_window rect_window, fixed delta_left) :
        by_value_template_action(rect_window, delta_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_left() const
    {
        return delta_property();
    }
};


class rect_window_move_left_to_action : public to_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    rect_window_move_left_to_action(rect_window rect_window, int duration_frames, fixed final_left) :
        to_value_template_action(rect_window, duration_frames, final_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_left() const
    {
        return final_property();
    }
};


class rect_window_move_left_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    rect_window_move_left_loop_action(rect_window rect_window, int duration_frames, fixed final_left) :
        loop_value_template_action(rect_window, duration_frames, final_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_left() const
    {
        return final_property();
    }
};


class rect_window_move_left_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    rect_window_move_left_toggle_action(rect_window rect_window, int duration_frames, fixed new_left) :
        toggle_value_template_action(rect_window, duration_frames, new_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_left() const
    {
        return new_property();
    }
};


// bottom

class rect_window_bottom_manager
{

public:
    [[nodiscard]] static fixed get(const rect_window& rect_window)
    {
        return rect_window.bottom();
    }

    static void set(fixed bottom, rect_window& rect_window)
    {
        rect_window.set_bottom(bottom);
    }
};


class rect_window_move_bottom_by_action :
        public by_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    rect_window_move_bottom_by_action(rect_window rect_window, fixed delta_bottom) :
        by_value_template_action(rect_window, delta_bottom)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_bottom() const
    {
        return delta_property();
    }
};


class rect_window_move_bottom_to_action :
        public to_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    rect_window_move_bottom_to_action(rect_window rect_window, int duration_frames, fixed final_bottom) :
        to_value_template_action(rect_window, duration_frames, final_bottom)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_bottom() const
    {
        return final_property();
    }
};


class rect_window_move_bottom_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    rect_window_move_bottom_loop_action(rect_window rect_window, int duration_frames, fixed final_bottom) :
        loop_value_template_action(rect_window, duration_frames, final_bottom)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_bottom() const
    {
        return final_property();
    }
};


class rect_window_move_bottom_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    rect_window_move_bottom_toggle_action(rect_window rect_window, int duration_frames, fixed new_bottom) :
        toggle_value_template_action(rect_window, duration_frames, new_bottom)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_bottom() const
    {
        return new_property();
    }
};


// right

class rect_window_right_manager
{

public:
    [[nodiscard]] static fixed get(const rect_window& rect_window)
    {
        return rect_window.right();
    }

    static void set(fixed right, rect_window& rect_window)
    {
        rect_window.set_right(right);
    }
};


class rect_window_move_right_by_action :
        public by_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    rect_window_move_right_by_action(rect_window rect_window, fixed delta_right) :
        by_value_template_action(rect_window, delta_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_right() const
    {
        return delta_property();
    }
};


class rect_window_move_right_to_action :
        public to_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    rect_window_move_right_to_action(rect_window rect_window, int duration_frames, fixed final_right) :
        to_value_template_action(rect_window, duration_frames, final_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_right() const
    {
        return final_property();
    }
};


class rect_window_move_right_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    rect_window_move_right_loop_action(rect_window rect_window, int duration_frames, fixed final_right) :
        loop_value_template_action(rect_window, duration_frames, final_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_right() const
    {
        return final_property();
    }
};


class rect_window_move_right_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    rect_window_move_right_toggle_action(rect_window rect_window, int duration_frames, fixed new_right) :
        toggle_value_template_action(rect_window, duration_frames, new_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_right() const
    {
        return new_property();
    }
};


// top_left

class rect_window_top_left_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const rect_window& rect_window)
    {
        return rect_window.top_left();
    }

    static void set(const fixed_point& top_left, rect_window& rect_window)
    {
        rect_window.set_top_left(top_left);
    }
};


class rect_window_move_top_left_by_action :
        public by_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    rect_window_move_top_left_by_action(rect_window rect_window, fixed delta_top, fixed delta_left) :
        by_value_template_action(rect_window, fixed_point(delta_left, delta_top))
    {
    }

    rect_window_move_top_left_by_action(rect_window rect_window, const fixed_point& delta_top_left) :
        by_value_template_action(rect_window, delta_top_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_top() const
    {
        return delta_property().y();
    }

    [[nodiscard]] fixed delta_left() const
    {
        return delta_property().x();
    }

    [[nodiscard]] const fixed_point& delta_top_left() const
    {
        return delta_property();
    }
};


class rect_window_move_top_left_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    rect_window_move_top_left_to_action(rect_window rect_window, int duration_frames, fixed final_top,
                                        fixed final_left) :
        to_value_template_action(rect_window, duration_frames, fixed_point(final_left, final_top))
    {
    }

    rect_window_move_top_left_to_action(rect_window rect_window, int duration_frames,
                                        const fixed_point& final_top_left) :
        to_value_template_action(rect_window, duration_frames, final_top_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_top() const
    {
        return final_property().y();
    }

    [[nodiscard]] fixed final_left() const
    {
        return final_property().x();
    }

    [[nodiscard]] const fixed_point& final_top_left() const
    {
        return final_property();
    }
};


class rect_window_move_top_left_loop_action :
        public loop_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    rect_window_move_top_left_loop_action(rect_window rect_window, int duration_frames, fixed final_top,
                                          fixed final_left) :
        loop_value_template_action(rect_window, duration_frames, fixed_point(final_left, final_top))
    {
    }

    rect_window_move_top_left_loop_action(rect_window rect_window, int duration_frames,
                                          const fixed_point& final_top_left) :
        loop_value_template_action(rect_window, duration_frames, final_top_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_top() const
    {
        return final_property().y();
    }

    [[nodiscard]] fixed final_left() const
    {
        return final_property().x();
    }

    [[nodiscard]] const fixed_point& final_top_left() const
    {
        return final_property();
    }
};


class rect_window_move_top_left_toggle_action :
        public toggle_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    rect_window_move_top_left_toggle_action(rect_window rect_window, int duration_frames, fixed new_top,
                                            fixed new_left) :
        toggle_value_template_action(rect_window, duration_frames, fixed_point(new_left, new_top))
    {
    }

    rect_window_move_top_left_toggle_action(rect_window rect_window, int duration_frames,
                                            const fixed_point& new_top_left) :
        toggle_value_template_action(rect_window, duration_frames, new_top_left)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_top() const
    {
        return new_property().y();
    }

    [[nodiscard]] fixed new_left() const
    {
        return new_property().x();
    }

    [[nodiscard]] const fixed_point& new_top_left() const
    {
        return new_property();
    }
};


// bottom_right

class rect_window_bottom_right_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const rect_window& rect_window)
    {
        return rect_window.bottom_right();
    }

    static void set(const fixed_point& bottom_right, rect_window& rect_window)
    {
        rect_window.set_bottom_right(bottom_right);
    }
};


class rect_window_move_bottom_right_by_action :
        public by_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    rect_window_move_bottom_right_by_action(rect_window rect_window, fixed delta_bottom, fixed delta_right) :
        by_value_template_action(rect_window, fixed_point(delta_right, delta_bottom))
    {
    }

    rect_window_move_bottom_right_by_action(rect_window rect_window, const fixed_point& delta_bottom_right) :
        by_value_template_action(rect_window, delta_bottom_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed delta_bottom() const
    {
        return delta_property().y();
    }

    [[nodiscard]] fixed delta_right() const
    {
        return delta_property().x();
    }

    [[nodiscard]] const fixed_point& delta_bottom_right() const
    {
        return delta_property();
    }
};


class rect_window_move_bottom_right_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    rect_window_move_bottom_right_to_action(rect_window rect_window, int duration_frames, fixed final_bottom,
                                            fixed final_right) :
        to_value_template_action(rect_window, duration_frames, fixed_point(final_right, final_bottom))
    {
    }

    rect_window_move_bottom_right_to_action(rect_window rect_window, int duration_frames,
                                            const fixed_point& final_bottom_right) :
        to_value_template_action(rect_window, duration_frames, final_bottom_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_bottom() const
    {
        return final_property().y();
    }

    [[nodiscard]] fixed final_right() const
    {
        return final_property().x();
    }

    [[nodiscard]] const fixed_point& final_bottom_right() const
    {
        return final_property();
    }
};


class rect_window_move_bottom_right_loop_action :
        public loop_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    rect_window_move_bottom_right_loop_action(rect_window rect_window, int duration_frames, fixed final_bottom,
                                              fixed final_right) :
        loop_value_template_action(rect_window, duration_frames, fixed_point(final_right, final_bottom))
    {
    }

    rect_window_move_bottom_right_loop_action(rect_window rect_window, int duration_frames,
                                              const fixed_point& final_bottom_right) :
        loop_value_template_action(rect_window, duration_frames, final_bottom_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed final_bottom() const
    {
        return final_property().y();
    }

    [[nodiscard]] fixed final_right() const
    {
        return final_property().x();
    }

    [[nodiscard]] const fixed_point& final_bottom_right() const
    {
        return final_property();
    }
};


class rect_window_move_bottom_right_toggle_action :
        public toggle_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    rect_window_move_bottom_right_toggle_action(rect_window rect_window, int duration_frames, fixed new_bottom,
                                                fixed new_right) :
        toggle_value_template_action(rect_window, duration_frames, fixed_point(new_right, new_bottom))
    {
    }

    rect_window_move_bottom_right_toggle_action(rect_window rect_window, int duration_frames,
                                                const fixed_point& new_bottom_right) :
        toggle_value_template_action(rect_window, duration_frames, new_bottom_right)
    {
    }

    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    [[nodiscard]] fixed new_bottom() const
    {
        return new_property().y();
    }

    [[nodiscard]] fixed new_right() const
    {
        return new_property().x();
    }

    [[nodiscard]] const fixed_point& new_bottom_right() const
    {
        return new_property();
    }
};

}

#endif
