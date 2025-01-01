/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_ACTIONS_H
#define BN_AFFINE_BG_ACTIONS_H

/**
 * @file
 * bn::affine_bg_ptr actions header file.
 *
 * @ingroup affine_bg
 * @ingroup action
 */

#include "bn_affine_bg_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if an affine_bg_ptr must be committed to the GBA or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_visible_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.visible();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must be committed to the GBA or not.
     */
    static void set(bool visible, affine_bg_ptr& bg)
    {
        bg.set_visible(visible);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_visible_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be committed to the GBA or not.
     */
    affine_bg_visible_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be committed to the GBA or not.
     */
    affine_bg_visible_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


// position

/**
 * @brief Manages the position of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_position_manager
{

public:
    /**
     * @brief Returns the position of the given affine_bg_ptr.
     */
    [[nodiscard]] static const fixed_point& get(const affine_bg_ptr& bg)
    {
        return bg.position();
    }

    /**
     * @brief Sets the position of the given affine_bg_ptr.
     */
    static void set(const fixed_point& position, affine_bg_ptr& bg)
    {
        bg.set_position(position);
    }
};


/**
 * @brief Modifies the position of an affine_bg_ptr by a given delta.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_by_action :
        public by_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param delta_x How much to add to the horizontal position of the given affine_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_by_action(const affine_bg_ptr& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(bg, fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param delta_x How much to add to the horizontal position of the given affine_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_by_action(affine_bg_ptr&& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(bg), fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param delta_position How much to add to the position of the given affine_bg_ptr when the action is updated.
     */
    affine_bg_move_by_action(const affine_bg_ptr& bg, const fixed_point& delta_position) :
        by_value_template_action(bg, delta_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param delta_position How much to add to the position of the given affine_bg_ptr when the action is updated.
     */
    affine_bg_move_by_action(affine_bg_ptr&& bg, const fixed_point& delta_position) :
        by_value_template_action(move(bg), delta_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the given affine_bg_ptr when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_to_action :
        public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given affine_bg_ptr is equal to final_position.
     * @param final_position Position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_to_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given affine_bg_ptr is equal to final_position.
     * @param final_position Position of the given affine_bg_ptr when the action is updated duration_updates times.
     */
    affine_bg_move_to_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given affine_bg_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of an affine_bg_ptr from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_loop_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_loop_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the position of the given affine_bg_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of an affine_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the given affine_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    affine_bg_move_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the given affine_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    affine_bg_move_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the given affine_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    affine_bg_move_toggle_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(bg, duration_updates, new_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the given affine_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    affine_bg_move_toggle_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(move(bg), duration_updates, new_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given affine_bg_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};


// top_left_position

/**
 * @brief Manages the top-left position of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_top_left_position_manager
{

public:
    /**
     * @brief Returns the top-left position of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed_point get(const affine_bg_ptr& bg)
    {
        return bg.top_left_position();
    }

    /**
     * @brief Sets the top-left position of the given affine_bg_ptr.
     */
    static void set(const fixed_point& top_left_position, affine_bg_ptr& bg)
    {
        bg.set_top_left_position(top_left_position);
    }
};


/**
 * @brief Modifies the top-left position of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_top_left_move_to_action :
        public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_top_left_x Horizontal top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_top_left_y Vertical top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_to_action(
            const affine_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_top_left_x Horizontal top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_top_left_y Vertical top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_to_action(
            affine_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given affine_bg_ptr is equal to final_top_left_position.
     * @param final_top_left_position Top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_to_action(
            const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) :
        to_value_template_action(bg, duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given affine_bg_ptr is equal to final_top_left_position.
     * @param final_top_left_position Top-left position of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_to_action(
            affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) :
        to_value_template_action(move(bg), duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the top-left position of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_top_left_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the top-left position of an affine_bg_ptr from a minimum to a maximum.
 * When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_top_left_move_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_x When the horizontal top-left position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_top_left_y When the vertical top-left position of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_top_left_move_loop_action(
            const affine_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_x When the horizontal top-left position of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_top_left_y When the vertical top-left position of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_top_left_move_loop_action(
            affine_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_position When the top-left position of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_top_left_move_loop_action(
            const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) :
        loop_value_template_action(bg, duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_position When the top-left position of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_top_left_move_loop_action(
        affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) :
            loop_value_template_action(move(bg), duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the top-left position of the given affine_bg_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_top_left_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the top-left position of an affine_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_top_left_move_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the top-left position of the given affine_bg_ptr.
     * @param new_top_left_x New horizontal top-left position when the action is updated duration_updates times.
     * @param new_top_left_y New vertical top-left position when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_toggle_action(
            const affine_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) :
        toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the top-left position of the given affine_bg_ptr.
     * @param new_top_left_x New horizontal top-left position when the action is updated duration_updates times.
     * @param new_top_left_y New vertical top-left position when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_toggle_action(
            affine_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) :
        toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the top-left position of the given affine_bg_ptr.
     * @param new_top_left_position New top-left position when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_toggle_action(
            const affine_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) :
        toggle_value_template_action(bg, duration_updates, new_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the top-left position of the given affine_bg_ptr.
     * @param new_top_left_position New top-left position when the action is updated duration_updates times.
     */
    affine_bg_top_left_move_toggle_action(
            affine_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) :
        toggle_value_template_action(move(bg), duration_updates, new_top_left_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the top-left position of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_top_left_position() const
    {
        return new_property();
    }
};


// rotation

/**
 * @brief Manages the rotation angle of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_rotation_manager
{

public:
    /**
     * @brief Returns the rotation angle of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.rotation_angle();
    }

    /**
     * @brief Sets the rotation angle of the given affine_bg_ptr.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @param bg affine_bg_ptr to modify.
     */
    static void set(fixed rotation_angle, affine_bg_ptr& bg)
    {
        bg.set_rotation_angle(rotation_angle);
    }
};


/**
 * @brief Modifies the rotation angle of an affine_bg_ptr by delta_rotation_angle.
 * When the rotation angle is over 360, it goes back to 0 and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_rotate_by_action :
        public cyclic_by_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param delta_rotation_angle How many degrees to add to the rotation angle of the given affine_bg_ptr
     * when the action is updated.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_by_action(const affine_bg_ptr& bg, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(bg, delta_rotation_angle, 0, 360)
    {
        BN_ASSERT(delta_rotation_angle > -360 && delta_rotation_angle < 360,
                  "Invalid delta rotation angle: ", delta_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param delta_rotation_angle How many degrees to add to the rotation angle of the given affine_bg_ptr
     * when the action is updated.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_by_action(affine_bg_ptr&& bg, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(move(bg), delta_rotation_angle, 0, 360)
    {
        BN_ASSERT(delta_rotation_angle > -360 && delta_rotation_angle < 360,
                  "Invalid delta rotation angle: ", delta_rotation_angle);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns how many degrees to add to the rotation angle of the given affine_bg_ptr
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_rotation_angle() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the rotation angle of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_rotate_to_action :
        public to_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the rotation angle of the given affine_bg_ptr is equal to final_rotation_angle.
     * @param final_rotation_angle Rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) :
        to_value_template_action(bg, duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the rotation angle of the given affine_bg_ptr is equal to final_rotation_angle.
     * @param final_rotation_angle Rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) :
        to_value_template_action(move(bg), duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the rotation angle of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the rotation angle of an affine_bg_ptr from a minimum to a maximum.
 * When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_rotate_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the rotation angle delta.
     * @param final_rotation_angle When the rotation angle of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) :
        loop_value_template_action(bg, duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the rotation angle delta.
     * @param final_rotation_angle When the rotation angle of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) :
        loop_value_template_action(move(bg), duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the rotation angle of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the rotation angle of an affine_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_rotate_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the rotation angle
     * of the given affine_bg_ptr.
     * @param new_rotation_angle New rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_rotation_angle) :
        toggle_value_template_action(bg, duration_updates, new_rotation_angle)
    {
        BN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the rotation angle
     * of the given affine_bg_ptr.
     * @param new_rotation_angle New rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    affine_bg_rotate_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_rotation_angle) :
        toggle_value_template_action(move(bg), duration_updates, new_rotation_angle)
    {
        BN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the rotation angle of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_rotation_angle() const
    {
        return new_property();
    }
};


// horizontal_scale

/**
 * @brief Manages the horizontal scale of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_scale_manager
{

public:
    /**
     * @brief Returns the horizontal scale of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.horizontal_scale();
    }

    /**
     * @brief Sets the horizontal scale of the given affine_bg_ptr.
     */
    static void set(fixed horizontal_scale, affine_bg_ptr& bg)
    {
        bg.set_horizontal_scale(horizontal_scale);
    }
};


