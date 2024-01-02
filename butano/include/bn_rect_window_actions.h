/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECT_WINDOW_ACTIONS_H
#define BN_RECT_WINDOW_ACTIONS_H

/**
 * @file
 * bn::rect_window actions header file.
 *
 * @ingroup rect_window
 * @ingroup action
 */

#include "bn_rect_window.h"
#include "bn_value_template_actions.h"

namespace bn
{

// top

/**
 * @brief Manages the position of the top boundary of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_top_manager
{

public:
    /**
     * @brief Returns the position of the top boundary the given rect_window.
     */
    [[nodiscard]] static fixed get(rect_window window)
    {
        return window.top();
    }

    /**
     * @brief Sets the position of the top boundary the given rect_window.
     */
    static void set(fixed top, rect_window window)
    {
        window.set_top(top);
    }
};


/**
 * @brief Modifies the position of the top boundary of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_by_action : public by_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_top How much to add to the position of the top boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_top_by_action(rect_window window, fixed delta_top) :
        by_value_template_action(window, delta_top)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the top boundary of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_top() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the top boundary of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_to_action : public to_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the top boundary of the given rect_window is equal to final_top.
     * @param final_top Position of the top boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_to_action(rect_window window, int duration_updates, fixed final_top) :
        to_value_template_action(window, duration_updates, final_top)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the top boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_top() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the top boundary of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_top When the position of the top boundary of the given rect_window is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    rect_window_move_top_loop_action(rect_window window, int duration_updates, fixed final_top) :
        loop_value_template_action(window, duration_updates, final_top)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the top boundary of the given rect_window is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_top() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the top boundary of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the top boundary of the given rect_window.
     * @param new_top New position of the top boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_toggle_action(rect_window window, int duration_updates, fixed new_top) :
        toggle_value_template_action(window, duration_updates, new_top)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the top boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_top() const
    {
        return new_property();
    }
};


// left

/**
 * @brief Manages the position of the left boundary of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_left_manager
{

public:
    /**
     * @brief Returns the position of the left boundary the given rect_window.
     */
    [[nodiscard]] static fixed get(rect_window window)
    {
        return window.left();
    }

    /**
     * @brief Sets the position of the left boundary the given rect_window.
     */
    static void set(fixed left, rect_window window)
    {
        window.set_left(left);
    }
};


/**
 * @brief Modifies the position of the left boundary of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_left_by_action : public by_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_left How much to add to the position of the left boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_left_by_action(rect_window window, fixed delta_left) :
        by_value_template_action(window, delta_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the left boundary of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_left() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the left boundary of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_left_to_action : public to_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the left boundary of the given rect_window is equal to final_left.
     * @param final_left Position of the left boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_left_to_action(rect_window window, int duration_updates, fixed final_left) :
        to_value_template_action(window, duration_updates, final_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the left boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_left() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the left boundary of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_left_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_left When the position of the left boundary of the given rect_window is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    rect_window_move_left_loop_action(rect_window window, int duration_updates, fixed final_left) :
        loop_value_template_action(window, duration_updates, final_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the left boundary of the given rect_window is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_left() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the left boundary of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_left_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the left boundary of the given rect_window.
     * @param new_left New position of the left boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_left_toggle_action(rect_window window, int duration_updates, fixed new_left) :
        toggle_value_template_action(window, duration_updates, new_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the left boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_left() const
    {
        return new_property();
    }
};


// bottom

/**
 * @brief Manages the position of the bottom boundary of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_bottom_manager
{

public:
    /**
     * @brief Returns the position of the bottom boundary the given rect_window.
     */
    [[nodiscard]] static fixed get(rect_window window)
    {
        return window.bottom();
    }

    /**
     * @brief Sets the position of the bottom boundary the given rect_window.
     */
    static void set(fixed bottom, rect_window window)
    {
        window.set_bottom(bottom);
    }
};


/**
 * @brief Modifies the position of the bottom boundary of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_by_action :
        public by_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_bottom How much to add to the position of the bottom boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_bottom_by_action(rect_window window, fixed delta_bottom) :
        by_value_template_action(window, delta_bottom)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the bottom boundary of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_bottom() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the bottom boundary of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_to_action :
        public to_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the bottom boundary of the given rect_window is equal to final_bottom.
     * @param final_bottom Position of the bottom boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_to_action(rect_window window, int duration_updates, fixed final_bottom) :
        to_value_template_action(window, duration_updates, final_bottom)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the bottom boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_bottom() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the bottom boundary of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_bottom When the position of the bottom boundary of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    rect_window_move_bottom_loop_action(rect_window window, int duration_updates, fixed final_bottom) :
        loop_value_template_action(window, duration_updates, final_bottom)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the bottom boundary of the given rect_window is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_bottom() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the bottom boundary of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the bottom boundary of the given rect_window.
     * @param new_bottom New position of the bottom boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_toggle_action(rect_window window, int duration_updates, fixed new_bottom) :
        toggle_value_template_action(window, duration_updates, new_bottom)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the bottom boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_bottom() const
    {
        return new_property();
    }
};


// right

/**
 * @brief Manages the position of the right boundary of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_right_manager
{

public:
    /**
     * @brief Returns the position of the right boundary the given rect_window.
     */
    [[nodiscard]] static fixed get(rect_window window)
    {
        return window.right();
    }

    /**
     * @brief Sets the position of the right boundary the given rect_window.
     */
    static void set(fixed right, rect_window window)
    {
        window.set_right(right);
    }
};


