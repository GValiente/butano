/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_ACTIONS_H
#define BN_REGULAR_BG_ACTIONS_H

/**
 * @file
 * bn::regular_bg_ptr actions header file.
 *
 * @ingroup regular_bg
 * @ingroup action
 */

#include "bn_regular_bg_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if a regular_bg_ptr must be committed to the GBA or not.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_visible_manager
{

public:
    /**
     * @brief Indicates if the given regular_bg_ptr is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.visible();
    }

    /**
     * @brief Sets if the given regular_bg_ptr must be committed to the GBA or not.
     */
    static void set(bool visible, regular_bg_ptr& bg)
    {
        bg.set_visible(visible);
    }
};


/**
 * @brief Toggles if a regular_bg_ptr must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_visible_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr must be committed to the GBA or not.
     */
    regular_bg_visible_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr must be committed to the GBA or not.
     */
    regular_bg_visible_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};


// position

/**
 * @brief Manages the position of a regular_bg_ptr.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_position_manager
{

public:
    /**
     * @brief Returns the position of the given regular_bg_ptr.
     */
    [[nodiscard]] static const fixed_point& get(const regular_bg_ptr& bg)
    {
        return bg.position();
    }

    /**
     * @brief Sets the position of the given regular_bg_ptr.
     */
    static void set(const fixed_point& position, regular_bg_ptr& bg)
    {
        bg.set_position(position);
    }
};


/**
 * @brief Modifies the position of a regular_bg_ptr by a given delta.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_move_by_action :
        public by_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param delta_x How much to add to the horizontal position of the given regular_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given regular_bg_ptr
     * when the action is updated.
     */
    regular_bg_move_by_action(const regular_bg_ptr& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(bg, fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param delta_x How much to add to the horizontal position of the given regular_bg_ptr
     * when the action is updated.
     * @param delta_y How much to add to the horizontal position of the given regular_bg_ptr
     * when the action is updated.
     */
    regular_bg_move_by_action(regular_bg_ptr&& bg, fixed delta_x, fixed delta_y) :
        by_value_template_action(move(bg), fixed_point(delta_x, delta_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param delta_position How much to add to the position of the given regular_bg_ptr when the action is updated.
     */
    regular_bg_move_by_action(const regular_bg_ptr& bg, const fixed_point& delta_position) :
        by_value_template_action(bg, delta_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param delta_position How much to add to the position of the given regular_bg_ptr when the action is updated.
     */
    regular_bg_move_by_action(regular_bg_ptr&& bg, const fixed_point& delta_position) :
        by_value_template_action(move(bg), delta_position)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns how much to add to the position of the given regular_bg_ptr when the action is updated.
     */
    [[nodiscard]] const fixed_point& delta_position() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the position of a regular_bg_ptr until it has a given state.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_move_to_action :
        public to_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given regular_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given regular_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the given regular_bg_ptr
     * when the action is updated duration_updates times.
     */
    regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given regular_bg_ptr is equal to (final_x, final_y).
     * @param final_x Horizontal position of the given regular_bg_ptr
     * when the action is updated duration_updates times.
     * @param final_y Vertical position of the given regular_bg_ptr
     * when the action is updated duration_updates times.
     */
    regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given regular_bg_ptr is equal to final_position.
     * @param final_position Position of the given regular_bg_ptr
     * when the action is updated duration_updates times.
     */
    regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the position of the given regular_bg_ptr is equal to final_position.
     * @param final_position Position of the given regular_bg_ptr when the action is updated duration_updates times.
     */
    regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        to_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given regular_bg_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the position of a regular_bg_ptr from a minimum to a maximum.
 * When the position is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_move_loop_action :
        public loop_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_x When the horizontal position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     * @param final_y When the vertical position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) :
        loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(bg, duration_updates, final_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the position delta.
     * @param final_position When the position of the given regular_bg_ptr is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) :
        loop_value_template_action(move(bg), duration_updates, final_position)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief When the position of the given regular_bg_ptr is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const fixed_point& final_position() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the position of a regular_bg_ptr when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_move_toggle_action :
        public toggle_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given regular_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given regular_bg_ptr.
     * @param new_x New horizontal position when the action is updated duration_updates times.
     * @param new_y New vertical position when the action is updated duration_updates times.
     */
    regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) :
        toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given regular_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(bg, duration_updates, new_position)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * to change the position of the given regular_bg_ptr.
     * @param new_position New position when the action is updated duration_updates times.
     */
    regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) :
        toggle_value_template_action(move(bg), duration_updates, new_position)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }

    /**
     * @brief Returns the position of the given regular_bg_ptr when the action is updated the given number of times.
     */
    [[nodiscard]] const fixed_point& new_position() const
    {
        return new_property();
    }
};


