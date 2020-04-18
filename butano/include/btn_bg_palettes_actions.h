#ifndef BTN_BG_PALETTES_ACTIONS_H
#define BTN_BG_PALETTES_ACTIONS_H

#include "btn_fixed.h"
#include "btn_bg_palettes.h"
#include "btn_template_actions.h"

namespace btn
{

// brightness

class bg_palettes_brightness_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bg_palettes::brightness();
    }

    static void set(fixed brightness)
    {
        bg_palettes::set_brightness(brightness);
    }
};


class bg_palettes_brightness_to_action : public to_template_action<fixed, bg_palettes_brightness_manager>
{

public:
    bg_palettes_brightness_to_action(int duration_frames, fixed final_brightness) :
        to_template_action(duration_frames, final_brightness)
    {
        BTN_ASSERT(final_brightness >= 0 && final_brightness <= 1, "Invalid final brightness: ", final_brightness);
    }

    [[nodiscard]] fixed final_brightness() const
    {
        return final_property();
    }
};


class bg_palettes_brightness_loop_action : public loop_template_action<fixed, bg_palettes_brightness_manager>
{

public:
    bg_palettes_brightness_loop_action(int duration_frames, fixed final_brightness) :
        loop_template_action(duration_frames, final_brightness)
    {
        BTN_ASSERT(final_brightness >= 0 && final_brightness <= 1, "Invalid final brightness: ", final_brightness);
    }

    [[nodiscard]] fixed final_brightness() const
    {
        return final_property();
    }
};


class bg_palettes_brightness_toggle_action : public toggle_template_action<fixed, bg_palettes_brightness_manager>
{

public:
    bg_palettes_brightness_toggle_action(int duration_frames, fixed new_brightness) :
        toggle_template_action(duration_frames, new_brightness)
    {
        BTN_ASSERT(new_brightness >= 0 && new_brightness <= 1, "Invalid new brightness: ", new_brightness);
    }

    [[nodiscard]] fixed new_brightness() const
    {
        return new_property();
    }
};


// contrast

class bg_palettes_contrast_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bg_palettes::contrast();
    }

    static void set(fixed contrast)
    {
        bg_palettes::set_contrast(contrast);
    }
};


class bg_palettes_contrast_to_action : public to_template_action<fixed, bg_palettes_contrast_manager>
{

public:
    bg_palettes_contrast_to_action(int duration_frames, fixed final_contrast) :
        to_template_action(duration_frames, final_contrast)
    {
        BTN_ASSERT(final_contrast >= 0 && final_contrast <= 1, "Invalid final contrast: ", final_contrast);
    }

    [[nodiscard]] fixed final_contrast() const
    {
        return final_property();
    }
};


class bg_palettes_contrast_loop_action : public loop_template_action<fixed, bg_palettes_contrast_manager>
{

public:
    bg_palettes_contrast_loop_action(int duration_frames, fixed final_contrast) :
        loop_template_action(duration_frames, final_contrast)
    {
        BTN_ASSERT(final_contrast >= 0 && final_contrast <= 1, "Invalid final contrast: ", final_contrast);
    }

    [[nodiscard]] fixed final_contrast() const
    {
        return final_property();
    }
};


class bg_palettes_contrast_toggle_action : public toggle_template_action<fixed, bg_palettes_contrast_manager>
{

public:
    bg_palettes_contrast_toggle_action(int duration_frames, fixed new_contrast) :
        toggle_template_action(duration_frames, new_contrast)
    {
        BTN_ASSERT(new_contrast >= 0 && new_contrast <= 1, "Invalid new contrast: ", new_contrast);
    }

    [[nodiscard]] fixed new_contrast() const
    {
        return new_property();
    }
};


// intensity

class bg_palettes_intensity_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bg_palettes::intensity();
    }

    static void set(fixed intensity)
    {
        bg_palettes::set_intensity(intensity);
    }
};


class bg_palettes_intensity_to_action : public to_template_action<fixed, bg_palettes_intensity_manager>
{

public:
    bg_palettes_intensity_to_action(int duration_frames, fixed final_intensity) :
        to_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_intensity_loop_action : public loop_template_action<fixed, bg_palettes_intensity_manager>
{

public:
    bg_palettes_intensity_loop_action(int duration_frames, fixed final_intensity) :
        loop_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_intensity_toggle_action : public toggle_template_action<fixed, bg_palettes_intensity_manager>
{

public:
    bg_palettes_intensity_toggle_action(int duration_frames, fixed new_intensity) :
        toggle_template_action(duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// inverted

class bg_palettes_inverted_manager
{

public:
    [[nodiscard]] static bool get()
    {
        return bg_palettes::inverted();
    }

    static void set(bool inverted)
    {
        bg_palettes::set_inverted(inverted);
    }
};


class bg_palettes_inverted_toggle_action : public bool_toggle_template_action<bg_palettes_inverted_manager>
{

public:
    bg_palettes_inverted_toggle_action(int duration_frames) :
        bool_toggle_template_action(duration_frames)
    {
    }
};


// grayscale

class bg_palettes_grayscale_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bg_palettes::grayscale_intensity();
    }

    static void set(fixed intensity)
    {
        bg_palettes::set_grayscale_intensity(intensity);
    }
};


class bg_palettes_grayscale_to_action : public to_template_action<fixed, bg_palettes_grayscale_manager>
{

public:
    bg_palettes_grayscale_to_action(int duration_frames, fixed final_intensity) :
        to_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_grayscale_loop_action : public loop_template_action<fixed, bg_palettes_grayscale_manager>
{

public:
    bg_palettes_grayscale_loop_action(int duration_frames, fixed final_intensity) :
        loop_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_grayscale_toggle_action : public toggle_template_action<fixed, bg_palettes_grayscale_manager>
{

public:
    bg_palettes_grayscale_toggle_action(int duration_frames, fixed new_intensity) :
        toggle_template_action(duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};


// fade

class bg_palettes_fade_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bg_palettes::fade_intensity();
    }

    static void set(fixed intensity)
    {
        bg_palettes::set_fade_intensity(intensity);
    }
};


class bg_palettes_fade_to_action : public to_template_action<fixed, bg_palettes_fade_manager>
{

public:
    bg_palettes_fade_to_action(int duration_frames, fixed final_intensity) :
        to_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_fade_loop_action : public loop_template_action<fixed, bg_palettes_fade_manager>
{

public:
    bg_palettes_fade_loop_action(int duration_frames, fixed final_intensity) :
        loop_template_action(duration_frames, final_intensity)
    {
        BTN_ASSERT(final_intensity >= 0 && final_intensity <= 1, "Invalid final intensity: ", final_intensity);
    }

    [[nodiscard]] fixed final_intensity() const
    {
        return final_property();
    }
};


class bg_palettes_fade_toggle_action : public toggle_template_action<fixed, bg_palettes_fade_manager>
{

public:
    bg_palettes_fade_toggle_action(int duration_frames, fixed new_intensity) :
        toggle_template_action(duration_frames, new_intensity)
    {
        BTN_ASSERT(new_intensity >= 0 && new_intensity <= 1, "Invalid new intensity: ", new_intensity);
    }

    [[nodiscard]] fixed new_intensity() const
    {
        return new_property();
    }
};

}

#endif
