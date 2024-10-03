/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_ACTIONS_H
#define BN_SPRITE_ACTIONS_H

/**
 * @file
 * bn::sprite_ptr actions header file.
 *
 * @ingroup sprite
 * @ingroup action
 */

#include "bn_sprite_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if a sprite_ptr must be committed to the GBA or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_visible_manager
{

public:
    /**
     * @brief Indicates if the given sprite_ptr is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.visible();
    }

    /**
     * @brief Sets if the given sprite_ptr must be committed to the GBA or not.
     */
    static void set(bool visible, sprite_ptr& sprite)
    {
        sprite.set_visible(visible);
    }
};


/**
 * @brief Toggles if a sprite_ptr must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_visible_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be committed to the GBA or not.
     */
    sprite_visible_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be committed to the GBA or not.
     */
    sprite_visible_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// position

/**
 * @brief Manages the position of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_position_manager
{

public:
    /**
     * @brief Returns the position of the given sprite_ptr.
     */
    [[nodiscard]] static const fixed_point& get(const sprite_ptr& sprite)
    {
        return sprite.position();
    }

    /**
     * @brief Sets the position of the given sprite_ptr.
     */
    static void set(const fixed_point& position, sprite_ptr& sprite)
    {
        sprite.set_position(position);
    }
};


