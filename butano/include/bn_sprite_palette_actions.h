/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_PALETTE_ACTIONS_H
#define BN_SPRITE_PALETTE_ACTIONS_H

/**
 * @file
 * bn::sprite_palette_ptr actions header file.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */

#include "bn_fixed.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// inverted

/**
 * @brief Manages if the colors of a sprite_palette_ptr must be inverted or not.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_inverted_manager
{

public:
    /**
     * @brief Indicates if the colors of the given sprite_palette_ptr are inverted or not.
     */
    [[nodiscard]] static bool get(const sprite_palette_ptr& palette)
    {
        return palette.inverted();
    }

    /**
     * @brief Sets if the colors of the given sprite_palette_ptr must be inverted or not.
     */
    static void set(bool inverted, sprite_palette_ptr& palette)
    {
        palette.set_inverted(inverted);
    }
};


/**
 * @brief Toggles if the colors of a sprite_palette_ptr must be inverted or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_inverted_toggle_action :
        public bool_toggle_value_template_action<sprite_palette_ptr, sprite_palette_inverted_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the colors of the given sprite_palette_ptr must be inverted or not.
     */
    sprite_palette_inverted_toggle_action(const sprite_palette_ptr& palette, int duration_updates) :
        bool_toggle_value_template_action(palette, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the colors of the given sprite_palette_ptr must be inverted or not.
     */
    sprite_palette_inverted_toggle_action(sprite_palette_ptr&& palette, int duration_updates) :
        bool_toggle_value_template_action(move(palette), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }
};


// grayscale

