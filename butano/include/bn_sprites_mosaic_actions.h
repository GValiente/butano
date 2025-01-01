/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_MOSAIC_ACTIONS_H
#define BN_SPRITES_MOSAIC_ACTIONS_H

/**
 * @file
 * bn::sprites_mosaic actions header file.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */

#include "bn_fixed.h"
#include "bn_sprites_mosaic.h"
#include "bn_template_actions.h"

namespace bn
{

// horizontal_stretch

/**
 * @brief Manages the horizontal stretch of the mosaic applied to the sprites.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_manager
{

public:
    /**
     * @brief Returns the horizontal stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] static fixed get()
    {
        return sprites_mosaic::horizontal_stretch();
    }

    /**
     * @brief Sets the horizontal stretch of the mosaic applied to the sprites.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     */
    static void set(fixed horizontal_stretch)
    {
        sprites_mosaic::set_horizontal_stretch(horizontal_stretch);
    }
};


/**
 * @brief Modifies the horizontal stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_to_action :
        public to_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the horizontal stretch is equal to final_horizontal_stretch.
     * @param final_horizontal_stretch Horizontal stretch when the action is updated duration_updates times.
     *
     * This horizontal stretch must be in the range [0..1].
     */
    sprites_mosaic_horizontal_stretch_to_action(int duration_updates, fixed final_horizontal_stretch) :
        to_template_action(duration_updates, final_horizontal_stretch)
    {
        BN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    /**
     * @brief Returns the horizontal stretch when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal stretch of the mosaic applied to the sprites from a minimum to a maximum.
 * When the horizontal stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_loop_action :
        public loop_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the horizontal stretch delta.
     * @param final_horizontal_stretch When the horizontal stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This horizontal stretch must be in the range [0..1].
     */
    sprites_mosaic_horizontal_stretch_loop_action(int duration_updates, fixed final_horizontal_stretch) :
        loop_template_action(duration_updates, final_horizontal_stretch)
    {
        BN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    /**
     * @brief When the horizontal stretch is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the horizontal stretch of the mosaic applied to the sprites
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_toggle_action :
        public toggle_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the horizontal stretch.
     * @param new_horizontal_stretch New horizontal stretch to set when the action is updated duration_updates times.
     *
     * This horizontal stretch must be in the range [0..1].
     */
    sprites_mosaic_horizontal_stretch_toggle_action(int duration_updates, fixed new_horizontal_stretch) :
        toggle_template_action(duration_updates, new_horizontal_stretch)
    {
        BN_ASSERT(new_horizontal_stretch >= 0 && new_horizontal_stretch <= 1,
                   "Invalid new horizontal stretch: ", new_horizontal_stretch);
    }

    /**
     * @brief Returns the horizontal stretch to set when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_horizontal_stretch() const
    {
        return new_property();
    }
};


// vertical_stretch

/**
 * @brief Manages the vertical stretch of the mosaic applied to the sprites.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_manager
{

public:
    /**
     * @brief Returns the vertical stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] static fixed get()
    {
        return sprites_mosaic::vertical_stretch();
    }

    /**
     * @brief Sets the vertical stretch of the mosaic applied to the sprites.
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    static void set(fixed vertical_stretch)
    {
        sprites_mosaic::set_vertical_stretch(vertical_stretch);
    }
};


/**
 * @brief Modifies the vertical stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_to_action :
        public to_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the vertical stretch is equal to final_vertical_stretch.
     * @param final_vertical_stretch Vertical stretch when the action is updated duration_updates times.
     *
     * This vertical stretch must be in the range [0..1].
     */
    sprites_mosaic_vertical_stretch_to_action(int duration_updates, fixed final_vertical_stretch) :
        to_template_action(duration_updates, final_vertical_stretch)
    {
        BN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    /**
     * @brief Returns the vertical stretch when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical stretch of the mosaic applied to the sprites from a minimum to a maximum.
 * When the vertical stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_loop_action :
        public loop_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the vertical stretch delta.
     * @param final_vertical_stretch When the vertical stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This vertical stretch must be in the range [0..1].
     */
    sprites_mosaic_vertical_stretch_loop_action(int duration_updates, fixed final_vertical_stretch) :
        loop_template_action(duration_updates, final_vertical_stretch)
    {
        BN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    /**
     * @brief When the vertical stretch is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the vertical stretch of the mosaic applied to the sprites
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_toggle_action :
        public toggle_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the vertical stretch.
     * @param new_vertical_stretch New vertical stretch to set when the action is updated duration_updates times.
     *
     * This vertical stretch must be in the range [0..1].
     */
    sprites_mosaic_vertical_stretch_toggle_action(int duration_updates, fixed new_vertical_stretch) :
        toggle_template_action(duration_updates, new_vertical_stretch)
    {
        BN_ASSERT(new_vertical_stretch >= 0 && new_vertical_stretch <= 1,
                   "Invalid new vertical stretch: ", new_vertical_stretch);
    }

    /**
     * @brief Returns the vertical stretch to set when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_vertical_stretch() const
    {
        return new_property();
    }
};


// stretch

/**
 * @brief Manages the stretch of the mosaic applied to the sprites.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_manager
{

public:
    /**
     * @brief Returns the horizontal stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] static fixed get()
    {
        return sprites_mosaic::horizontal_stretch();
    }

    /**
     * @brief Sets the stretch of the mosaic applied to the sprites.
     * @param stretch Stretch in the range [0..1].
     */
    static void set(fixed stretch)
    {
        sprites_mosaic::set_stretch(stretch);
    }
};


/**
 * @brief Modifies the stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_to_action : public to_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the stretch is equal to final_stretch.
     * @param final_stretch Stretch when the action is updated duration_updates times.
     *
     * This stretch must be in the range [0..1].
     */
    sprites_mosaic_stretch_to_action(int duration_updates, fixed final_stretch) :
        to_template_action(duration_updates, final_stretch)
    {
        BN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    /**
     * @brief Returns the stretch when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the stretch of the mosaic applied to the sprites from a minimum to a maximum.
 * When the stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_loop_action : public loop_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the stretch delta.
     * @param final_stretch When the stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This stretch must be in the range [0..1].
     */
    sprites_mosaic_stretch_loop_action(int duration_updates, fixed final_stretch) :
        loop_template_action(duration_updates, final_stretch)
    {
        BN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    /**
     * @brief When the stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the stretch of the mosaic applied to the sprites
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_toggle_action : public toggle_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the stretch.
     * @param new_stretch New stretch to set when the action is updated duration_updates times.
     *
     * This stretch must be in the range [0..1].
     */
    sprites_mosaic_stretch_toggle_action(int duration_updates, fixed new_stretch) :
        toggle_template_action(duration_updates, new_stretch)
    {
        BN_ASSERT(new_stretch >= 0 && new_stretch <= 1, "Invalid new stretch: ", new_stretch);
    }

    /**
     * @brief Returns the stretch to set when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_stretch() const
    {
        return new_property();
    }
};

}

#endif