/**
 * @brief Modifies the position of the right boundary of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_right_by_action :
        public by_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_right How much to add to the position of the right boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_right_by_action(rect_window window, fixed delta_right) :
        by_value_template_action(window, delta_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the right boundary of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_right() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the right boundary of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_right_to_action :
        public to_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the right boundary of the given rect_window is equal to final_right.
     * @param final_right Position of the right boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_right_to_action(rect_window window, int duration_updates, fixed final_right) :
        to_value_template_action(window, duration_updates, final_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the right boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_right() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the right boundary of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_right_loop_action :
        public loop_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_right When the position of the right boundary of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    rect_window_move_right_loop_action(rect_window window, int duration_updates, fixed final_right) :
        loop_value_template_action(window, duration_updates, final_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the right boundary of the given rect_window is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_right() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the right boundary of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_right_toggle_action :
        public toggle_value_template_action<rect_window, fixed, rect_window_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the right boundary of the given rect_window.
     * @param new_right New position of the right boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_right_toggle_action(rect_window window, int duration_updates, fixed new_right) :
        toggle_value_template_action(window, duration_updates, new_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the right boundary of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_right() const
    {
        return new_property();
    }
};


// top_left

/**
 * @brief Manages the position of the top-left corner of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_top_left_manager
{

public:
    /**
     * @brief Returns the position of the top-left corner the given rect_window.
     */
    [[nodiscard]] static const fixed_point& get(rect_window window)
    {
        return window.top_left();
    }

    /**
     * @brief Sets the position of the top-left corner the given rect_window.
     */
    static void set(const fixed_point& top_left, rect_window window)
    {
        window.set_top_left(top_left);
    }
};