/**
 * @brief Manages the intensity of the grayscale effect applied to a sprite_palette_ptr.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_grayscale_manager
{

public:
    /**
     * @brief Returns the intensity of the grayscale effect applied to the given sprite_palette_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.grayscale_intensity();
    }

    /**
     * @brief Sets the intensity of the grayscale effect applied to the given sprite_palette_ptr.
     * @param intensity New intensity in the range [0..1].
     * @param palette sprite_palette_ptr to modify.
     */
    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_grayscale_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the grayscale effect applied to a sprite_palette_ptr
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_grayscale_to_action :
        public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_to_action(const sprite_palette_ptr& palette, int duration_updates,
                                       fixed final_intensity) :
        to_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the intensity of the grayscale effect applied to a sprite_palette_ptr
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_grayscale_loop_action :
        public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_loop_action(const sprite_palette_ptr& palette, int duration_updates,
                                         fixed final_intensity) :
        loop_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief When the intensity is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the grayscale effect applied to a sprite_palette_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_grayscale_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_toggle_action(const sprite_palette_ptr& palette, int duration_updates,
                                           fixed new_intensity) :
        toggle_value_template_action(palette, duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// hue_shift

/**
 * @brief Manages the intensity of the hue shift effect applied to a sprite_palette_ptr.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_hue_shift_manager
{

public:
    /**
     * @brief Returns the intensity of the hue shift effect applied to the given sprite_palette_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.hue_shift_intensity();
    }

    /**
     * @brief Sets the intensity of the hue shift effect applied to the given sprite_palette_ptr.
     * @param intensity New intensity in the range [0..1].
     * @param palette sprite_palette_ptr to modify.
     */
    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_hue_shift_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the hue shift effect applied to a sprite_palette_ptr
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_hue_shift_to_action :
        public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_to_action(const sprite_palette_ptr& palette, int duration_updates,
                                       fixed final_intensity) :
        to_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the intensity of the hue shift effect applied to a sprite_palette_ptr
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_hue_shift_loop_action :
        public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_loop_action(const sprite_palette_ptr& palette, int duration_updates,
                                         fixed final_intensity) :
        loop_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief When the intensity is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the hue shift effect applied to a sprite_palette_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_hue_shift_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_toggle_action(const sprite_palette_ptr& palette, int duration_updates,
                                           fixed new_intensity) :
        toggle_value_template_action(palette, duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_hue_shift_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// fade

/**
 * @brief Manages the intensity of the fade effect applied to a sprite_palette_ptr.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_fade_manager
{

public:
    /**
     * @brief Returns the intensity of the fade effect applied to the given sprite_palette_ptr.
     */
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.fade_intensity();
    }

    /**
     * @brief Sets the intensity of the fade effect applied to the given sprite_palette_ptr.
     * @param intensity New intensity in the range [0..1].
     * @param palette sprite_palette_ptr to modify.
     */
    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_fade_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the fade effect applied to a sprite_palette_ptr
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_fade_to_action :
        public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_to_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the intensity of the fade effect applied to a sprite_palette_ptr
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_fade_loop_action :
        public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_loop_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(palette, duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief When the intensity is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the fade effect applied to a sprite_palette_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_fade_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_toggle_action(const sprite_palette_ptr& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(palette, duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// rotate

/**
 * @brief Manages the number of colors to rotate to the right in a sprite_palette_ptr.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_manager
{

public:
    /**
     * @brief Returns the number of colors to rotate to the right in the given sprite_palette_ptr.
     */
    [[nodiscard]] static int get(const sprite_palette_ptr& palette)
    {
        return palette.rotate_count();
    }

    /**
     * @brief Sets the number of colors to rotate to the right in the given sprite_palette_ptr.
     * @param count Number of colors to rotate to the right in the range [2 - colors_count() .. colors_count() - 2].
     * @param palette sprite_palette_ptr to modify.
     */
    static void set(int count, sprite_palette_ptr& palette)
    {
        palette.set_rotate_count(count);
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite_palette_ptr by delta_count
 * when the action is updated a given number of times. When the property is over the given maximum,
 * it goes back to the given minimum and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_by_action :
        public cyclic_duration_by_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when the action is updated duration_updates times.
     */
    sprite_palette_rotate_by_action(const sprite_palette_ptr& palette, int duration_updates, int delta_count) :
        cyclic_duration_by_value_template_action(palette, duration_updates, delta_count, 0, 0)
    {
        int colors_count = this->palette().colors_count();
        BN_ASSERT(abs(delta_count) < colors_count - 1, "Invalid delta count: ", delta_count, " - ", colors_count);

        set_after_max_property(colors_count - 1);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when the action is updated duration_updates times.
     */
    sprite_palette_rotate_by_action(sprite_palette_ptr&& palette, int duration_updates, int delta_count) :
        cyclic_duration_by_value_template_action(move(palette), duration_updates, delta_count, 0, 0)
    {
        int colors_count = this->palette().colors_count();
        BN_ASSERT(abs(delta_count) < colors_count - 1, "Invalid delta count: ", delta_count, " - ", colors_count);

        set_after_max_property(colors_count - 1);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns how much colors to add to the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite_palette_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_to_action :
        public to_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_to_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) :
        to_value_template_action(palette, duration_updates, final_count)
    {
        BN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_to_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) :
        to_value_template_action(move(palette), duration_updates, final_count)
    {
        BN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite_palette_ptr
 * from a minimum to a maximum. When the number of colors to rotate to the right is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_loop_action :
        public loop_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_loop_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) :
        loop_value_template_action(palette, duration_updates, final_count)
    {
        BN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_loop_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) :
        loop_value_template_action(move(palette), duration_updates, final_count)
    {
        BN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief When the number of colors to rotate to the right is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the number of colors to rotate to the right in a sprite_palette_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_toggle_action(const sprite_palette_ptr& palette, int duration_updates, int new_count) :
        toggle_value_template_action(palette, duration_updates, new_count)
    {
        BN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range [2 - colors_count() .. colors_count() - 2].
     */
    sprite_palette_rotate_toggle_action(sprite_palette_ptr&& palette, int duration_updates, int new_count) :
        toggle_value_template_action(move(palette), duration_updates, new_count)
    {
        BN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int new_count() const
    {
        return new_property();
    }
};


// rotate range

struct sprite_palette_rotate_range_context
{
    sprite_palette_ptr palette;
    int start_index;
    int end_index;
};

/**
 * @brief Manages the number of colors to rotate to the right within the specified range of indexes in a sprite_palette_ptr.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_range_manager
{

public:
     /**
     * @brief Returns the number of colors to rotate to the right in the sprite_palette_ptr provided through the given context.
     */
    [[nodiscard]] static int get(const sprite_palette_rotate_range_context& context)
    {
        return context.palette.rotate_count();
    }

     /**
     * @brief Sets the number of colors to rotate to the right in the sprite_palette_ptr within the range of indexes, provided through the given context.
     * @param count Number of colors to rotate to the right in the range [2 - colors_count() .. colors_count() - 2].
     * @param context containing the palette sprite_palette_ptr to modify, and the start_index and end_index that specify the range of colors to be rotated.
     */
    static void set(int count, sprite_palette_rotate_range_context& context)
    {
        context.palette.set_rotate_count_range(count, context.start_index, context.end_index);
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right within the specified range of indexes in a sprite_palette_ptr by delta_count
 * when the action is updated a given number of times. When the property is over the given maximum,
 * it goes back to the given minimum and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_range_by_action :
        public cyclic_duration_by_value_template_action<sprite_palette_rotate_range_context, int, sprite_palette_rotate_range_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_by_action(const sprite_palette_ptr& palette, int duration_updates, int delta_count, int start_index, int end_index) :
        cyclic_duration_by_value_template_action({palette, start_index, end_index}, duration_updates, delta_count, 0, 0)       
    {       
        int colors_count = this->palette().colors_count();
        
        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(delta_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid delta_count: ", delta_count, " - ", colors_count);
        
        int rotation_range_length = (end_index - start_index + colors_count) % colors_count - (start_index > end_index);
        set_after_max_property(rotation_range_length + 1);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_by_action(sprite_palette_ptr&& palette, int duration_updates, int delta_count, int start_index, int end_index) :
        cyclic_duration_by_value_template_action({move(palette), start_index, end_index}, duration_updates, delta_count, 0, 0)
    {        
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(delta_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid delta_count: ", delta_count, " - ", colors_count);

        int rotation_range_length = (end_index - start_index + colors_count) % colors_count - (start_index > end_index);
        set_after_max_property(rotation_range_length + 1);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value().palette;
    }

    /**
     * @brief Returns how much colors to add to the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }    
};


/**
 * @brief Modifies the number of colors to rotate to the right within the specified range of indexes in a sprite_palette_ptr until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_range_to_action : public to_value_template_action<sprite_palette_rotate_range_context, int, sprite_palette_rotate_range_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_to_action(const sprite_palette_ptr& palette, int duration_updates, int final_count, int start_index, int end_index) :
        to_value_template_action({palette, start_index, end_index}, duration_updates, final_count)        
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(final_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid final count: ", final_count, " - ", colors_count);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates Number of times that the action must be updated
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_to_action(sprite_palette_ptr&& palette, int duration_updates, int final_count, int start_index, int end_index) :
        to_value_template_action({move(palette), start_index, end_index}, duration_updates, final_count) 
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(final_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid final count: ", final_count, " - ", colors_count);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value().palette;
    }

    /**
     * @brief Returns the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right within the specified range of indexes in a sprite_palette_ptr from a minimum to a maximum.
 * When the number of colors to rotate to the right is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_range_loop_action :
        public loop_value_template_action<sprite_palette_rotate_range_context, int, sprite_palette_rotate_range_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_loop_action(const sprite_palette_ptr& palette, int duration_updates, int final_count, int start_index, int end_index) :
        loop_value_template_action({palette, start_index, end_index}, duration_updates, final_count)
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(final_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid final count: ", final_count, " - ", colors_count);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_loop_action(sprite_palette_ptr&& palette, int duration_updates, int final_count, int start_index, int end_index) :
        loop_value_template_action({move(palette), start_index, end_index}, duration_updates, final_count)
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(final_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid final count: ", final_count, " - ", colors_count);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value().palette;
    }

    /**
     * @brief When the number of colors to rotate to the right is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] int final_count() const
    {
        return final_property();
    }   
};


/**
 * @brief Changes the number of colors to rotate to the right within the specified range of indexes in a sprite_palette_ptr
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_range_toggle_action :
        public toggle_value_template_action<sprite_palette_rotate_range_context, int, sprite_palette_rotate_range_manager>
{

public:
    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to copy.
     * @param duration_updates How much times the action has to be updated to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_toggle_action(const sprite_palette_ptr& palette, int duration_updates, int new_count, int start_index, int end_index) :
        toggle_value_template_action({palette, start_index, end_index}, duration_updates, new_count)      
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(new_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid new count: ", new_count, " - ", colors_count);
    }

    /**
     * @brief Constructor.
     * @param palette sprite_palette_ptr to move.
     * @param duration_updates How much times the action has to be updated to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right
     * when the action is updated duration_updates times.
     *
     * This count must be in the range:
     *  - If start_index < end_index, [-(end_index - start_index) .. end_index - start_index]
     *  - If start_index > end_index, [-(colors_count() - 1 - start_index + end_index) .. colors_count() - 1 - start_index + end_index]
     * @param start_index Index of the first color in the rotation range [1 .. colors_count() - 1]
     * @param end_index Index of the last color in the rotation range:
     *  - If start_index == 1, [2 .. colors_count() - 1]
     *  - If start_index > 1 and start_index < colors_count() - 1, [1 .. start_index - 1] and [start_index + 1 .. colors_count() - 1]
     *  - If start_index == colors_count() - 1, [1 .. colors_count() - 2]   
     */
    sprite_palette_rotate_range_toggle_action(sprite_palette_ptr&& palette, int duration_updates, int new_count, int start_index, int end_index) :
        toggle_value_template_action({move(palette), start_index, end_index}, duration_updates, new_count)
    {
        int colors_count = this->palette().colors_count();

        BN_ASSERT(start_index > 0 && start_index <= colors_count - 1 && start_index != end_index, "Invalid start_index: ", start_index);
	    BN_ASSERT(end_index > 0 && end_index <= colors_count - 1 && end_index != start_index, "Invalid end_index: ", end_index);
	    BN_ASSERT(abs(new_count) <= ((end_index - start_index + colors_count) % colors_count - (start_index > end_index)),
                    "Invalid new count: ", new_count, " - ", colors_count);
    }

    /**
     * @brief Returns the sprite_palette_ptr to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value().palette;
    }

    /**
     * @brief Returns the number of colors to rotate to the right
     * when the action is updated the given number of times.
     */
    [[nodiscard]] int new_count() const
    {
        return new_property();
    }
};

}

#endif
