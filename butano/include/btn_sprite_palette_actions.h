#ifndef BTN_SPRITE_PALETTE_ACTIONS_H
#define BTN_SPRITE_PALETTE_ACTIONS_H

#include "btn_fixed.h"
#include "btn_algorithm.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// inverted

class sprite_palette_inverted_manager
{

public:
    [[nodiscard]] static bool get(const sprite_palette_ptr& palette)
    {
        return palette.inverted();
    }

    static void set(bool inverted, sprite_palette_ptr& palette)
    {
        palette.set_inverted(inverted);
    }
};


class sprite_palette_inverted_toggle_action :
        public bool_toggle_value_template_action<sprite_palette_ptr, sprite_palette_inverted_manager>
{

public:
    sprite_palette_inverted_toggle_action(const sprite_palette_ptr& palette, int duration_frames) :
        bool_toggle_value_template_action(palette, duration_frames)
    {
    }

    sprite_palette_inverted_toggle_action(sprite_palette_ptr&& palette, int duration_frames) :
        bool_toggle_value_template_action(move(palette), duration_frames)
    {
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }
};


// grayscale

class sprite_palette_grayscale_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.grayscale_intensity();
    }

    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_grayscale_intensity(intensity);
    }
};


class sprite_palette_grayscale_to_action :
        public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    sprite_palette_grayscale_to_action(const sprite_palette_ptr& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(palette, duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    sprite_palette_grayscale_to_action(sprite_palette_ptr&& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(move(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class sprite_palette_grayscale_loop_action :
        public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    sprite_palette_grayscale_loop_action(const sprite_palette_ptr& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(palette, duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    sprite_palette_grayscale_loop_action(sprite_palette_ptr&& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class sprite_palette_grayscale_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>
{

public:
    sprite_palette_grayscale_toggle_action(const sprite_palette_ptr& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(palette, duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    sprite_palette_grayscale_toggle_action(sprite_palette_ptr&& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// fade

class sprite_palette_fade_manager
{

public:
    [[nodiscard]] static fixed get(const sprite_palette_ptr& palette)
    {
        return palette.fade_intensity();
    }

    static void set(fixed intensity, sprite_palette_ptr& palette)
    {
        palette.set_fade_intensity(intensity);
    }
};


class sprite_palette_fade_to_action : public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    sprite_palette_fade_to_action(const sprite_palette_ptr& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(palette, duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    sprite_palette_fade_to_action(sprite_palette_ptr&& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(move(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class sprite_palette_fade_loop_action : public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    sprite_palette_fade_loop_action(const sprite_palette_ptr& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(palette, duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    sprite_palette_fade_loop_action(sprite_palette_ptr&& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(move(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class sprite_palette_fade_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>
{

public:
    sprite_palette_fade_toggle_action(const sprite_palette_ptr& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(palette, duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    sprite_palette_fade_toggle_action(sprite_palette_ptr&& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(move(palette), duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// rotate

class sprite_palette_rotate_manager
{

public:
    [[nodiscard]] static int get(const sprite_palette_ptr& palette)
    {
        return palette.rotate_count();
    }

    static void set(int count, sprite_palette_ptr& palette)
    {
        palette.set_rotate_count(count);
    }
};


class sprite_palette_rotate_by_action :
        public cyclic_duration_by_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    sprite_palette_rotate_by_action(const sprite_palette_ptr& palette, int duration_frames, int delta_count) :
        cyclic_duration_by_value_template_action(palette, duration_frames, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    sprite_palette_rotate_by_action(sprite_palette_ptr&& palette, int duration_frames, int delta_count) :
        cyclic_duration_by_value_template_action(move(palette), duration_frames, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }
};


class sprite_palette_rotate_to_action :
        public to_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    sprite_palette_rotate_to_action(const sprite_palette_ptr& palette, int duration_frames, int final_count) :
        to_value_template_action(palette, duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    sprite_palette_rotate_to_action(sprite_palette_ptr&& palette, int duration_frames, int final_count) :
        to_value_template_action(move(palette), duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


class sprite_palette_rotate_loop_action :
        public loop_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    sprite_palette_rotate_loop_action(const sprite_palette_ptr& palette, int duration_frames, int final_count) :
        loop_value_template_action(palette, duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    sprite_palette_rotate_loop_action(sprite_palette_ptr&& palette, int duration_frames, int final_count) :
        loop_value_template_action(move(palette), duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < this->palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", this->palette().colors_count());
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


class sprite_palette_rotate_toggle_action :
        public toggle_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>
{

public:
    sprite_palette_rotate_toggle_action(const sprite_palette_ptr& palette, int duration_frames, int new_count) :
        toggle_value_template_action(palette, duration_frames, new_count)
    {
        BTN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    sprite_palette_rotate_toggle_action(sprite_palette_ptr&& palette, int duration_frames, int new_count) :
        toggle_value_template_action(move(palette), duration_frames, new_count)
    {
        BTN_ASSERT(abs(new_count) < this->palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", this->palette().colors_count());
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int new_count() const
    {
        return new_property();
    }
};

}

#endif
