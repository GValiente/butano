/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BLENDING_ACTIONS_H
#define BN_BLENDING_ACTIONS_H

/**
 * @file
 * bn::blending actions header file.
 *
 * @ingroup blending
 * @ingroup action
 */

#include "bn_fixed.h"
#include "bn_blending.h"
#include "bn_template_actions.h"

namespace bn
{

// transparency_alpha

/**
 * @brief Manages the weight of the transparency blending.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_alpha_manager
{

public:
    /**
     * @brief Returns the weight of the transparency blending,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] static fixed get()
    {
        return blending::transparency_alpha();
    }

    /**
     * @brief Sets the weight of the transparency blending,
     * with 0 being fully transparent and 1 being fully visible.
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     *
     * @param transparency_alpha Transparency weight in the range [0..1].
     */
    static void set(fixed transparency_alpha)
    {
        blending::set_transparency_alpha(transparency_alpha);
    }
};


/**
 * @brief Modifies the weight of the transparency blending until it has a given state.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_alpha_to_action : public to_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the weight of the transparency blending is equal to final_transparency_alpha.
     * @param final_transparency_alpha Weight of the transparency blending
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_alpha_to_action(int duration_updates, fixed final_transparency_alpha) :
        to_template_action(duration_updates, final_transparency_alpha)
    {
        BN_ASSERT(final_transparency_alpha >= 0 && final_transparency_alpha <= 1,
                  "Invalid final transparency alpha: ", final_transparency_alpha);
    }

    /**
     * @brief Returns the weight of the transparency blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_transparency_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the weight of the transparency blending from a minimum to a maximum.
 * When the weight is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_alpha_loop_action :
        public loop_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the weight delta.
     * @param final_transparency_alpha When the weight of the transparency blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_alpha_loop_action(int duration_updates, fixed final_transparency_alpha) :
        loop_template_action(duration_updates, final_transparency_alpha)
    {
        BN_ASSERT(final_transparency_alpha >= 0 && final_transparency_alpha <= 1,
                  "Invalid final transparency alpha: ", final_transparency_alpha);
    }

    /**
     * @brief When the weight of the transparency blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_transparency_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the weight of the transparency blending when the action is updated a given number of times.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_alpha_toggle_action :
        public toggle_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * to change the weight of the transparency blending.
     * @param new_transparency_alpha New weight of the transparency blending
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_alpha_toggle_action(int duration_updates, fixed new_transparency_alpha) :
        toggle_template_action(duration_updates, new_transparency_alpha)
    {
        BN_ASSERT(new_transparency_alpha >= 0 && new_transparency_alpha <= 1,
                  "Invalid new transparency alpha: ", new_transparency_alpha);
    }

    /**
     * @brief Returns the weight of the transparency blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_transparency_alpha() const
    {
        return new_property();
    }
};


// intensity_alpha

/**
 * @brief Manages the weight of the intensity blending.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_intensity_alpha_manager
{

public:
    /**
     * @brief Returns the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
     */
    [[nodiscard]] static fixed get()
    {
        return blending::intensity_alpha();
    }

    /**
     * @brief Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
     *
     * Keep in mind that intensity and fade blendings can't be enabled at the same time.
     *
     * @param intensity_alpha Intensity weight in the range [0..1].
     */
    static void set(fixed intensity_alpha)
    {
        blending::set_intensity_alpha(intensity_alpha);
    }
};


/**
 * @brief Modifies the weight of the intensity blending until it has a given state.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_intensity_alpha_to_action : public to_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the weight of the intensity blending is equal to final_intensity_alpha.
     * @param final_intensity_alpha Weight of the intensity blending
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that intensity and fade blendings can't be enabled at the same time.
     */
    blending_intensity_alpha_to_action(int duration_updates, fixed final_intensity_alpha) :
        to_template_action(duration_updates, final_intensity_alpha)
    {
        BN_ASSERT(final_intensity_alpha >= 0 && final_intensity_alpha <= 1,
                  "Invalid final intensity alpha: ", final_intensity_alpha);
    }

    /**
     * @brief Returns the weight of the intensity blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_intensity_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the weight of the intensity blending from a minimum to a maximum.
 * When the weight is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_intensity_alpha_loop_action :
        public loop_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the weight delta.
     * @param final_intensity_alpha When the weight of the intensity blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that intensity and fade blendings can't be enabled at the same time.
     */
    blending_intensity_alpha_loop_action(int duration_updates, fixed final_intensity_alpha) :
        loop_template_action(duration_updates, final_intensity_alpha)
    {
        BN_ASSERT(final_intensity_alpha >= 0 && final_intensity_alpha <= 1,
                  "Invalid final intensity alpha: ", final_intensity_alpha);
    }

    /**
     * @brief When the weight of the intensity blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the weight of the intensity blending when the action is updated a given number of times.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_intensity_alpha_toggle_action :
        public toggle_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * to change the weight of the intensity blending.
     * @param new_intensity_alpha New weight of the intensity blending
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that intensity and fade blendings can't be enabled at the same time.
     */
    blending_intensity_alpha_toggle_action(int duration_updates, fixed new_intensity_alpha) :
        toggle_template_action(duration_updates, new_intensity_alpha)
    {
        BN_ASSERT(new_intensity_alpha >= 0 && new_intensity_alpha <= 1,
                  "Invalid new intensity alpha: ", new_intensity_alpha);
    }

    /**
     * @brief Returns the weight of the intensity blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity_alpha() const
    {
        return new_property();
    }
};


// transparency_top_weight

/**
 * @brief Manages the weight of the transparency blending for the top layer.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_top_weight_manager
{

public:
    /**
     * @brief Returns the weight of the transparency blending for the top layer,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] static fixed get()
    {
        return blending::transparency_top_weight();
    }

    /**
     * @brief Sets the weight of the transparency blending for the top layer,
     * with 0 being fully transparent and 1 being fully visible.
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     *
     * @param transparency_top_weight Transparency weight in the range [0..1].
     */
    static void set(fixed transparency_top_weight)
    {
        blending::set_transparency_top_weight(transparency_top_weight);
    }
};