/**
 * @brief Modifies the position of the top-left corner of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_left_by_action :
        public by_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_top How much to add to the position of the top boundary of the given rect_window
     * when the action is updated.
     * @param delta_left How much to add to the position of the left boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_top_left_by_action(rect_window window, fixed delta_top, fixed delta_left) :
        by_value_template_action(window, fixed_point(delta_left, delta_top))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_top_left How much to add to the position of the top-left corner of the given rect_window
     * when the action is updated.
     */
    rect_window_move_top_left_by_action(rect_window window, const fixed_point& delta_top_left) :
        by_value_template_action(window, delta_top_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the top-left corner of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_top_left() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the top-left corner of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_left_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the top-left corner of the given rect_window is equal to (final_top, final_left).
     * @param final_top Position of the top boundary of the given rect_window
     * when the action is updated duration_updates times.
     * @param final_left Position of the left boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_left_to_action(rect_window window, int duration_updates, fixed final_top, fixed final_left) :
        to_value_template_action(window, duration_updates, fixed_point(final_left, final_top))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the top-left corner of the given rect_window is equal to final_top_left.
     * @param final_top_left Position of the top-left corner of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_left_to_action(rect_window window, int duration_updates, const fixed_point& final_top_left) :
        to_value_template_action(window, duration_updates, final_top_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the top-left corner of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_top_left() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the top-left corner of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_left_loop_action :
        public loop_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_top When the position of the top boundary of the given rect_window is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_left When the position of the left boundary of the given rect_window is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    rect_window_move_top_left_loop_action(rect_window window, int duration_updates, fixed final_top,
                                          fixed final_left) :
        loop_value_template_action(window, duration_updates, fixed_point(final_left, final_top))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_top_left When the position of the top-left corner of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    rect_window_move_top_left_loop_action(rect_window window, int duration_updates,
                                          const fixed_point& final_top_left) :
        loop_value_template_action(window, duration_updates, final_top_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the top-left corner of the given rect_window is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_top_left() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the top-left corner of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_top_left_toggle_action :
        public toggle_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the top-left corner of the given rect_window.
     * @param new_top New position of the top boundary of the given rect_window
     * when the action is updated duration_updates times.
     * @param new_left New position of the left boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_left_toggle_action(rect_window window, int duration_updates, fixed new_top, fixed new_left) :
        toggle_value_template_action(window, duration_updates, fixed_point(new_left, new_top))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the top-left corner of the given rect_window.
     * @param new_top_left New position of the top-left corner of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_top_left_toggle_action(rect_window window, int duration_updates,
                                            const fixed_point& new_top_left) :
        toggle_value_template_action(window, duration_updates, new_top_left)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the top-left corner of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_top_left() const
    {
        return new_property();
    }
};


// bottom_right

/**
 * @brief Manages the position of the bottom-right corner of a rect_window.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_bottom_right_manager
{

public:
    /**
     * @brief Returns the position of the bottom-right corner the given rect_window.
     */
    [[nodiscard]] static const fixed_point& get(rect_window window)
    {
        return window.bottom_right();
    }

    /**
     * @brief Sets the position of the bottom-right corner the given rect_window.
     */
    static void set(const fixed_point& bottom_right, rect_window window)
    {
        window.set_bottom_right(bottom_right);
    }
};


/**
 * @brief Modifies the position of the bottom-right corner of a rect_window by a given delta.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_right_by_action :
        public by_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_bottom How much to add to the position of the bottom boundary of the given rect_window
     * when the action is updated.
     * @param delta_right How much to add to the position of the right boundary of the given rect_window
     * when the action is updated.
     */
    rect_window_move_bottom_right_by_action(rect_window window, fixed delta_bottom, fixed delta_right) :
        by_value_template_action(window, fixed_point(delta_right, delta_bottom))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param delta_bottom_right How much to add to the position of the bottom-right corner of the given rect_window
     * when the action is updated.
     */
    rect_window_move_bottom_right_by_action(rect_window window, const fixed_point& delta_bottom_right) :
        by_value_template_action(window, delta_bottom_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the bottom-right corner of the given rect_window
     * when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_bottom_right() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the bottom-right corner of a rect_window until it has a given state.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_right_to_action :
        public to_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the bottom-right corner of the given rect_window is equal to (final_bottom, final_right).
     * @param final_bottom Position of the bottom boundary of the given rect_window
     * when the action is updated duration_updates times.
     * @param final_right Position of the right boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_right_to_action(rect_window window, int duration_updates, fixed final_bottom,
                                            fixed final_right) :
        to_value_template_action(window, duration_updates, fixed_point(final_right, final_bottom))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the bottom-right corner of the given rect_window is equal to final_bottom_right.
     * @param final_bottom_right Position of the bottom-right corner of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_right_to_action(rect_window window, int duration_updates,
                                            const fixed_point& final_bottom_right) :
        to_value_template_action(window, duration_updates, final_bottom_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the bottom-right corner of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_bottom_right() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the bottom-right corner of a rect_window from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_right_loop_action :
        public loop_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_bottom When the position of the bottom boundary of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_right When the position of the right boundary of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    rect_window_move_bottom_right_loop_action(rect_window window, int duration_updates, fixed final_bottom,
                                              fixed final_right) :
        loop_value_template_action(window, duration_updates, fixed_point(final_right, final_bottom))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_bottom_right When the position of the bottom-right corner of the given rect_window
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    rect_window_move_bottom_right_loop_action(rect_window window, int duration_updates,
                                              const fixed_point& final_bottom_right) :
        loop_value_template_action(window, duration_updates, final_bottom_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief When the position of the bottom-right corner of the given rect_window
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_bottom_right() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the bottom-right corner of a rect_window
 * when the action is updated a given number of times.
 *
 * @ingroup rect_window
 * @ingroup action
 */
class rect_window_move_bottom_right_toggle_action :
        public toggle_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the bottom-right corner of the given rect_window.
     * @param new_bottom New position of the bottom boundary of the given rect_window
     * when the action is updated duration_updates times.
     * @param new_right New position of the right boundary of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_right_toggle_action(rect_window window, int duration_updates, fixed new_bottom,
                                                fixed new_right) :
        toggle_value_template_action(window, duration_updates, fixed_point(new_right, new_bottom))
    {
    }

    /**
     * @brief Constructor.
     * @param window rect_window to modify.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the bottom-right corner of the given rect_window.
     * @param new_bottom_right New position of the bottom-right corner of the given rect_window
     * when the action is updated duration_updates times.
     */
    rect_window_move_bottom_right_toggle_action(rect_window window, int duration_updates,
                                                const fixed_point& new_bottom_right) :
        toggle_value_template_action(window, duration_updates, new_bottom_right)
    {
    }

    /**
     * @brief Returns the rect_window to modify.
     */
    [[nodiscard]] rect_window window() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the bottom-right corner of the given rect_window
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_bottom_right() const
    {
        return new_property();
    }
};

}

#endif
