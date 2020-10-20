/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_INSIDE_WINDOW_ACTIONS_H
#define BTN_INSIDE_WINDOW_ACTIONS_H

#include "btn_inside_window.h"
#include "btn_value_template_actions.h"

namespace btn
{

// visible

class inside_window_visible_manager
{

public:
    [[nodiscard]] static bool get(const inside_window& inside_window)
    {
        return inside_window.visible();
    }

    static void set(bool visible, inside_window& inside_window)
    {
        inside_window.set_visible(visible);
    }
};


class inside_window_visible_toggle_action :
        public bool_toggle_value_template_action<inside_window, inside_window_visible_manager>
{

public:
    inside_window_visible_toggle_action(const inside_window& inside_window, int duration_frames) :
        bool_toggle_value_template_action(inside_window, duration_frames)
    {
    }

    [[nodiscard]] const inside_window& inside_window() const
    {
        return value();
    }
};

}

#endif