// mosaic

/**
 * @brief Manages if the mosaic effect must be applied to a regular_bg_ptr or not.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_mosaic_manager
{

public:
    /**
     * @brief Indicates if the mosaic effect is applied to the given regular_bg_ptr or not.
     */
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.mosaic_enabled();
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the given regular_bg_ptr or not.
     */
    static void set(bool mosaic_enabled, regular_bg_ptr& bg)
    {
        bg.set_mosaic_enabled(mosaic_enabled);
    }
};


/**
 * @brief Toggles if the mosaic effect must be applied to a regular_bg_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_mosaic_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_mosaic_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    regular_bg_mosaic_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the mosaic effect must be applied or not.
     */
    regular_bg_mosaic_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};


// blending

/**
 * @brief Manages if blending must be applied to a regular_bg_ptr or not.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_manager
{

public:
    /**
     * @brief Indicates if blending is applied to the given regular_bg_ptr or not.
     */
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.blending_enabled();
    }

    /**
     * @brief Sets if blending must be applied to the given regular_bg_ptr or not.
     */
    static void set(bool blending_enabled, regular_bg_ptr& bg)
    {
        bg.set_blending_enabled(blending_enabled);
    }
};


/**
 * @brief Toggles if blending must be applied to a regular_bg_ptr or not
 * when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    regular_bg_blending_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if blending must be applied or not.
     */
    regular_bg_blending_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};


/**
 * @brief Manages if a regular_bg_ptr must be part of the blending top layer or not.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_top_manager
{

public:
    /**
     * @brief Indicates if the given regular_bg_ptr is part of the blending top layer or not.
     */
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.blending_top_enabled();
    }

    /**
     * @brief Sets if the given regular_bg_ptr must be part of the blending top layer or not.
     */
    static void set(bool blending_top_enabled, regular_bg_ptr& bg)
    {
        bg.set_blending_top_enabled(blending_top_enabled);
    }
};


/**
 * @brief Toggles if a regular_bg_ptr is part of the blending top layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_top_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_top_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr is part of the blending top layer or not.
     */
    regular_bg_blending_top_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr is part of the blending top layer or not.
     */
    regular_bg_blending_top_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};


/**
 * @brief Manages if a regular_bg_ptr must be part of the blending bottom layer or not.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_bottom_manager
{

public:
    /**
     * @brief Indicates if the given regular_bg_ptr is part of the blending bottom layer or not.
     */
    [[nodiscard]] static bool get(const regular_bg_ptr& bg)
    {
        return bg.blending_bottom_enabled();
    }

    /**
     * @brief Sets if the given regular_bg_ptr must be part of the blending bottom layer or not.
     */
    static void set(bool blending_bottom_enabled, regular_bg_ptr& bg)
    {
        bg.set_blending_bottom_enabled(blending_bottom_enabled);
    }
};


/**
 * @brief Toggles if a regular_bg_ptr is part of the blending bottom layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup regular_bg
 * @ingroup action
 */
class regular_bg_blending_bottom_toggle_action :
        public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_bottom_manager>
{

public:
    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr is part of the blending bottom layer or not.
     */
    regular_bg_blending_bottom_toggle_action(const regular_bg_ptr& bg, int duration_updates) :
        bool_toggle_value_template_action(bg, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param bg regular_bg_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given regular_bg_ptr is part of the blending bottom layer or not.
     */
    regular_bg_blending_bottom_toggle_action(regular_bg_ptr&& bg, int duration_updates) :
        bool_toggle_value_template_action(move(bg), duration_updates)
    {
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return value();
    }
};

}

#endif