/**
 * @brief Modifies the weight of the transparency blending for the top layer until it has a given state.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_top_weight_to_action :
        public to_template_action<fixed, blending_transparency_top_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the weight of the transparency blending for the top layer is equal to final_transparency_top_weight.
     * @param final_transparency_top_weight Weight of the transparency blending for the top layer
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_top_weight_to_action(int duration_updates, fixed final_transparency_top_weight) :
        to_template_action(duration_updates, final_transparency_top_weight)
    {
        BN_ASSERT(final_transparency_top_weight >= 0 && final_transparency_top_weight <= 1,
                  "Invalid final transparency top weight: ", final_transparency_top_weight);
    }

    /**
     * @brief Returns the weight of the transparency blending for the top layer
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_transparency_top_weight() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the weight of the transparency blending for the top layer from a minimum to a maximum.
 * When the weight is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_top_weight_loop_action :
        public loop_template_action<fixed, blending_transparency_top_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the weight delta.
     * @param final_transparency_top_weight When the weight of the transparency blending for the top layer
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_top_weight_loop_action(int duration_updates, fixed final_transparency_top_weight) :
        loop_template_action(duration_updates, final_transparency_top_weight)
    {
        BN_ASSERT(final_transparency_top_weight >= 0 && final_transparency_top_weight <= 1,
                  "Invalid final transparency top weight: ", final_transparency_top_weight);
    }

    /**
     * @brief When the weight of the transparency blending for the top layer is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_transparency_top_weight() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the weight of the transparency blending for the top layer
 * when the action is updated a given number of times.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_top_weight_toggle_action :
        public toggle_template_action<fixed, blending_transparency_top_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * to change the weight of the transparency blending for the top layer.
     * @param new_transparency_top_weight New weight of the transparency blending for the top layer
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_top_weight_toggle_action(int duration_updates, fixed new_transparency_top_weight) :
        toggle_template_action(duration_updates, new_transparency_top_weight)
    {
        BN_ASSERT(new_transparency_top_weight >= 0 && new_transparency_top_weight <= 1,
                  "Invalid new transparency top weight: ", new_transparency_top_weight);
    }

    /**
     * @brief Returns the weight of the transparency blending for the top layer
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_transparency_top_weight() const
    {
        return new_property();
    }
};


// transparency_bottom_weight

/**
 * @brief Manages the weight of the transparency blending for the bottom layer.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_bottom_weight_manager
{

public:
    /**
     * @brief Returns the weight of the transparency blending for the bottom layer,
     * with 0 being fully transparent and 1 being fully visible.
     */
    [[nodiscard]] static fixed get()
    {
        return blending::transparency_bottom_weight();
    }

    /**
     * @brief Sets the weight of the transparency blending for the bottom layer,
     * with 0 being fully transparent and 1 being fully visible.
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     *
     * @param transparency_bottom_weight Transparency weight in the range [0..1].
     */
    static void set(fixed transparency_bottom_weight)
    {
        blending::set_transparency_bottom_weight(transparency_bottom_weight);
    }
};


