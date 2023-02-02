/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_PALETTES_ACTIONS_H
#define BN_SPRITE_PALETTES_ACTIONS_H

/**
 * @file
 * bn::sprite_palettes actions header file.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */

#include "bn_fixed.h"
#include "bn_sprite_palettes.h"
#include "bn_template_actions.h"

namespace bn
{

// brightness

/**
 * @brief Manages the brightness of all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_brightness_manager
{

public:
    /**
     * @brief Returns the brightness of all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::brightness();
    }

    /**
     * @brief Sets the brightness of all sprite color palettes.
     * @param brightness New brightness in the range [0..1].
     */
    static void set(fixed brightness)
    {
        sprite_palettes::set_brightness(brightness);
    }
};


/**
 * @brief Modifies the brightness of all sprite color palettes until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_brightness_to_action : public to_template_action<fixed, sprite_palettes_brightness_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the brightness is equal to final_brightness.
     * @param final_brightness Brightness when the action is updated duration_updates times.
     *
     * This brightness must be in the range [0..1].
     */
    sprite_palettes_brightness_to_action(int duration_updates, fixed final_brightness) :
        to_template_action(duration_updates, final_brightness)
    {
        BN_ASSERT(final_brightness >= 0 && final_brightness <= 1, "Invalid final brightness: ", final_brightness);
    }

    /**
     * @brief Returns the brightness when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_brightness() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the brightness of all sprite color palettes from a minimum to a maximum.
 * When the brightness is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_brightness_loop_action : public loop_template_action<fixed, sprite_palettes_brightness_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the brightness delta.
     * @param final_brightness When the brightness is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This brightness must be in the range [0..1].
     */
    sprite_palettes_brightness_loop_action(int duration_updates, fixed final_brightness) :
        loop_template_action(duration_updates, final_brightness)
    {
        BN_ASSERT(final_brightness >= 0 && final_brightness <= 1, "Invalid final brightness: ", final_brightness);
    }

    /**
     * @brief When the brightness is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_brightness() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the brightness of all sprite color palettes when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_brightness_toggle_action :
        public toggle_template_action<fixed, sprite_palettes_brightness_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the brightness.
     * @param new_brightness New brightness when the action is updated duration_updates times.
     *
     * This brightness must be in the range [0..1].
     */
    sprite_palettes_brightness_toggle_action(int duration_updates, fixed new_brightness) :
        toggle_template_action(duration_updates, new_brightness)
    {
        BN_ASSERT(new_brightness >= 0 && new_brightness <= 1, "Invalid new brightness: ", new_brightness);
    }

    /**
     * @brief Returns the brightness when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_brightness() const
    {
        return new_property();
    }
};


// contrast

/**
 * @brief Manages the contrast of all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_contrast_manager
{

public:
    /**
     * @brief Returns the contrast of all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::contrast();
    }

    /**
     * @brief Sets the contrast of all sprite color palettes.
     * @param contrast New contrast in the range [0..1].
     */
    static void set(fixed contrast)
    {
        sprite_palettes::set_contrast(contrast);
    }
};


/**
 * @brief Modifies the contrast of all sprite color palettes until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_contrast_to_action : public to_template_action<fixed, sprite_palettes_contrast_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the contrast is equal to final_contrast.
     * @param final_contrast Contrast when the action is updated duration_updates times.
     *
     * This contrast must be in the range [0..1].
     */
    sprite_palettes_contrast_to_action(int duration_updates, fixed final_contrast) :
        to_template_action(duration_updates, final_contrast)
    {
        BN_ASSERT(final_contrast >= 0 && final_contrast <= 1, "Invalid final contrast: ", final_contrast);
    }

    /**
     * @brief Returns the contrast when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_contrast() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the contrast of all sprite color palettes from a minimum to a maximum.
 * When the contrast is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_contrast_loop_action : public loop_template_action<fixed, sprite_palettes_contrast_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the contrast delta.
     * @param final_contrast When the contrast is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This contrast must be in the range [0..1].
     */
    sprite_palettes_contrast_loop_action(int duration_updates, fixed final_contrast) :
        loop_template_action(duration_updates, final_contrast)
    {
        BN_ASSERT(final_contrast >= 0 && final_contrast <= 1, "Invalid final contrast: ", final_contrast);
    }

    /**
     * @brief When the contrast is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_contrast() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the contrast of all sprite color palettes when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_contrast_toggle_action : public toggle_template_action<fixed, sprite_palettes_contrast_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the contrast.
     * @param new_contrast New contrast when the action is updated duration_updates times.
     *
     * This contrast must be in the range [0..1].
     */
    sprite_palettes_contrast_toggle_action(int duration_updates, fixed new_contrast) :
        toggle_template_action(duration_updates, new_contrast)
    {
        BN_ASSERT(new_contrast >= 0 && new_contrast <= 1, "Invalid new contrast: ", new_contrast);
    }

    /**
     * @brief Returns the contrast when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_contrast() const
    {
        return new_property();
    }
};


// intensity

/**
 * @brief Manages the intensity of all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_intensity_manager
{

public:
    /**
     * @brief Returns the intensity of all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::intensity();
    }

    /**
     * @brief Sets the intensity of all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    static void set(fixed intensity)
    {
        sprite_palettes::set_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of all sprite color palettes until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_intensity_to_action : public to_template_action<fixed, sprite_palettes_intensity_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_intensity_to_action(int duration_updates, fixed final_intensity) :
        to_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
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
 * @brief Modifies the intensity of all sprite color palettes from a minimum to a maximum.
 * When the intensity is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_intensity_loop_action : public loop_template_action<fixed, sprite_palettes_intensity_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_intensity_loop_action(int duration_updates, fixed final_intensity) :
        loop_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of all sprite color palettes when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_intensity_toggle_action : public toggle_template_action<fixed, sprite_palettes_intensity_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_intensity_toggle_action(int duration_updates, fixed new_intensity) :
        toggle_template_action(duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// inverted

/**
 * @brief Manages if the colors of all sprite color palettes must be inverted or not.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_inverted_manager
{

public:
    /**
     * @brief Indicates if the colors of all sprite color palettes are inverted or not.
     */
    [[nodiscard]] static bool get()
    {
        return sprite_palettes::inverted();
    }

    /**
     * @brief Sets if the colors of all sprite color palettes must be inverted or not.
     */
    static void set(bool inverted)
    {
        sprite_palettes::set_inverted(inverted);
    }
};


