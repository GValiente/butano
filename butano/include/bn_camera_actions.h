/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CAMERA_ACTIONS_H
#define BN_CAMERA_ACTIONS_H

/**
 * @file
 * bn::camera_ptr actions header file.
 *
 * @ingroup camera
 * @ingroup action
 */

#include "bn_camera_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// position

/**
 * @brief Manages the position of a camera_ptr.
 *
 * @ingroup camera
 * @ingroup action
 */
class camera_position_manager
{

public:
    /**
     * @brief Returns the position of the given camera_ptr.
     */
    [[nodiscard]] static const fixed_point& get(const camera_ptr& camera)
    {
        return camera.position();
    }

    /**
     * @brief Sets the position of the given camera_ptr.
     */
    static void set(const fixed_point& position, camera_ptr& camera)
    {
        camera.set_position(position);
    }
};


/**
 * @brief Modifies the position of a camera_ptr by a given delta.
 *
 * @ingroup camera
 * @ingroup action
 */
class camera_move_by_action : public by_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param delta_x How much to add to the horizontal position of the given camera_ptr when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given camera_ptr when the action is updated.
     */
    camera_move_by_action(const camera_ptr& camera, fixed delta_x, fixed delta_y) :
        by_value_template_action(camera, fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param delta_x How much to add to the horizontal position of the given camera_ptr when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given camera_ptr when the action is updated.
     */
    camera_move_by_action(camera_ptr&& camera, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(camera), fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param delta_position How much to add to the position of the given camera_ptr when the action is updated.
     */
    camera_move_by_action(const camera_ptr& camera, const fixed_point& delta_position) :
        by_value_template_action(camera, delta_position)
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param delta_position How much to add to the position of the given camera_ptr when the action is updated.
     */
    camera_move_by_action(camera_ptr&& camera, const fixed_point& delta_position) :
        by_value_template_action(move(camera), delta_position)
    {
    }

    /**
     * @brief Returns the camera_ptr to modify.
     */
    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the given camera_ptr when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of a camera_ptr until it has a given state.
 *
 * @ingroup camera
 * @ingroup action
 */
class camera_move_to_action : public to_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given camera_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given camera_ptr when the action is updated duration_updates times.
     * @param final_y Vertical position of the given camera_ptr when the action is updated duration_updates times.
     */
    camera_move_to_action(const camera_ptr& camera, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(camera, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given camera_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given camera_ptr when the action is updated duration_updates times.
     * @param final_y Vertical position of the given camera_ptr when the action is updated duration_updates times.
     */
    camera_move_to_action(camera_ptr&& camera, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(camera), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given camera_ptr is equal to final_position.
     * @param final_position Position of the given camera_ptr when the action is updated duration_updates times.
     */
    camera_move_to_action(const camera_ptr& camera, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(camera, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given camera_ptr is equal to final_position.
     * @param final_position Position of the given camera_ptr when the action is updated duration_updates times.
     */
    camera_move_to_action(camera_ptr&& camera, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(camera), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the camera_ptr to modify.
     */
    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given camera_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of a camera_ptr from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup camera
 * @ingroup action
 */
class camera_move_loop_action : public loop_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    camera_move_loop_action(const camera_ptr& camera, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(camera, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    camera_move_loop_action(camera_ptr&& camera, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(camera), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    camera_move_loop_action(const camera_ptr& camera, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(camera, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given camera_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    camera_move_loop_action(camera_ptr&& camera, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(move(camera), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the camera_ptr to modify.
     */
    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    /**
     * @brief When the position of the given camera_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of a camera_ptr when the action is updated a given number of times.
 *
 * @ingroup camera
 * @ingroup action
 */
class camera_move_toggle_action :
        public toggle_value_template_action<camera_ptr, fixed_point, camera_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given camera_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    camera_move_toggle_action(const camera_ptr& camera, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(camera, duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given camera_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    camera_move_toggle_action(camera_ptr&& camera, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(camera), duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given camera_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    camera_move_toggle_action(const camera_ptr& camera, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(camera, duration_updates, new_position)
    {
    }

    /**
     * @brief Constructor.
     * @param camera camera_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given camera_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    camera_move_toggle_action(camera_ptr&& camera, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(move(camera), duration_updates, new_position)
    {
    }

    /**
     * @brief Returns the camera_ptr to modify.
     */
    [[nodiscard]] const camera_ptr& camera() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given camera_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};

}

#endif
