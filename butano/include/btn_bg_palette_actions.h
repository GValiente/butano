#ifndef BTN_BG_PALETTE_ACTIONS_H
#define BTN_BG_PALETTE_ACTIONS_H

#include "btn_fixed.h"
#include "btn_bg_palette_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

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

}

#endif
