/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PALETTE_ACTIONS_H
#define BTN_SPRITE_PALETTE_ACTIONS_H

#include "btn_fixed.h"
#include "btn_algorithm.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// inverted

/**
 * @brief Manages if the colors of a sprite color palette must be inverted or not.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_inverted_manager
{

public:
    /**
     * @brief Indicates if the colors of the given sprite color palette are inverted or not.
     */
    [[nodiscard]] static bool get(const sprite_palette_ptr& palette)
    {
        return palette.inverted();
    }

    /**
     * @brief Sets if the colors of the given sprite color palette must be inverted or not.
     */
    static void set(bool inverted, sprite_palette_ptr& palette)
    {
        palette.set_inverted(inverted);
    }
};


/**
 * @brief Toggles if the colors of a sprite color palette must be inverted or not
 * when a given amount of update calls are done.
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done to toggle
     * if the colors of the given sprite color palette must be inverted or not.
     */
    sprite_palette_inverted_toggle_action(const sprite_palette_ptr& palette, int duration_updates) :
        bool_toggle_value_template_action(palette, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done to toggle
     * if the colors of the given sprite color palette must be inverted or not.
     */
    sprite_palette_inverted_toggle_action(sprite_palette_ptr&& palette, int duration_updates) :
        bool_toggle_value_template_action(move(palette), duration_updates)
    {
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }
};


// grayscale

/**
 * @brief Manages the intensity of the grayscale effect applied to a sprite color palette.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_grayscale_manager
{

public:
    /**
     * @brief Returns the intensity of the grayscale effect applied to the given sprite color palette.
     */
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.grayscale_intensity();
    }

    /**
     * @brief Sets the intensity of the grayscale effect applied to the given sprite color palette.
     * @param intensity New intensity in the range [0..1].
     * @param palette Sprite color palette to modify.
     */
    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_grayscale_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the grayscale effect applied to a sprite color palette
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates Number of times that update has to be called
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_to_action(const sprite_palette_ptr& palette, int duration_updates,
                                       fixed final_intensity) :
        to_value_template_action(palette, duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates Number of times that update has to be called
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when update is called duration_updates times.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the intensity of the grayscale effect applied to a sprite color palette
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done
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
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
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
 * @brief Changes the intensity of the grayscale effect applied to a sprite color palette
 * when a given amount of update calls are done.
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done to change the intensity.
     * @param new_intensity New intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_toggle_action(const sprite_palette_ptr& palette, int duration_updates,
                                           fixed new_intensity) :
        toggle_value_template_action(palette, duration_updates, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done to change the intensity.
     * @param new_intensity New intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_grayscale_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_updates, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when update is called duration_updates times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// fade

/**
 * @brief Manages the intensity of the fade effect applied to a sprite color palette.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_fade_manager
{

public:
    /**
     * @brief Returns the intensity of the fade effect applied to the given sprite color palette.
     */
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.fade_intensity();
    }

    /**
     * @brief Sets the intensity of the fade effect applied to the given sprite color palette.
     * @param intensity New intensity in the range [0..1].
     * @param palette Sprite color palette to modify.
     */
    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_fade_intensity(intensity);
    }
};


/**
 * @brief Modifies the intensity of the fade effect applied to a sprite color palette
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates Number of times that update has to be called
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_to_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(palette, duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates Number of times that update has to be called
     * until the intensity is equal to final_intensity.
     * @param final_intensity Intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        to_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when update is called duration_updates times.
     */
    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the intensity of the fade effect applied to a sprite color palette
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_loop_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(palette, duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the intensity delta.
     * @param final_intensity When the intensity is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_updates, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
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
 * @brief Changes the intensity of the fade effect applied to a sprite color palette
 * when a given amount of update calls are done.
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done to change the intensity.
     * @param new_intensity New intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_toggle_action(const sprite_palette_ptr& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(palette, duration_updates, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done to change the intensity.
     * @param new_intensity New intensity when update is called duration_updates times.
     *
     * This intensity must be in the range [0..1].
     */
    sprite_palette_fade_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_updates, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the intensity when update is called duration_updates times.
     */
    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// rotate

/**
 * @brief Manages the number of colors to rotate to the right in a sprite color palette.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup action
 */
class sprite_palette_rotate_manager
{

public:
    /**
     * @brief Returns the number of colors to rotate to the right in the given sprite color palette.
     */
    [[nodiscard]] static int get(const sprite_palette_ptr& palette)
    {
        return palette.rotate_count();
    }

    /**
     * @brief Sets the number of colors to rotate to the right in the given sprite color palette.
     * @param count Number of colors to rotate to the right in the range [1 - colors_count() .. colors_count() - 1].
     * @param palette Sprite color palette to modify.
     */
    static void set(int count, sprite_palette_ptr& palette)
    {
        palette.set_rotate_count(count);
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite color palette by delta_count
 * when a given amount of update calls are done. When the property is over the given maximum,
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when update is called duration_updates times.
     */
    sprite_palette_rotate_by_action(const sprite_palette_ptr& palette, int duration_updates, int delta_count) :
        cyclic_duration_by_value_template_action(palette, duration_updates, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done before updating the number of colors to rotate.
     * @param delta_count How much colors to add to the number of colors to rotate to the right
     * when update is called duration_updates times.
     */
    sprite_palette_rotate_by_action(sprite_palette_ptr&& palette, int duration_updates, int delta_count) :
        cyclic_duration_by_value_template_action(move(palette), duration_updates, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns how much colors to add to the number of colors to rotate to the right
     * when update is called duration_updates times.
     */
    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite color palette until it has a given state.
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates Number of times that update has to be called
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right when update is called duration_updates times.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_to_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) :
        to_value_template_action(palette, duration_updates, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates Number of times that update has to be called
     * until the number of colors to rotate to the right is equal to final_count.
     * @param final_count Number of colors to rotate to the right when update is called duration_updates times.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_to_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) :
        to_value_template_action(move(palette), duration_updates, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the number of colors to rotate to the right when update is called duration_updates times.
     */
    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the number of colors to rotate to the right in a sprite color palette
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_loop_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) :
        loop_value_template_action(palette, duration_updates, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done
     * before changing the direction of the number of colors to rotate delta.
     * @param final_count When the the number of colors to rotate to the right is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_loop_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) :
        loop_value_template_action(move(palette), duration_updates, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite color palette to modify.
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
 * @brief Changes the number of colors to rotate to the right in a sprite color palette
 * when a given amount of update calls are done.
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
     * @param palette Sprite color palette to copy.
     * @param duration_updates How much update calls have to be done to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right when update is called duration_updates times.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_toggle_action(const sprite_palette_ptr& palette, int duration_updates, int new_count) :
        toggle_value_template_action(palette, duration_updates, new_count)
    {
        BTN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Constructor.
     * @param palette Sprite color palette to move.
     * @param duration_updates How much update calls have to be done to change
     * the number of colors to rotate to the right.
     * @param new_count New number of colors to rotate to the right when update is called duration_updates times.
     *
     * This count must be in the range [1 - colors_count() .. colors_count() - 1].
     */
    sprite_palette_rotate_toggle_action(sprite_palette_ptr&& palette, int duration_updates, int new_count) :
        toggle_value_template_action(move(palette), duration_updates, new_count)
    {
        BTN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    /**
     * @brief Returns the sprite color palette to modify.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    /**
     * @brief Returns the number of colors to rotate to the right when update is called duration_updates times.
     */
    [[nodiscard]] int new_count() const
    {
        return new_property();
    }
};

}

#endif
