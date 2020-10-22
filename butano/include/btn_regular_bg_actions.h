/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_REGULAR_BG_ACTIONS_H
#define BTN_REGULAR_BG_ACTIONS_H

#include "btn_fixed_point.h"
#include "btn_regular_bg_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// visible

class regular_bg_visible_manager
{

public:
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.visible();
    }

    static void set(bool visible, regular_bg_ptr& bg)
    {
        bg.set_visible(visible);
    }
};


class regular_bg_visible_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_visible_manager>
{

public:
    regular_bg_visible_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    regular_bg_visible_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};


// position

class bg_position_manager
{

public:
    [[nodiscard]] static const fixed_point& get(const regular_bg_ptr& bg)
    {
        return bg.position();
    }

    static void set(const fixed_point& position, regular_bg_ptr& bg)
    {
        bg.set_position(position);
    }
};


class regular_bg_move_by_action :
        public by_value_template_action<regular_bg_ptr, fixed_point, bg_position_manager>
{

public:
    regular_bg_move_by_action(const regular_bg_ptr& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(bg, fixed_point(delta_x, delta_y))
    {
    }

    regular_bg_move_by_action(regular_bg_ptr&& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(bg), fixed_point(delta_x, delta_y))
    {
    }

    regular_bg_move_by_action(const regular_bg_ptr& bg, const fixed_point& delta_position) :
        by_value_template_action(bg, delta_position)
    {
    }

    regular_bg_move_by_action(regular_bg_ptr&& bg, const fixed_point& delta_position) :
        by_value_template_action(move(bg), delta_position)
    {
    }

    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


class regular_bg_move_to_action : public to_value_template_action<regular_bg_ptr, fixed_point, bg_position_manager>
{

public:
    regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(bg, duration_updates, final_position)
    {
    }

    regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class regular_bg_move_loop_action : public loop_value_template_action<regular_bg_ptr, fixed_point, bg_position_manager>
{

public:
    regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(bg, duration_updates, final_position)
    {
    }

    regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


class regular_bg_move_toggle_action :
        public toggle_value_template_action<regular_bg_ptr, fixed_point, bg_position_manager>
{

public:
    regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))
    {
    }

    regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))
    {
    }

    regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(bg, duration_updates, new_position)
    {
    }

    regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(move(bg), duration_updates, new_position)
    {
    }

    [[nodiscard]] const regular_bg_ptr& bg() const
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