/**
 * @brief Modifies the horizontal scale of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_scale_to_action :
        public to_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal scale of the given affine_bg_ptr is equal to final_horizontal_scale.
     * @param final_horizontal_scale Horizontal scale when the action is updated duration_updates times.
     */
    affine_bg_horizontal_scale_to_action(const affine_bg_ptr& bg, int duration_updates,
                                         fixed final_horizontal_scale) :
        to_value_template_action(bg, duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal scale of the given affine_bg_ptr is equal to final_horizontal_scale.
     * @param final_horizontal_scale Horizontal scale when the action is updated duration_updates times.
     */
    affine_bg_horizontal_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) :
        to_value_template_action(move(bg), duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal scale of an affine_bg_ptr from a minimum to a maximum.
 * When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_scale_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the horizontal scale delta.
     * @param final_horizontal_scale When the horizontal scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_horizontal_scale_loop_action(const affine_bg_ptr& bg, int duration_updates,
                                           fixed final_horizontal_scale) :
        loop_value_template_action(bg, duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the horizontal scale delta.
     * @param final_horizontal_scale When the horizontal scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_horizontal_scale_loop_action(affine_bg_ptr&& bg, int duration_updates,
                                           fixed final_horizontal_scale) :
        loop_value_template_action(move(bg), duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the horizontal scale of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the horizontal scale of an affine_bg_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_scale_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the horizontal scale
     * of the given affine_bg_ptr.
     * @param new_horizontal_scale New horizontal scale when the action is updated duration_updates times.
     */
    affine_bg_horizontal_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates,
                                             fixed new_horizontal_scale) :
        toggle_value_template_action(bg, duration_updates, new_horizontal_scale)
    {
        BN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the horizontal scale
     * of the given affine_bg_ptr.
     * @param new_horizontal_scale New horizontal scale when the action is updated duration_updates times.
     */
    affine_bg_horizontal_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates,
                                             fixed new_horizontal_scale) :
        toggle_value_template_action(move(bg), duration_updates, new_horizontal_scale)
    {
        BN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_horizontal_scale() const
    {
        return new_property();
    }
};


// vertical_scale

/**
 * @brief Manages the vertical scale of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_scale_manager
{

public:
    /**
     * @brief Returns the vertical scale of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.vertical_scale();
    }

    /**
     * @brief Sets the vertical scale of the given affine_bg_ptr.
     */
    static void set(fixed vertical_scale, affine_bg_ptr& bg)
    {
        bg.set_vertical_scale(vertical_scale);
    }
};


/**
 * @brief Modifies the vertical scale of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_scale_to_action :
        public to_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical scale of the given affine_bg_ptr is equal to final_vertical_scale.
     * @param final_vertical_scale Vertical scale when the action is updated duration_updates times.
     */
    affine_bg_vertical_scale_to_action(const affine_bg_ptr& bg, int duration_updates,
                                       fixed final_vertical_scale) :
        to_value_template_action(bg, duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical scale of the given affine_bg_ptr is equal to final_vertical_scale.
     * @param final_vertical_scale Vertical scale when the action is updated duration_updates times.
     */
    affine_bg_vertical_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) :
        to_value_template_action(move(bg), duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical scale of an affine_bg_ptr from a minimum to a maximum.
 * When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_scale_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the vertical scale delta.
     * @param final_vertical_scale When the vertical scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_vertical_scale_loop_action(const affine_bg_ptr& bg, int duration_updates,
                                         fixed final_vertical_scale) :
        loop_value_template_action(bg, duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the vertical scale delta.
     * @param final_vertical_scale When the vertical scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_vertical_scale_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) :
        loop_value_template_action(move(bg), duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the vertical scale of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the vertical scale of an affine_bg_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_scale_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the vertical scale
     * of the given affine_bg_ptr.
     * @param new_vertical_scale New vertical scale when the action is updated duration_updates times.
     */
    affine_bg_vertical_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates,
                                           fixed new_vertical_scale) :
        toggle_value_template_action(bg, duration_updates, new_vertical_scale)
    {
        BN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the vertical scale
     * of the given affine_bg_ptr.
     * @param new_vertical_scale New vertical scale when the action is updated duration_updates times.
     */
    affine_bg_vertical_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_vertical_scale) :
        toggle_value_template_action(move(bg), duration_updates, new_vertical_scale)
    {
        BN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_vertical_scale() const
    {
        return new_property();
    }
};


// scale

/**
 * @brief Manages the scale of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_scale_manager
{

public:
    /**
     * @brief Returns the horizontal scale of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.horizontal_scale();
    }

    /**
     * @brief Sets the scale of the given affine_bg_ptr.
     */
    static void set(fixed scale, affine_bg_ptr& bg)
    {
        bg.set_scale(scale);
    }
};


/**
 * @brief Modifies the scale of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_scale_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the scale of the given affine_bg_ptr is equal to final_scale.
     * @param final_scale scale when the action is updated duration_updates times.
     */
    affine_bg_scale_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_scale) :
        to_value_template_action(bg, duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the scale of the given affine_bg_ptr is equal to final_scale.
     * @param final_scale Scale when the action is updated duration_updates times.
     */
    affine_bg_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_scale) :
        to_value_template_action(move(bg), duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the scale of an affine_bg_ptr from a minimum to a maximum.
 * When the scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_scale_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the scale delta.
     * @param final_scale When the scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_scale_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_scale) :
        loop_value_template_action(bg, duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the scale delta.
     * @param final_scale When the scale of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_scale_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_scale) :
        loop_value_template_action(move(bg), duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the scale of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the scale of an affine_bg_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_scale_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the scale
     * of the given affine_bg_ptr.
     * @param new_scale New scale when the action is updated duration_updates times.
     */
    affine_bg_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_scale) :
        toggle_value_template_action(bg, duration_updates, new_scale)
    {
        BN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the scale
     * of the given affine_bg_ptr.
     * @param new_scale New scale when the action is updated duration_updates times.
     */
    affine_bg_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_scale) :
        toggle_value_template_action(move(bg), duration_updates, new_scale)
    {
        BN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the scale of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_scale() const
    {
        return new_property();
    }
};


