/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITES_MOSAIC_ACTIONS_H
#define BTN_SPRITES_MOSAIC_ACTIONS_H

#include "btn_fixed.h"
#include "btn_sprites_mosaic.h"
#include "btn_template_actions.h"

namespace btn
{

// horizontal_stretch

/**
 * @brief Manages the horizontal stretch of the mosaic applied to the sprites.
 *
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
     * @brief Set the horizontal stretch of the mosaic applied to the sprites.
     */
    static void set(fixed horizontal_stretch)
    {
        sprites_mosaic::set_horizontal_stretch(horizontal_stretch);
    }
};


/**
 * @brief Modifies the horizontal stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_to_action :
        public to_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that update has to be called
     * until the horizontal stretch is equal to final_horizontal_stretch.
     * @param final_horizontal_stretch Horizontal stretch when update is called duration_updates times.
     */
    sprites_mosaic_horizontal_stretch_to_action(int duration_updates, fixed final_horizontal_stretch) :
        to_template_action(duration_updates, final_horizontal_stretch)
    {
        BTN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    /**
     * @brief Returns the horizontal stretch when update is called duration_updates times.
     */
    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the horizontal stretch of the mosaic applied to the sprites delta from a minimum to a maximum.
 * When the horizontal stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_loop_action :
        public loop_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the horizontal stretch delta.
     * @param final_horizontal_stretch When the horizontal stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprites_mosaic_horizontal_stretch_loop_action(int duration_updates, fixed final_horizontal_stretch) :
        loop_template_action(duration_updates, final_horizontal_stretch)
    {
        BTN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
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
 * when a given amount of update calls are done.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_horizontal_stretch_toggle_action :
        public toggle_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done to change the horizontal stretch.
     * @param new_horizontal_stretch New horizontal stretch to set when update is called duration_updates times.
     */
    sprites_mosaic_horizontal_stretch_toggle_action(int duration_updates, fixed new_horizontal_stretch) :
        toggle_template_action(duration_updates, new_horizontal_stretch)
    {
        BTN_ASSERT(new_horizontal_stretch >= 0 && new_horizontal_stretch <= 1,
                   "Invalid new horizontal stretch: ", new_horizontal_stretch);
    }

    /**
     * @brief Returns the new horizontal stretch to set when update is called duration_updates times.
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
     * @brief Set the vertical stretch of the mosaic applied to the sprites.
     */
    static void set(fixed vertical_stretch)
    {
        sprites_mosaic::set_vertical_stretch(vertical_stretch);
    }
};


/**
 * @brief Modifies the vertical stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_to_action :
        public to_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that update has to be called
     * until the vertical stretch is equal to final_vertical_stretch.
     * @param final_vertical_stretch Vertical stretch when update is called duration_updates times.
     */
    sprites_mosaic_vertical_stretch_to_action(int duration_updates, fixed final_vertical_stretch) :
        to_template_action(duration_updates, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    /**
     * @brief Returns the vertical stretch when update is called duration_updates times.
     */
    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the vertical stretch of the mosaic applied to the sprites delta from a minimum to a maximum.
 * When the vertical stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_loop_action :
        public loop_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the vertical stretch delta.
     * @param final_vertical_stretch When the vertical stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprites_mosaic_vertical_stretch_loop_action(int duration_updates, fixed final_vertical_stretch) :
        loop_template_action(duration_updates, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
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
 * when a given amount of update calls are done.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_vertical_stretch_toggle_action :
        public toggle_template_action<fixed, sprites_mosaic_vertical_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done to change the vertical stretch.
     * @param new_vertical_stretch New vertical stretch to set when update is called duration_updates times.
     */
    sprites_mosaic_vertical_stretch_toggle_action(int duration_updates, fixed new_vertical_stretch) :
        toggle_template_action(duration_updates, new_vertical_stretch)
    {
        BTN_ASSERT(new_vertical_stretch >= 0 && new_vertical_stretch <= 1,
                   "Invalid new vertical stretch: ", new_vertical_stretch);
    }

    /**
     * @brief Returns the new vertical stretch to set when update is called duration_updates times.
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
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_manager
{

public:
    /**
     * @brief Returns the stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] static fixed get()
    {
        return sprites_mosaic::horizontal_stretch();
    }

    /**
     * @brief Set the stretch of the mosaic applied to the sprites.
     */
    static void set(fixed stretch)
    {
        sprites_mosaic::set_stretch(stretch);
    }
};


/**
 * @brief Modifies the stretch of the mosaic applied to the sprites until it has a given state.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_to_action : public to_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that update has to be called
     * until the stretch is equal to final_stretch.
     * @param final_stretch Stretch when update is called duration_updates times.
     */
    sprites_mosaic_stretch_to_action(int duration_updates, fixed final_stretch) :
        to_template_action(duration_updates, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    /**
     * @brief Returns the stretch when update is called duration_updates times.
     */
    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the stretch of the mosaic applied to the sprites delta from a minimum to a maximum.
 * When the stretch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_loop_action : public loop_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the stretch delta.
     * @param final_stretch When the stretch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sprites_mosaic_stretch_loop_action(int duration_updates, fixed final_stretch) :
        loop_template_action(duration_updates, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    /**
     * @brief When the stretch is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the stretch of the mosaic applied to the sprites
 * when a given amount of update calls are done.
 *
 * @ingroup mosaic
 * @ingroup action
 */
class sprites_mosaic_stretch_toggle_action : public toggle_template_action<fixed, sprites_mosaic_stretch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much update calls have to be done to change the stretch.
     * @param new_stretch New stretch to set when update is called duration_updates times.
     */
    sprites_mosaic_stretch_toggle_action(int duration_updates, fixed new_stretch) :
        toggle_template_action(duration_updates, new_stretch)
    {
        BTN_ASSERT(new_stretch >= 0 && new_stretch <= 1, "Invalid new stretch: ", new_stretch);
    }

    /**
     * @brief Returns the new stretch to set when update is called duration_updates times.
     */
    [[nodiscard]] fixed new_stretch() const
    {
        return new_property();
    }
};

}

#endif