/**
 * @brief Modifies the weight of the transparency blending for the bottom layer until it has a given state.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_bottom_weight_to_action :
        public to_template_action<fixed, blending_transparency_bottom_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the weight of the transparency blending for the bottom layer
     * is equal to final_transparency_bottom_weight.
     * @param final_transparency_bottom_weight Weight of the transparency blending for the bottom layer
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_bottom_weight_to_action(int duration_updates, fixed final_transparency_bottom_weight) :
        to_template_action(duration_updates, final_transparency_bottom_weight)
    {
        BN_ASSERT(final_transparency_bottom_weight >= 0 && final_transparency_bottom_weight <= 1,
                  "Invalid final transparency bottom weight: ", final_transparency_bottom_weight);
    }

    /**
     * @brief Returns the weight of the transparency blending for the bottom layer
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_transparency_bottom_weight() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the weight of the transparency blending for the bottom layer from a minimum to a maximum.
 * When the weight is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_bottom_weight_loop_action :
        public loop_template_action<fixed, blending_transparency_bottom_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the weight delta.
     * @param final_transparency_bottom_weight When the weight of the transparency blending for the bottom layer
     * is equal to this parameter, it goes back to its initial state and vice versa.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_bottom_weight_loop_action(int duration_updates, fixed final_transparency_bottom_weight) :
        loop_template_action(duration_updates, final_transparency_bottom_weight)
    {
        BN_ASSERT(final_transparency_bottom_weight >= 0 && final_transparency_bottom_weight <= 1,
                  "Invalid final transparency bottom weight: ", final_transparency_bottom_weight);
    }

    /**
     * @brief When the weight of the transparency blending for the bottom layer is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_transparency_bottom_weight() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the weight of the transparency blending for the bottom layer
 * when the action is updated a given number of times.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_transparency_bottom_weight_toggle_action :
        public toggle_template_action<fixed, blending_transparency_bottom_weight_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * to change the weight of the transparency blending for the bottom layer.
     * @param new_transparency_bottom_weight New weight of the transparency blending for the bottom layer
     * when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that transparency and fade blendings can't be enabled at the same time.
     */
    blending_transparency_bottom_weight_toggle_action(int duration_updates, fixed new_transparency_bottom_weight) :
        toggle_template_action(duration_updates, new_transparency_bottom_weight)
    {
        BN_ASSERT(new_transparency_bottom_weight >= 0 && new_transparency_bottom_weight <= 1,
                  "Invalid new transparency bottom weight: ", new_transparency_bottom_weight);
    }

    /**
     * @brief Returns the weight of the transparency blending for the bottom layer
     * when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_transparency_bottom_weight() const
    {
        return new_property();
    }
};


// fade_alpha

/**
 * @brief Manages the weight of the fade blending.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_fade_alpha_manager
{

public:
    /**
     * @brief Returns the weight of the fade blending, with 0 being disabled and 1 being fully applied.
     */
    [[nodiscard]] static fixed get()
    {
        return blending::fade_alpha();
    }

    /**
     * @brief Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
     *
     * Keep in mind that fade blending and other blendings can't be enabled at the same time.
     *
     * @param fade_alpha Fade weight in the range [0..1].
     */
    static void set(fixed fade_alpha)
    {
        blending::set_fade_alpha(fade_alpha);
    }
};


/**
 * @brief Modifies the weight of the fade blending until it has a given state.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_fade_alpha_to_action : public to_template_action<fixed, blending_fade_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the weight of the fade blending is equal to final_fade_alpha.
     * @param final_fade_alpha Weight of the fade blending when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that fade blending and other blendings can't be enabled at the same time.
     */
    blending_fade_alpha_to_action(int duration_updates, fixed final_fade_alpha) :
        to_template_action(duration_updates, final_fade_alpha)
    {
        BN_ASSERT(final_fade_alpha >= 0 && final_fade_alpha <= 1, "Invalid final fade alpha: ", final_fade_alpha);
    }

    /**
     * @brief Returns the weight of the fade blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_fade_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the weight of the fade blending from a minimum to a maximum.
 * When the weight is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_fade_alpha_loop_action :
        public loop_template_action<fixed, blending_fade_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the weight delta.
     * @param final_fade_alpha When the weight of the fade blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that fade blending and other blendings can't be enabled at the same time.
     */
    blending_fade_alpha_loop_action(int duration_updates, fixed final_fade_alpha) :
        loop_template_action(duration_updates, final_fade_alpha)
    {
        BN_ASSERT(final_fade_alpha >= 0 && final_fade_alpha <= 1, "Invalid final fade alpha: ", final_fade_alpha);
    }

    /**
     * @brief When the weight of the fade blending is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_fade_alpha() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the weight of the fade blending when the action is updated a given number of times.
 *
 * @ingroup blending
 * @ingroup action
 */
class blending_fade_alpha_toggle_action :
        public toggle_template_action<fixed, blending_fade_alpha_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * to change the weight of the fade blending.
     * @param new_fade_alpha New weight of the fade blending when the action is updated duration_updates times.
     *
     * This weight must be in the range [0..1].
     *
     * Keep in mind that fade blending and other blendings can't be enabled at the same time.
     */
    blending_fade_alpha_toggle_action(int duration_updates, fixed new_fade_alpha) :
        toggle_template_action(duration_updates, new_fade_alpha)
    {
        BN_ASSERT(new_fade_alpha >= 0 && new_fade_alpha <= 1, "Invalid new fade alpha: ", new_fade_alpha);
    }

    /**
     * @brief Returns the weight of the fade blending when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_fade_alpha() const
    {
        return new_property();
    }
};

}

#endif