// horizontal_shear

/**
 * @brief Manages the horizontal shear of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_shear_manager
{

public:
    /**
     * @brief Returns the horizontal shear of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.horizontal_shear();
    }

    /**
     * @brief Sets the horizontal shear of the given affine_bg_ptr.
     */
    static void set(fixed horizontal_shear, affine_bg_ptr& bg)
    {
        bg.set_horizontal_shear(horizontal_shear);
    }
};


/**
 * @brief Modifies the horizontal shear of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_shear_to_action :
        public to_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal shear of the given affine_bg_ptr is equal to final_horizontal_shear.
     * @param final_horizontal_shear Horizontal shear when the action is updated duration_updates times.
     */
    affine_bg_horizontal_shear_to_action(const affine_bg_ptr& bg, int duration_updates,
                                         fixed final_horizontal_shear) :
        to_value_template_action(bg, duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal shear of the given affine_bg_ptr is equal to final_horizontal_shear.
     * @param final_horizontal_shear Horizontal shear when the action is updated duration_updates times.
     */
    affine_bg_horizontal_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) :
        to_value_template_action(move(bg), duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal shear of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_horizontal_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal shear of an affine_bg_ptr from a minimum to a maximum.
 * When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_shear_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the horizontal shear delta.
     * @param final_horizontal_shear When the horizontal shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_horizontal_shear_loop_action(const affine_bg_ptr& bg, int duration_updates,
                                           fixed final_horizontal_shear) :
        loop_value_template_action(bg, duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the horizontal shear delta.
     * @param final_horizontal_shear When the horizontal shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_horizontal_shear_loop_action(affine_bg_ptr&& bg, int duration_updates,
                                           fixed final_horizontal_shear) :
        loop_value_template_action(move(bg), duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the horizontal shear of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_horizontal_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the horizontal shear of an affine_bg_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_shear_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the horizontal shear
     * of the given affine_bg_ptr.
     * @param new_horizontal_shear New horizontal shear when the action is updated duration_updates times.
     */
    affine_bg_horizontal_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates,
                                             fixed new_horizontal_shear) :
        toggle_value_template_action(bg, duration_updates, new_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the horizontal shear
     * of the given affine_bg_ptr.
     * @param new_horizontal_shear New horizontal shear when the action is updated duration_updates times.
     */
    affine_bg_horizontal_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates,
                                             fixed new_horizontal_shear) :
        toggle_value_template_action(move(bg), duration_updates, new_horizontal_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal shear of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_horizontal_shear() const
    {
        return new_property();
    }
};


// vertical_shear

/**
 * @brief Manages the vertical shear of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_shear_manager
{

public:
    /**
     * @brief Returns the vertical shear of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.vertical_shear();
    }

    /**
     * @brief Sets the vertical shear of the given affine_bg_ptr.
     */
    static void set(fixed vertical_shear, affine_bg_ptr& bg)
    {
        bg.set_vertical_shear(vertical_shear);
    }
};


/**
 * @brief Modifies the vertical shear of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_shear_to_action :
        public to_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical shear of the given affine_bg_ptr is equal to final_vertical_shear.
     * @param final_vertical_shear Vertical shear when the action is updated duration_updates times.
     */
    affine_bg_vertical_shear_to_action(const affine_bg_ptr& bg, int duration_updates,
                                       fixed final_vertical_shear) :
        to_value_template_action(bg, duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical shear of the given affine_bg_ptr is equal to final_vertical_shear.
     * @param final_vertical_shear Vertical shear when the action is updated duration_updates times.
     */
    affine_bg_vertical_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) :
        to_value_template_action(move(bg), duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical shear of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_vertical_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical shear of an affine_bg_ptr from a minimum to a maximum.
 * When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_shear_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the vertical shear delta.
     * @param final_vertical_shear When the vertical shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_vertical_shear_loop_action(const affine_bg_ptr& bg, int duration_updates,
                                         fixed final_vertical_shear) :
        loop_value_template_action(bg, duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the vertical shear delta.
     * @param final_vertical_shear When the vertical shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_vertical_shear_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) :
        loop_value_template_action(move(bg), duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the vertical shear of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_vertical_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the vertical shear of an affine_bg_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_shear_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the vertical shear
     * of the given affine_bg_ptr.
     * @param new_vertical_shear New vertical shear when the action is updated duration_updates times.
     */
    affine_bg_vertical_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates,
                                           fixed new_vertical_shear) :
        toggle_value_template_action(bg, duration_updates, new_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the vertical shear
     * of the given affine_bg_ptr.
     * @param new_vertical_shear New vertical shear when the action is updated duration_updates times.
     */
    affine_bg_vertical_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_vertical_shear) :
        toggle_value_template_action(move(bg), duration_updates, new_vertical_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical shear of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_vertical_shear() const
    {
        return new_property();
    }
};


// shear

/**
 * @brief Manages the shear of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_shear_manager
{

public:
    /**
     * @brief Returns the horizontal shear of the given affine_bg_ptr.
     */
    [[nodiscard]] static fixed get(const affine_bg_ptr& bg)
    {
        return bg.horizontal_shear();
    }

    /**
     * @brief Sets the shear of the given affine_bg_ptr.
     */
    static void set(fixed shear, affine_bg_ptr& bg)
    {
        bg.set_shear(shear);
    }
};


/**
 * @brief Modifies the shear of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_shear_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the shear of the given affine_bg_ptr is equal to final_shear.
     * @param final_shear Shear when the action is updated duration_updates times.
     */
    affine_bg_shear_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_shear) :
        to_value_template_action(bg, duration_updates, final_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the shear of the given affine_bg_ptr is equal to final_shear.
     * @param final_shear Shear when the action is updated duration_updates times.
     */
    affine_bg_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_shear) :
        to_value_template_action(move(bg), duration_updates, final_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the shear of the given affine_bg_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the shear of an affine_bg_ptr from a minimum to a maximum.
 * When the shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_shear_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the shear delta.
     * @param final_shear When the shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_shear_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_shear) :
        loop_value_template_action(bg, duration_updates, final_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the shear delta.
     * @param final_shear When the shear of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_shear_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_shear) :
        loop_value_template_action(move(bg), duration_updates, final_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the shear of the given affine_bg_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the shear of an affine_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_shear_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to change the shear
     * of the given affine_bg_ptr.
     * @param new_shear New shear when the action is updated duration_updates times.
     */
    affine_bg_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_shear) :
        toggle_value_template_action(bg, duration_updates, new_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to change the shear
     * of the given affine_bg_ptr.
     * @param new_shear New shear when the action is updated duration_updates times.
     */
    affine_bg_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_shear) :
        toggle_value_template_action(move(bg), duration_updates, new_shear)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the shear of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_shear() const
    {
        return new_property();
    }
};