/**
 * @brief Toggles if the colors of all sprite color palettes must be inverted or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_inverted_toggle_action : public bool_toggle_template_action<sprite_palettes_inverted_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the colors of all sprite color palettes must be inverted or not.
     */
    explicit sprite_palettes_inverted_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};


// grayscale

/**
 * @brief Manages the intensity of the grayscale effect applied to all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_grayscale_manager
{

public:
    /**
     * @brief Returns the intensity of the grayscale effect applied to all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::grayscale_intensity();
    }

    /**
     * @brief Sets the intensity of the grayscale effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    static void set(fixed intensity)
    {
        sprite_palettes::set_grayscale_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the grayscale effect applied to all sprite color palettes
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_grayscale_to_action : public to_template_action<fixed, sprite_palettes_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_grayscale_to_action(int duration_updates, fixed final_intensity) :
        to_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
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
 * @brief Modifies the intensity of the grayscale effect applied to all sprite color palettes
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_grayscale_loop_action : public loop_template_action<fixed, sprite_palettes_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_grayscale_loop_action(int duration_updates, fixed final_intensity) :
        loop_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the grayscale effect applied to all sprite color palettes
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_grayscale_toggle_action : public toggle_template_action<fixed, sprite_palettes_grayscale_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_grayscale_toggle_action(int duration_updates, fixed new_intensity) :
        toggle_template_action(duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
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
 * @brief Manages the intensity of the hue shift effect applied to all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_hue_shift_manager
{

public:
    /**
     * @brief Returns the intensity of the hue shift effect applied to all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::hue_shift_intensity();
    }

    /**
     * @brief Sets the intensity of the hue shift effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    static void set(fixed intensity)
    {
        sprite_palettes::set_hue_shift_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the hue shift effect applied to all sprite color palettes
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_hue_shift_to_action : public to_template_action<fixed, sprite_palettes_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_hue_shift_to_action(int duration_updates, fixed final_intensity) :
        to_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
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
 * @brief Modifies the intensity of the hue shift effect applied to all sprite color palettes
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_hue_shift_loop_action : public loop_template_action<fixed, sprite_palettes_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_hue_shift_loop_action(int duration_updates, fixed final_intensity) :
        loop_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the hue shift effect applied to all sprite color palettes
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_hue_shift_toggle_action : public toggle_template_action<fixed, sprite_palettes_hue_shift_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_hue_shift_toggle_action(int duration_updates, fixed new_intensity) :
        toggle_template_action(duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
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
 * @brief Manages the intensity of the fade effect applied to all sprite color palettes.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_fade_manager
{

public:
    /**
     * @brief Returns the intensity of the fade effect applied to all sprite color palettes.
     */
    [[nodiscard]] static fixed get()
    {
        return sprite_palettes::fade_intensity();
    }

    /**
     * @brief Sets the intensity of the fade effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    static void set(fixed intensity)
    {
        sprite_palettes::set_fade_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the fade effect applied to all sprite color palettes
 * until it has a given state.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_fade_to_action : public to_template_action<fixed, sprite_palettes_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_fade_to_action(int duration_updates, fixed final_intensity) :
        to_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
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
 * @brief Modifies the intensity of the fade effect applied to all sprite color palettes
 * from a minimum to a maximum. When the intensity is equal to the given final state,
 * it goes back to its initial state and vice versa.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_fade_loop_action : public loop_template_action<fixed, sprite_palettes_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_fade_loop_action(int duration_updates, fixed final_intensity) :
        loop_template_action(duration_updates, final_intensity)
    {
        BN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the intensity of the fade effect applied to all sprite color palettes
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palettes_fade_toggle_action : public toggle_template_action<fixed, sprite_palettes_fade_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the intensity.
     * @param new_intensity New intensity when the action is updated duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palettes_fade_toggle_action(int duration_updates, fixed new_intensity) :
        toggle_template_action(duration_updates, new_intensity)
    {
        BN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the intensity when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};

}

#endif