/**
 * @brief Modifies the position of a sprite_ptr by a given delta.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_move_by_action : public by_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param delta_x How much to add to the horizontal position of the given sprite_ptr when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given sprite_ptr when the action is updated.
     */
    sprite_move_by_action(const sprite_ptr& sprite, fixed delta_x, fixed delta_y) :
        by_value_template_action(sprite, fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param delta_x How much to add to the horizontal position of the given sprite_ptr when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given sprite_ptr when the action is updated.
     */
    sprite_move_by_action(sprite_ptr&& sprite, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(sprite), fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param delta_position How much to add to the position of the given sprite_ptr when the action is updated.
     */
    sprite_move_by_action(const sprite_ptr& sprite, const fixed_point& delta_position) :
        by_value_template_action(sprite, delta_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param delta_position How much to add to the position of the given sprite_ptr when the action is updated.
     */
    sprite_move_by_action(sprite_ptr&& sprite, const fixed_point& delta_position) :
        by_value_template_action(move(sprite), delta_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the given sprite_ptr when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_move_to_action : public to_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given sprite_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given sprite_ptr when the action is updated duration_updates times.
     * @param final_y Vertical position of the given sprite_ptr when the action is updated duration_updates times.
     */
    sprite_move_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(sprite, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given sprite_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given sprite_ptr when the action is updated duration_updates times.
     * @param final_y Vertical position of the given sprite_ptr when the action is updated duration_updates times.
     */
    sprite_move_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(sprite), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given sprite_ptr is equal to final_position.
     * @param final_position Position of the given sprite_ptr when the action is updated duration_updates times.
     */
    sprite_move_to_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(sprite, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given sprite_ptr is equal to final_position.
     * @param final_position Position of the given sprite_ptr when the action is updated duration_updates times.
     */
    sprite_move_to_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(sprite), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given sprite_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of a sprite_ptr from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_move_loop_action : public loop_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_move_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(sprite, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_move_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(sprite), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_move_loop_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(sprite, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_move_loop_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(move(sprite), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the position of the given sprite_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of a sprite_ptr when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_move_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given sprite_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    sprite_move_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(sprite, duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given sprite_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    sprite_move_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(sprite), duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given sprite_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    sprite_move_toggle_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(sprite, duration_updates, new_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given sprite_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    sprite_move_toggle_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(move(sprite), duration_updates, new_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given sprite_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};


// top_left_position

/**
 * @brief Manages the top-left position of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_top_left_position_manager
{

public:
    /**
     * @brief Returns the top-left position of the given sprite_ptr.
     */
    [[nodiscard]] static fixed_point get(const sprite_ptr& sprite)
    {
        return sprite.top_left_position();
    }

    /**
     * @brief Sets the top-left position of the given sprite_ptr.
     */
    static void set(const fixed_point& top_left_position, sprite_ptr& sprite)
    {
        sprite.set_top_left_position(top_left_position);
    }
};


/**
 * @brief Modifies the top-left position of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_top_left_move_to_action :
        public to_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given sprite_ptr is equal to (final_x, final_y).
     * @param final_top_left_x Horizontal top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     * @param final_top_left_y Vertical top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     */
    sprite_top_left_move_to_action(
            const sprite_ptr& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        to_value_template_action(sprite, duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given sprite_ptr is equal to (final_x, final_y).
     * @param final_top_left_x Horizontal top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     * @param final_top_left_y Vertical top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     */
    sprite_top_left_move_to_action(
            sprite_ptr&& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        to_value_template_action(move(sprite), duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given sprite_ptr is equal to final_top_left_position.
     * @param final_top_left_position Top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     */
    sprite_top_left_move_to_action(
            const sprite_ptr& sprite, int duration_updates, const fixed_point& final_top_left_position) :
        to_value_template_action(sprite, duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the top-left position of the given sprite_ptr is equal to final_top_left_position.
     * @param final_top_left_position Top-left position of the given sprite_ptr
     * when the action is updated duration_updates times.
     */
    sprite_top_left_move_to_action(
            sprite_ptr&& sprite, int duration_updates, const fixed_point& final_top_left_position) :
        to_value_template_action(move(sprite), duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the top-left position of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_top_left_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the top-left position of a sprite_ptr from a minimum to a maximum.
 * When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_top_left_move_loop_action :
        public loop_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_x When the horizontal top-left position of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_top_left_y When the vertical top-left position of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_top_left_move_loop_action(
            const sprite_ptr& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        loop_value_template_action(sprite, duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_x When the horizontal top-left position of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     * @param final_top_left_y When the vertical top-left position of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_top_left_move_loop_action(
            sprite_ptr&& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) :
        loop_value_template_action(move(sprite), duration_updates, fixed_point(final_top_left_x, final_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_position When the top-left position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_top_left_move_loop_action(
            const sprite_ptr& sprite, int duration_updates, const fixed_point& final_top_left_position) :
        loop_value_template_action(sprite, duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the top-left position delta.
     * @param final_top_left_position When the top-left position of the given sprite_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprite_top_left_move_loop_action(
            sprite_ptr&& sprite, int duration_updates, const fixed_point& final_top_left_position) :
        loop_value_template_action(move(sprite), duration_updates, final_top_left_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the top-left position of the given sprite_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_top_left_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the top-left position of a sprite_ptr when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_top_left_move_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the top-left position of the given sprite_ptr.
     * @param new_top_left_x New horizontal top-left position when the action is updated duration_updates times.
     * @param new_top_left_y New vertical top-left position when the action is updated duration_updates times.
     */
    sprite_top_left_move_toggle_action(
            const sprite_ptr& sprite, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) :
        toggle_value_template_action(sprite, duration_updates, fixed_point(new_top_left_x, new_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the top-left position of the given sprite_ptr.
     * @param new_top_left_x New horizontal top-left position when the action is updated duration_updates times.
     * @param new_top_left_y New vertical top-left position when the action is updated duration_updates times.
     */
    sprite_top_left_move_toggle_action(
            sprite_ptr&& sprite, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) :
        toggle_value_template_action(move(sprite), duration_updates, fixed_point(new_top_left_x, new_top_left_y))
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the top-left position of the given sprite_ptr.
     * @param new_top_left_position New top-left position when the action is updated duration_updates times.
     */
    sprite_top_left_move_toggle_action(
            const sprite_ptr& sprite, int duration_updates, const fixed_point& new_top_left_position) :
        toggle_value_template_action(sprite, duration_updates, new_top_left_position)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the top-left position of the given sprite_ptr.
     * @param new_top_left_position New top-left position when the action is updated duration_updates times.
     */
    sprite_top_left_move_toggle_action(
            sprite_ptr&& sprite, int duration_updates, const fixed_point& new_top_left_position) :
        toggle_value_template_action(move(sprite), duration_updates, new_top_left_position)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the top-left position of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_top_left_position() const
    {
        return new_property();
    }
};


// rotation

/**
 * @brief Manages the rotation angle of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_rotation_manager
{

public:
    /**
     * @brief Returns the rotation angle of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.rotation_angle();
    }

    /**
     * @brief Sets the rotation angle of the given sprite_ptr.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @param sprite sprite_ptr to modify.
     */
    static void set(fixed rotation_angle, sprite_ptr& sprite)
    {
        sprite.set_rotation_angle(rotation_angle);
    }
};


/**
 * @brief Modifies the rotation angle of a sprite_ptr by delta_rotation_angle.
 * When the rotation angle is over 360, it goes back to 0 and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_rotate_by_action : public cyclic_by_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param delta_rotation_angle How much degrees to add to the rotation angle of the given sprite_ptr
     * when the action is updated.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_by_action(const sprite_ptr& sprite, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(sprite, delta_rotation_angle, 0, 360)
    {
        BN_ASSERT(delta_rotation_angle > -360 && delta_rotation_angle < 360,
                  "Invalid delta rotation angle: ", delta_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param delta_rotation_angle How much degrees to add to the rotation angle of the given sprite_ptr
     * when the action is updated.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_by_action(sprite_ptr&& sprite, fixed delta_rotation_angle) :
        cyclic_by_value_template_action(move(sprite), delta_rotation_angle, 0, 360)
    {
        BN_ASSERT(delta_rotation_angle > -360 && delta_rotation_angle < 360,
                  "Invalid delta rotation angle: ", delta_rotation_angle);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns how much degrees to add to the rotation angle of the given sprite_ptr
     * when the action is updated.
     */
    [[nodiscard]] fixed delta_rotation_angle() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the rotation angle of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_rotate_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the rotation angle of the given sprite_ptr is equal to final_rotation_angle.
     * @param final_rotation_angle Rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_rotation_angle) :
        to_value_template_action(sprite, duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the rotation angle of the given sprite_ptr is equal to final_rotation_angle.
     * @param final_rotation_angle Rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_rotation_angle) :
        to_value_template_action(move(sprite), duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the rotation angle of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the rotation angle of a sprite_ptr from a minimum to a maximum.
 * When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_rotate_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the rotation angle delta.
     * @param final_rotation_angle When the rotation angle of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_rotation_angle) :
        loop_value_template_action(sprite, duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the rotation angle delta.
     * @param final_rotation_angle When the rotation angle of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_rotation_angle) :
        loop_value_template_action(move(sprite), duration_updates, final_rotation_angle)
    {
        BN_ASSERT(final_rotation_angle >= 0 && final_rotation_angle <= 360,
                   "Invalid final rotation angle: ", final_rotation_angle);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the rotation angle of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_rotation_angle() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the rotation angle of a sprite_ptr when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_rotate_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the rotation angle
     * of the given sprite_ptr.
     * @param new_rotation_angle New rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_rotation_angle) :
        toggle_value_template_action(sprite, duration_updates, new_rotation_angle)
    {
        BN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the rotation angle
     * of the given sprite_ptr.
     * @param new_rotation_angle New rotation angle when the action is updated duration_updates times.
     *
     * This rotation angle must be in the range [0..360].
     */
    sprite_rotate_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_rotation_angle) :
        toggle_value_template_action(move(sprite), duration_updates, new_rotation_angle)
    {
        BN_ASSERT(new_rotation_angle >= 0 && new_rotation_angle <= 360,
                   "Invalid new rotation angle: ", new_rotation_angle);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the rotation angle of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_rotation_angle() const
    {
        return new_property();
    }
};


// horizontal_scale

/**
 * @brief Manages the horizontal scale of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_scale_manager
{

public:
    /**
     * @brief Returns the horizontal scale of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_scale();
    }

    /**
     * @brief Sets the horizontal scale of the given sprite_ptr.
     */
    static void set(fixed horizontal_scale, sprite_ptr& sprite)
    {
        sprite.set_horizontal_scale(horizontal_scale);
    }
};


/**
 * @brief Modifies the horizontal scale of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_scale_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal scale of the given sprite_ptr is equal to final_horizontal_scale.
     * @param final_horizontal_scale Horizontal scale when the action is updated duration_updates times.
     */
    sprite_horizontal_scale_to_action(const sprite_ptr& sprite, int duration_updates,
                                      fixed final_horizontal_scale) :
        to_value_template_action(sprite, duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal scale of the given sprite_ptr is equal to final_horizontal_scale.
     * @param final_horizontal_scale Horizontal scale when the action is updated duration_updates times.
     */
    sprite_horizontal_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_scale) :
        to_value_template_action(move(sprite), duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal scale of a sprite_ptr from a minimum to a maximum.
 * When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_scale_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the horizontal scale delta.
     * @param final_horizontal_scale When the horizontal scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_horizontal_scale_loop_action(const sprite_ptr& sprite, int duration_updates,
                                        fixed final_horizontal_scale) :
        loop_value_template_action(sprite, duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the horizontal scale delta.
     * @param final_horizontal_scale When the horizontal scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_horizontal_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_scale) :
        loop_value_template_action(move(sprite), duration_updates, final_horizontal_scale)
    {
        BN_ASSERT(final_horizontal_scale > 0, "Invalid final horizontal scale: ", final_horizontal_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the horizontal scale of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_horizontal_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the horizontal scale of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_scale_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the horizontal scale
     * of the given sprite_ptr.
     * @param new_horizontal_scale New horizontal scale when the action is updated duration_updates times.
     */
    sprite_horizontal_scale_toggle_action(const sprite_ptr& sprite, int duration_updates,
                                          fixed new_horizontal_scale) :
        toggle_value_template_action(sprite, duration_updates, new_horizontal_scale)
    {
        BN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the horizontal scale
     * of the given sprite_ptr.
     * @param new_horizontal_scale New horizontal scale when the action is updated duration_updates times.
     */
    sprite_horizontal_scale_toggle_action(sprite_ptr&& sprite, int duration_updates,
                                          fixed new_horizontal_scale) :
        toggle_value_template_action(move(sprite), duration_updates, new_horizontal_scale)
    {
        BN_ASSERT(new_horizontal_scale > 0, "Invalid new horizontal scale: ", new_horizontal_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_horizontal_scale() const
    {
        return new_property();
    }
};


// vertical_scale

/**
 * @brief Manages the vertical scale of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_scale_manager
{

public:
    /**
     * @brief Returns the vertical scale of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.vertical_scale();
    }

    /**
     * @brief Sets the vertical scale of the given sprite_ptr.
     */
    static void set(fixed vertical_scale, sprite_ptr& sprite)
    {
        sprite.set_vertical_scale(vertical_scale);
    }
};


/**
 * @brief Modifies the vertical scale of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_scale_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical scale of the given sprite_ptr is equal to final_vertical_scale.
     * @param final_vertical_scale Vertical scale when the action is updated duration_updates times.
     */
    sprite_vertical_scale_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_scale) :
        to_value_template_action(sprite, duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical scale of the given sprite_ptr is equal to final_vertical_scale.
     * @param final_vertical_scale Vertical scale when the action is updated duration_updates times.
     */
    sprite_vertical_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_scale) :
        to_value_template_action(move(sprite), duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical scale of a sprite_ptr from a minimum to a maximum.
 * When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_scale_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the vertical scale delta.
     * @param final_vertical_scale When the vertical scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_vertical_scale_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_scale) :
        loop_value_template_action(sprite, duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the vertical scale delta.
     * @param final_vertical_scale When the vertical scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_vertical_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_scale) :
        loop_value_template_action(move(sprite), duration_updates, final_vertical_scale)
    {
        BN_ASSERT(final_vertical_scale > 0, "Invalid final vertical scale: ", final_vertical_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the vertical scale of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_vertical_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the vertical scale of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_scale_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the vertical scale
     * of the given sprite_ptr.
     * @param new_vertical_scale New vertical scale when the action is updated duration_updates times.
     */
    sprite_vertical_scale_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_vertical_scale) :
        toggle_value_template_action(sprite, duration_updates, new_vertical_scale)
    {
        BN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the vertical scale
     * of the given sprite_ptr.
     * @param new_vertical_scale New vertical scale when the action is updated duration_updates times.
     */
    sprite_vertical_scale_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_vertical_scale) :
        toggle_value_template_action(move(sprite), duration_updates, new_vertical_scale)
    {
        BN_ASSERT(new_vertical_scale > 0, "Invalid new vertical scale: ", new_vertical_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_vertical_scale() const
    {
        return new_property();
    }
};


// scale

/**
 * @brief Manages the scale of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_scale_manager
{

public:
    /**
     * @brief Returns the horizontal scale of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_scale();
    }

    /**
     * @brief Sets the scale of the given sprite_ptr.
     */
    static void set(fixed scale, sprite_ptr& sprite)
    {
        sprite.set_scale(scale);
    }
};


/**
 * @brief Modifies the scale of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_scale_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the scale of the given sprite_ptr is equal to final_scale.
     * @param final_scale Scale when the action is updated duration_updates times.
     */
    sprite_scale_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_scale) :
        to_value_template_action(sprite, duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the scale of the given sprite_ptr is equal to final_scale.
     * @param final_scale Scale when the action is updated duration_updates times.
     */
    sprite_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_scale) :
        to_value_template_action(move(sprite), duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the scale of a sprite_ptr from a minimum to a maximum.
 * When the scale is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_scale_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the scale delta.
     * @param final_scale When the scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_scale_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_scale) :
        loop_value_template_action(sprite, duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the scale delta.
     * @param final_scale When the scale of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_scale) :
        loop_value_template_action(move(sprite), duration_updates, final_scale)
    {
        BN_ASSERT(final_scale > 0, "Invalid final scale: ", final_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the scale of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_scale() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the scale of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_scale_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_scale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the scale
     * of the given sprite_ptr.
     * @param new_scale New scale when the action is updated duration_updates times.
     */
    sprite_scale_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_scale) :
        toggle_value_template_action(sprite, duration_updates, new_scale)
    {
        BN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the scale
     * of the given sprite_ptr.
     * @param new_scale New scale when the action is updated duration_updates times.
     */
    sprite_scale_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_scale) :
        toggle_value_template_action(move(sprite), duration_updates, new_scale)
    {
        BN_ASSERT(new_scale > 0, "Invalid new scale: ", new_scale);
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the scale of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_scale() const
    {
        return new_property();
    }
};


// horizontal_shear

/**
 * @brief Manages the horizontal shear of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_shear_manager
{

public:
    /**
     * @brief Returns the horizontal shear of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_shear();
    }

    /**
     * @brief Sets the horizontal shear of the given sprite_ptr.
     */
    static void set(fixed horizontal_shear, sprite_ptr& sprite)
    {
        sprite.set_horizontal_shear(horizontal_shear);
    }
};


/**
 * @brief Modifies the horizontal shear of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_shear_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal shear of the given sprite_ptr is equal to final_horizontal_shear.
     * @param final_horizontal_shear Horizontal shear when the action is updated duration_updates times.
     */
    sprite_horizontal_shear_to_action(const sprite_ptr& sprite, int duration_updates,
                                      fixed final_horizontal_shear) :
        to_value_template_action(sprite, duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal shear of the given sprite_ptr is equal to final_horizontal_shear.
     * @param final_horizontal_shear Horizontal shear when the action is updated duration_updates times.
     */
    sprite_horizontal_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_shear) :
        to_value_template_action(move(sprite), duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_horizontal_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal shear of a sprite_ptr from a minimum to a maximum.
 * When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_shear_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the horizontal shear delta.
     * @param final_horizontal_shear When the horizontal shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_horizontal_shear_loop_action(const sprite_ptr& sprite, int duration_updates,
                                        fixed final_horizontal_shear) :
        loop_value_template_action(sprite, duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the horizontal shear delta.
     * @param final_horizontal_shear When the horizontal shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_horizontal_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_shear) :
        loop_value_template_action(move(sprite), duration_updates, final_horizontal_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the horizontal shear of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_horizontal_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the horizontal shear of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_shear_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the horizontal shear
     * of the given sprite_ptr.
     * @param new_horizontal_shear New horizontal shear when the action is updated duration_updates times.
     */
    sprite_horizontal_shear_toggle_action(const sprite_ptr& sprite, int duration_updates,
                                          fixed new_horizontal_shear) :
        toggle_value_template_action(sprite, duration_updates, new_horizontal_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the horizontal shear
     * of the given sprite_ptr.
     * @param new_horizontal_shear New horizontal shear when the action is updated duration_updates times.
     */
    sprite_horizontal_shear_toggle_action(sprite_ptr&& sprite, int duration_updates,
                                          fixed new_horizontal_shear) :
        toggle_value_template_action(move(sprite), duration_updates, new_horizontal_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the horizontal shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_horizontal_shear() const
    {
        return new_property();
    }
};


// vertical_shear

/**
 * @brief Manages the vertical shear of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_shear_manager
{

public:
    /**
     * @brief Returns the vertical shear of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.vertical_shear();
    }

    /**
     * @brief Sets the vertical shear of the given sprite_ptr.
     */
    static void set(fixed vertical_shear, sprite_ptr& sprite)
    {
        sprite.set_vertical_shear(vertical_shear);
    }
};


/**
 * @brief Modifies the vertical shear of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_shear_to_action :
        public to_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical shear of the given sprite_ptr is equal to final_vertical_shear.
     * @param final_vertical_shear Vertical shear when the action is updated duration_updates times.
     */
    sprite_vertical_shear_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_shear) :
        to_value_template_action(sprite, duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical shear of the given sprite_ptr is equal to final_vertical_shear.
     * @param final_vertical_shear Vertical shear when the action is updated duration_updates times.
     */
    sprite_vertical_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_shear) :
        to_value_template_action(move(sprite), duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_vertical_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical shear of a sprite_ptr from a minimum to a maximum.
 * When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_shear_loop_action :
        public loop_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the vertical shear delta.
     * @param final_vertical_shear When the vertical shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_vertical_shear_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_shear) :
        loop_value_template_action(sprite, duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the vertical shear delta.
     * @param final_vertical_shear When the vertical shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_vertical_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_shear) :
        loop_value_template_action(move(sprite), duration_updates, final_vertical_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the vertical shear of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_vertical_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the vertical shear of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_shear_toggle_action :
        public toggle_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the vertical shear
     * of the given sprite_ptr.
     * @param new_vertical_shear New vertical shear when the action is updated duration_updates times.
     */
    sprite_vertical_shear_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_vertical_shear) :
        toggle_value_template_action(sprite, duration_updates, new_vertical_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the vertical shear
     * of the given sprite_ptr.
     * @param new_vertical_shear New vertical shear when the action is updated duration_updates times.
     */
    sprite_vertical_shear_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_vertical_shear) :
        toggle_value_template_action(move(sprite), duration_updates, new_vertical_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the vertical shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_vertical_shear() const
    {
        return new_property();
    }
};


// shear

/**
 * @brief Manages the shear of a sprite_ptr.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_shear_manager
{

public:
    /**
     * @brief Returns the horizontal shear of the given sprite_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_shear();
    }

    /**
     * @brief Sets the shear of the given sprite_ptr.
     */
    static void set(fixed shear, sprite_ptr& sprite)
    {
        sprite.set_shear(shear);
    }
};


/**
 * @brief Modifies the shear of a sprite_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_shear_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the shear of the given sprite_ptr is equal to final_shear.
     * @param final_shear Shear when the action is updated duration_updates times.
     */
    sprite_shear_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_shear) :
        to_value_template_action(sprite, duration_updates, final_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the shear of the given sprite_ptr is equal to final_shear.
     * @param final_shear Shear when the action is updated duration_updates times.
     */
    sprite_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_shear) :
        to_value_template_action(move(sprite), duration_updates, final_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the shear of a sprite_ptr from a minimum to a maximum.
 * When the shear is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_shear_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the shear delta.
     * @param final_shear When the shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_shear_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_shear) :
        loop_value_template_action(sprite, duration_updates, final_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the shear delta.
     * @param final_shear When the shear of the given sprite_ptr
     * is equal to this parameter, it goes back to its initial state and vice versa.
     */
    sprite_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_shear) :
        loop_value_template_action(move(sprite), duration_updates, final_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief When the shear of the given sprite_ptr
     * is equal to this returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_shear() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the shear of a sprite_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_shear_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_shear_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the shear
     * of the given sprite_ptr.
     * @param new_shear New shear when the action is updated duration_updates times.
     */
    sprite_shear_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_shear) :
        toggle_value_template_action(sprite, duration_updates, new_shear)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the shear
     * of the given sprite_ptr.
     * @param new_shear New shear when the action is updated duration_updates times.
     */
    sprite_shear_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_shear) :
        toggle_value_template_action(move(sprite), duration_updates, new_shear)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }

    /**
     * @brief Returns the shear of the given sprite_ptr
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_shear() const
    {
        return new_property();
    }
};


// horizontal_flip

/**
 * @brief Manages if a sprite_ptr is flipped in its horizontal axis or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_flip_manager
{

public:
    /**
     * @brief Indicates if the given sprite_ptr is flipped in the horizontal axis or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.horizontal_flip();
    }

    /**
     * @brief Sets if the given sprite_ptr must be flipped in the horizontal axis or not.
     */
    static void set(bool horizontal_flip, sprite_ptr& sprite)
    {
        sprite.set_horizontal_flip(horizontal_flip);
    }
};


/**
 * @brief Toggles if a sprite_ptr must be flipped in the horizontal axis or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_horizontal_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_ptr, sprite_horizontal_flip_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be flipped in the horizontal axis or not.
     */
    sprite_horizontal_flip_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be flipped in the horizontal axis or not.
     */
    sprite_horizontal_flip_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// vertical_flip

/**
 * @brief Manages if a sprite_ptr is flipped in its vertical axis or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_flip_manager
{

public:
    /**
     * @brief Indicates if the given sprite_ptr is flipped in the vertical axis or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.vertical_flip();
    }

    /**
     * @brief Sets if the given sprite_ptr must be flipped in the vertical axis or not.
     */
    static void set(bool vertical_flip, sprite_ptr& sprite)
    {
        sprite.set_vertical_flip(vertical_flip);
    }
};


/**
 * @brief Toggles if a sprite_ptr must be flipped in the vertical axis or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_vertical_flip_toggle_action :
        public bool_toggle_value_template_action<sprite_ptr, sprite_vertical_flip_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be flipped in the vertical axis or not.
     */
    sprite_vertical_flip_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be flipped in the vertical axis or not.
     */
    sprite_vertical_flip_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// mosaic

/**
 * @brief Manages if the mosaic effect must be applied to a sprite_ptr or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_mosaic_manager
{

public:
    /**
     * @brief Indicates if the mosaic effect is applied to the given sprite_ptr or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.mosaic_enabled();
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the given sprite_ptr or not.
     */
    static void set(bool mosaic_enabled, sprite_ptr& sprite)
    {
        sprite.set_mosaic_enabled(mosaic_enabled);
    }
};


/**
 * @brief Toggles if the mosaic effect must be applied to a sprite_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_mosaic_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_mosaic_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    sprite_mosaic_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    sprite_mosaic_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// blending

/**
 * @brief Manages if blending must be applied to a sprite_ptr or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_blending_manager
{

public:
    /**
     * @brief Indicates if blending is applied to the given sprite_ptr or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.blending_enabled();
    }

    /**
     * @brief Sets if blending must be applied to the given sprite_ptr or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite_ptr at the same time.
     */
    static void set(bool blending_enabled, sprite_ptr& sprite)
    {
        sprite.set_blending_enabled(blending_enabled);
    }
};


/**
 * @brief Toggles if blending must be applied to a sprite_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_blending_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_blending_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    sprite_blending_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    sprite_blending_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};


// window

/**
 * @brief Manages if a sprite_ptr must be part of the silhouette of the sprite window or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_window_manager
{

public:
    /**
     * @brief Indicates if the given sprite_ptr must be part of the silhouette of the sprite window or not.
     */
    [[nodiscard]] static bool get(const sprite_ptr& sprite)
    {
        return sprite.window_enabled();
    }

    /**
     * @brief Sets if the given sprite_ptr must be part of the silhouette of the sprite window or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite_ptr at the same time.
     */
    static void set(bool window_enabled, sprite_ptr& sprite)
    {
        sprite.set_window_enabled(window_enabled);
    }
};


/**
 * @brief Toggles if a sprite_ptr must be part of the silhouette of the sprite window or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprite_window_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_window_manager>
{

public:
    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be part of the silhouette of the sprite window or not.
     */
    sprite_window_toggle_action(const sprite_ptr& sprite, int duration_updates) :
        bool_toggle_value_template_action(sprite, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param sprite sprite_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given sprite_ptr must be part of the silhouette of the sprite window or not.
     */
    sprite_window_toggle_action(sprite_ptr&& sprite, int duration_updates) :
        bool_toggle_value_template_action(move(sprite), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return value();
    }
};

}

#endif
