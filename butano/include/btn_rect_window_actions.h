#ifndef BTN_RECT_WINDOW_ACTIONS_H
#define BTN_RECT_WINDOW_ACTIONS_H

#include "btn_display.h"
#include "btn_fixed_point.h"
#include "btn_rect_window.h"
#include "btn_value_template_actions.h"

namespace btn
{

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


class rect_window_move_top_left_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    rect_window_move_top_left_to_action(rect_window rect_window, int duration_frames, fixed final_top,
                                        fixed final_left) :
        to_value_template_action(rect_window, duration_frames, fixed_point(final_left, final_top))
    {
        BTN_ASSERT(final_top >= 0 && final_top <= display::height(), "Invalid final top: ", final_top);
        BTN_ASSERT(final_left >= 0 && final_left <= display::width(), "Invalid final left: ", final_left);
    }

    rect_window_move_top_left_to_action(rect_window rect_window, int duration_frames,
                                        const fixed_point& final_top_left) :
        to_value_template_action(rect_window, duration_frames, final_top_left)
    {
        BTN_ASSERT(final_top_left.y() >= 0 && final_top_left.y() <= display::height(),
                   "Invalid final top: ", final_top_left.y());
        BTN_ASSERT(final_top_left.x() >= 0 && final_top_left.x() <= display::width(),
                   "Invalid final left: ", final_top_left.x());
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
        BTN_ASSERT(final_top >= 0 && final_top <= display::height(), "Invalid final top: ", final_top);
        BTN_ASSERT(final_left >= 0 && final_left <= display::width(), "Invalid final left: ", final_left);
    }

    rect_window_move_top_left_loop_action(rect_window rect_window, int duration_frames,
                                          const fixed_point& final_top_left) :
        loop_value_template_action(rect_window, duration_frames, final_top_left)
    {
        BTN_ASSERT(final_top_left.y() >= 0 && final_top_left.y() <= display::height(),
                   "Invalid final top: ", final_top_left.y());
        BTN_ASSERT(final_top_left.x() >= 0 && final_top_left.x() <= display::width(),
                   "Invalid final left: ", final_top_left.x());
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


class rect_window_move_bottom_right_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    rect_window_move_bottom_right_to_action(rect_window rect_window, int duration_frames, fixed final_bottom,
                                            fixed final_right) :
        to_value_template_action(rect_window, duration_frames, fixed_point(final_right, final_bottom))
    {
        BTN_ASSERT(final_bottom >= 0 && final_bottom <= display::height(), "Invalid final bottom: ", final_bottom);
        BTN_ASSERT(final_right >= 0 && final_right <= display::width(), "Invalid final right: ", final_right);
    }

    rect_window_move_bottom_right_to_action(rect_window rect_window, int duration_frames,
                                            const fixed_point& final_bottom_right) :
        to_value_template_action(rect_window, duration_frames, final_bottom_right)
    {
        BTN_ASSERT(final_bottom_right.y() >= 0 && final_bottom_right.y() <= display::height(),
                   "Invalid final bottom: ", final_bottom_right.y());
        BTN_ASSERT(final_bottom_right.x() >= 0 && final_bottom_right.x() <= display::width(),
                   "Invalid final right: ", final_bottom_right.x());
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
        BTN_ASSERT(final_bottom >= 0 && final_bottom <= display::height(), "Invalid final bottom: ", final_bottom);
        BTN_ASSERT(final_right >= 0 && final_right <= display::width(), "Invalid final right: ", final_right);
    }

    rect_window_move_bottom_right_loop_action(rect_window rect_window, int duration_frames,
                                              const fixed_point& final_bottom_right) :
        loop_value_template_action(rect_window, duration_frames, final_bottom_right)
    {
        BTN_ASSERT(final_bottom_right.y() >= 0 && final_bottom_right.y() <= display::height(),
                   "Invalid final bottom: ", final_bottom_right.y());
        BTN_ASSERT(final_bottom_right.x() >= 0 && final_bottom_right.x() <= display::width(),
                   "Invalid final right: ", final_bottom_right.x());
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

}

#endif
