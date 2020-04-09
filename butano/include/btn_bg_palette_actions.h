#ifndef BTN_BG_PALETTE_ACTIONS_H
#define BTN_BG_PALETTE_ACTIONS_H

#include "btn_fixed.h"
#include "btn_algorithm.h"
#include "btn_bg_palette_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// inverted

class bg_palette_inverted_manager
{

public:
    [[nodiscard]] static bool get(const bg_palette_ptr& palette)
    {
        return palette.inverted();
    }

    static void set(bool inverted, bg_palette_ptr& palette)
    {
        palette.set_inverted(inverted);
    }
};


class bg_palette_inverted_toggle_action :
        public bool_toggle_value_template_action<bg_palette_ptr, bg_palette_inverted_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_inverted_toggle_action(BgPalettePtr&& palette, int duration_frames) :
        bool_toggle_value_template_action(forward<BgPalettePtr>(palette), duration_frames)
    {
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }
};


// grayscale

class bg_palette_grayscale_manager
{

public:
    [[nodiscard]] static fixed get(const bg_palette_ptr& palette)
    {
        return palette.grayscale_intensity();
    }

    static void set(fixed intensity, bg_palette_ptr& palette)
    {
        palette.set_grayscale_intensity(intensity);
    }
};


class bg_palette_grayscale_to_action :
        public to_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_grayscale_to_action(BgPalettePtr&& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palette_grayscale_loop_action :
        public loop_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_grayscale_loop_action(BgPalettePtr&& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palette_grayscale_toggle_action :
        public toggle_value_template_action<bg_palette_ptr, fixed, bg_palette_grayscale_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_grayscale_toggle_action(BgPalettePtr&& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(forward<BgPalettePtr>(palette), duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// fade

class bg_palette_fade_manager
{

public:
    [[nodiscard]] static fixed get(const bg_palette_ptr& palette)
    {
        return palette.fade_intensity();
    }

    static void set(fixed intensity, bg_palette_ptr& palette)
    {
        palette.set_fade_intensity(intensity);
    }
};


class bg_palette_fade_to_action : public to_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_fade_to_action(BgPalettePtr&& palette, int duration_frames, fixed final_intensity) :
        to_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palette_fade_loop_action : public loop_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_fade_loop_action(BgPalettePtr&& palette, int duration_frames, fixed final_intensity) :
        loop_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palette_fade_toggle_action :
        public toggle_value_template_action<bg_palette_ptr, fixed, bg_palette_fade_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_fade_toggle_action(BgPalettePtr&& palette, int duration_frames, fixed new_intensity) :
        toggle_value_template_action(forward<BgPalettePtr>(palette), duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// rotate

class bg_palette_rotate_manager
{

public:
    [[nodiscard]] static int get(const bg_palette_ptr& palette)
    {
        return palette.rotate_count();
    }

    static void set(int count, bg_palette_ptr& palette)
    {
        palette.set_rotate_count(count);
    }
};


class bg_palette_rotate_by_action :
        public cyclic_duration_by_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_rotate_by_action(BgPalettePtr&& palette, int duration_frames, int delta_count) :
        cyclic_duration_by_value_template_action(forward<BgPalettePtr>(palette), duration_frames, delta_count, 0, 0)
    {
        set_after_max_property(this->palette().colors_count() - 1);
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int delta_count() const
    {
        return delta_property();
    }
};


class bg_palette_rotate_to_action : public to_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_rotate_to_action(BgPalettePtr&& palette, int duration_frames, int final_count) :
        to_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", palette().colors_count());
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


class bg_palette_rotate_loop_action : public loop_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_rotate_loop_action(BgPalettePtr&& palette, int duration_frames, int final_count) :
        loop_value_template_action(forward<BgPalettePtr>(palette), duration_frames, final_count)
    {
        BTN_ASSERT(abs(final_count) < palette().colors_count() - 1,
                   "Invalid final count: ", final_count, " - ", palette().colors_count());
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return value();
    }

    [[nodiscard]] int final_count() const
    {
        return final_property();
    }
};


class bg_palette_rotate_toggle_action :
        public toggle_value_template_action<bg_palette_ptr, int, bg_palette_rotate_manager>
{

public:
    template<class BgPalettePtr>
    bg_palette_rotate_toggle_action(BgPalettePtr&& palette, int duration_frames, int new_count) :
        toggle_value_template_action(forward<BgPalettePtr>(palette), duration_frames, new_count)
    {
        BTN_ASSERT(abs(new_count) < palette().colors_count() - 1,
                   "Invalid new count: ", new_count, " - ", palette().colors_count());
    }

    [[nodiscard]] const bg_palette_ptr& palette() const
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
