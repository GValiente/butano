/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_INSIDE_WINDOW_ACTIONS_H
#define BN_INSIDE_WINDOW_ACTIONS_H

/**
 * @file
 * bn::inside_window actions header file.
 *
 * @ingroup inside_window
 * @ingroup action
 */

#include "bn_inside_window.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if a inside_window must be committed to the GBA or not.
 *
 * @ingroup inside_window
 * @ingroup action
 */
class inside_window_visible_manager
{

public:
    /**
     * @brief Indicates if the given inside_window is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(inside_window window)
    {
        return window.visible();
    }

    /**
     * @brief Sets if the given inside_window must be committed to the GBA or not.
     */
    static void set(bool visible, inside_window window)
    {
        window.set_visible(visible);
    }
};


/**
 * @brief Toggles if a inside_window must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup inside_window
 * @ingroup action
 */
class inside_window_visible_toggle_action :
        public bool_toggle_value_template_action<inside_window, inside_window_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param window inside_window to modify.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given inside_window must be committed to the GBA or not.
     */
    inside_window_visible_toggle_action(inside_window window, int duration_updates) :
        bool_toggle_value_template_action(window, duration_updates)
    {
    }

    /**
     * @brief Returns the inside_window to modify.
     */
    [[nodiscard]] inside_window window() const
    {
        return value();
    }
};

}

#endif