// horizontal_flip

/**
 * @brief Manages if an affine_bg_ptr is flipped in its horizontal axis or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_flip_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr is flipped in the horizontal axis or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.horizontal_flip();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must be flipped in the horizontal axis or not.
     */
    static void set(bool horizontal_flip, affine_bg_ptr& bg)
    {
        bg.set_horizontal_flip(horizontal_flip);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr must be flipped in the horizontal axis or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_horizontal_flip_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_horizontal_flip_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be flipped in the horizontal axis or not.
     */
    affine_bg_horizontal_flip_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be flipped in the horizontal axis or not.
     */
    affine_bg_horizontal_flip_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


// vertical_flip

/**
 * @brief Manages if an affine_bg_ptr is flipped in its vertical axis or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_flip_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr is flipped in the vertical axis or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.vertical_flip();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must be flipped in the vertical axis or not.
     */
    static void set(bool vertical_flip, affine_bg_ptr& bg)
    {
        bg.set_vertical_flip(vertical_flip);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr must be flipped in the vertical axis or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_vertical_flip_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_vertical_flip_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be flipped in the vertical axis or not.
     */
    affine_bg_vertical_flip_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr must be flipped in the vertical axis or not.
     */
    affine_bg_vertical_flip_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


// position

/**
 * @brief Manages the position of the pivot of an affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_pivot_position_manager
{

public:
    /**
     * @brief Returns the position of the pivot of the given affine_bg_ptr.
     */
    [[nodiscard]] static const fixed_point& get(const affine_bg_ptr& bg)
    {
        return bg.pivot_position();
    }

    /**
     * @brief Sets the position of the pivot of the given affine_bg_ptr.
     */
    static void set(const fixed_point& position, affine_bg_ptr& bg)
    {
        bg.set_pivot_position(position);
    }
};


/**
 * @brief Modifies the position of the pivot of an affine_bg_ptr by a given delta.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_pivot_by_action :
        public by_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param delta_x How much to add to the horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_pivot_by_action(const affine_bg_ptr& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(bg, fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param delta_x How much to add to the horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_pivot_by_action(affine_bg_ptr&& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(bg), fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param delta_position How much to add to the position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_pivot_by_action(const affine_bg_ptr& bg, const fixed_point& delta_position) :
        by_value_template_action(bg, delta_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param delta_position How much to add to the position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     */
    affine_bg_move_pivot_by_action(affine_bg_ptr&& bg, const fixed_point& delta_position) :
        by_value_template_action(move(bg), delta_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the pivot of the given affine_bg_ptr
     * when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of the pivot of an affine_bg_ptr until it has a given state.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_pivot_to_action :
        public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the pivot of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the pivot of the given affine_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the pivot of the given affine_bg_ptr is equal to final_position.
     * @param final_position Position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_to_action(const affine_bg_ptr& bg, int duration_updates,
                                   const fixed_point& final_position) :
        to_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the pivot of the given affine_bg_ptr is equal to final_position.
     * @param final_position Position of the pivot of the given affine_bg_ptr
     * when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_to_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the pivot of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of the pivot of an affine_bg_ptr from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_pivot_loop_action :
        public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the pivot of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the pivot of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_move_pivot_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the pivot of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the pivot of the given affine_bg_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    affine_bg_move_pivot_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the pivot of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_pivot_loop_action(const affine_bg_ptr& bg, int duration_updates,
                                     const fixed_point& final_position) :
        loop_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the pivot of the given affine_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    affine_bg_move_pivot_loop_action(affine_bg_ptr&& bg, int duration_updates,
                                     const fixed_point& final_position) :
        loop_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the position of the pivot of the given affine_bg_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of the pivot of an affine_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_move_pivot_toggle_action :
        public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the pivot of the given affine_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the pivot of the given affine_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the pivot of the given affine_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_toggle_action(const affine_bg_ptr& bg, int duration_updates,
                                       const fixed_point& new_position) :
        toggle_value_template_action(bg, duration_updates, new_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated
     * to change the position of the pivot of the given affine_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    affine_bg_move_pivot_toggle_action(affine_bg_ptr&& bg, int duration_updates,
                                       const fixed_point& new_position) :
        toggle_value_template_action(move(bg), duration_updates, new_position)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the pivot of the given affine_bg_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};


// wrapping

/**
 * @brief Manages if an affine background must wrap around at the edges or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_wrapping_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr wraps around at the edges or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.wrapping_enabled();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must wrap around at the edges or not.
     */
    static void set(bool wrapping_enabled, affine_bg_ptr& bg)
    {
        bg.set_wrapping_enabled(wrapping_enabled);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr must wrap around at the edges or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_wrapping_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_wrapping_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr wraps around at the edges or not.
     */
    affine_bg_wrapping_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr wraps around at the edges or not.
     */
    affine_bg_wrapping_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


// mosaic

/**
 * @brief Manages if the mosaic effect must be applied to an affine_bg_ptr or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_mosaic_manager
{

public:
    /**
     * @brief Indicates if the mosaic effect is applied to the given affine_bg_ptr or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.mosaic_enabled();
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the given affine_bg_ptr or not.
     */
    static void set(bool mosaic_enabled, affine_bg_ptr& bg)
    {
        bg.set_mosaic_enabled(mosaic_enabled);
    }
};


/**
 * @brief Toggles if the mosaic effect must be applied to an affine_bg_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_mosaic_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_mosaic_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    affine_bg_mosaic_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    affine_bg_mosaic_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


// blending

/**
 * @brief Manages if blending must be applied to an affine_bg_ptr or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_manager
{

public:
    /**
     * @brief Indicates if blending is applied to the given affine_bg_ptr or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.blending_enabled();
    }

    /**
     * @brief Sets if blending must be applied to the given affine_bg_ptr or not.
     */
    static void set(bool blending_enabled, affine_bg_ptr& bg)
    {
        bg.set_blending_enabled(blending_enabled);
    }
};


/**
 * @brief Toggles if blending must be applied to an affine_bg_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    affine_bg_blending_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    affine_bg_blending_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


/**
 * @brief Manages if an affine_bg_ptr must be part of the blending top layer or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_top_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr is part of the blending top layer or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.blending_top_enabled();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must be part of the blending top layer or not.
     */
    static void set(bool blending_top_enabled, affine_bg_ptr& bg)
    {
        bg.set_blending_top_enabled(blending_top_enabled);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr is part of the blending top layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_top_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr is part of the blending top layer or not.
     */
    affine_bg_blending_top_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr is part of the blending top layer or not.
     */
    affine_bg_blending_top_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};


/**
 * @brief Manages if an affine_bg_ptr must be part of the blending bottom layer or not.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_bottom_manager
{

public:
    /**
     * @brief Indicates if the given affine_bg_ptr is part of the blending bottom layer or not.
     */
    [[nodiscard]] static bool get(const affine_bg_ptr& bg)
    {
        return bg.blending_bottom_enabled();
    }

    /**
     * @brief Sets if the given affine_bg_ptr must be part of the blending bottom layer or not.
     */
    static void set(bool blending_bottom_enabled, affine_bg_ptr& bg)
    {
        bg.set_blending_bottom_enabled(blending_bottom_enabled);
    }
};


/**
 * @brief Toggles if an affine_bg_ptr is part of the blending bottom layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup affine_bg
 * @ingroup action
 */
class affine_bg_blending_bottom_toggle_action :
        public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to copy.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr is part of the blending bottom layer or not.
     */
    affine_bg_blending_bottom_toggle_action(const affine_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg affine_bg_ptr to move.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the given affine_bg_ptr is part of the blending bottom layer or not.
     */
    affine_bg_blending_bottom_toggle_action(affine_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return value();
    }
};

}

#endif
