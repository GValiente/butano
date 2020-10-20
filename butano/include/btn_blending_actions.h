/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_ACTIONS_H
#define BTN_BLENDING_ACTIONS_H

#include "btn_fixed.h"
#include "btn_blending.h"
#include "btn_template_actions.h"

namespace btn
{

// transparency_alpha

class blending_transparency_alpha_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return blending::transparency_alpha();
    }

    static void set(fixed transparency_alpha)
    {
        blending::set_transparency_alpha(transparency_alpha);
    }
};


class blending_transparency_alpha_to_action : public to_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    blending_transparency_alpha_to_action(int duration_frames, fixed final_transparency_alpha) :
        to_template_action(duration_frames, final_transparency_alpha)
    {
        BTN_ASSERT(final_transparency_alpha >= 0 && final_transparency_alpha <= 1,
                   "Invalid final transparency alpha: ", final_transparency_alpha);
    }

    [[nodiscard]] fixed final_transparency_alpha() const
    {
        return final_property();
    }
};


class blending_transparency_alpha_loop_action :
        public loop_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    blending_transparency_alpha_loop_action(int duration_frames, fixed final_transparency_alpha) :
        loop_template_action(duration_frames, final_transparency_alpha)
    {
        BTN_ASSERT(final_transparency_alpha >= 0 && final_transparency_alpha <= 1,
                   "Invalid final transparency alpha: ", final_transparency_alpha);
    }

    [[nodiscard]] fixed final_transparency_alpha() const
    {
        return final_property();
    }
};


class blending_transparency_alpha_toggle_action :
        public toggle_template_action<fixed, blending_transparency_alpha_manager>
{

public:
    blending_transparency_alpha_toggle_action(int duration_frames, fixed new_transparency_alpha) :
        toggle_template_action(duration_frames, new_transparency_alpha)
    {
        BTN_ASSERT(new_transparency_alpha >= 0 && new_transparency_alpha <= 1,
                   "Invalid new transparency alpha: ", new_transparency_alpha);
    }

    [[nodiscard]] fixed new_transparency_alpha() const
    {
        return new_property();
    }
};


// intensity_alpha

class blending_intensity_alpha_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return blending::intensity_alpha();
    }

    static void set(fixed intensity_alpha)
    {
        blending::set_intensity_alpha(intensity_alpha);
    }
};


class blending_intensity_alpha_to_action : public to_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    blending_intensity_alpha_to_action(int duration_frames, fixed final_intensity_alpha) :
        to_template_action(duration_frames, final_intensity_alpha)
    {
        BTN_ASSERT(final_intensity_alpha >= 0 && final_intensity_alpha <= 1,
                   "Invalid final intensity alpha: ", final_intensity_alpha);
    }

    [[nodiscard]] fixed final_intensity_alpha() const
    {
        return final_property();
    }
};


class blending_intensity_alpha_loop_action :
        public loop_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    blending_intensity_alpha_loop_action(int duration_frames, fixed final_intensity_alpha) :
        loop_template_action(duration_frames, final_intensity_alpha)
    {
        BTN_ASSERT(final_intensity_alpha >= 0 && final_intensity_alpha <= 1,
                   "Invalid final intensity alpha: ", final_intensity_alpha);
    }

    [[nodiscard]] fixed final_intensity_alpha() const
    {
        return final_property();
    }
};


class blending_intensity_alpha_toggle_action :
        public toggle_template_action<fixed, blending_intensity_alpha_manager>
{

public:
    blending_intensity_alpha_toggle_action(int duration_frames, fixed new_intensity_alpha) :
        toggle_template_action(duration_frames, new_intensity_alpha)
    {
        BTN_ASSERT(new_intensity_alpha >= 0 && new_intensity_alpha <= 1,
                   "Invalid new intensity alpha: ", new_intensity_alpha);
    }

    [[nodiscard]] fixed new_intensity_alpha() const
    {
        return new_property();
    }
};


// fade_alpha

class blending_fade_alpha_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return blending::fade_alpha();
    }

    static void set(fixed fade_alpha)
    {
        blending::set_fade_alpha(fade_alpha);
    }
};


class blending_fade_alpha_to_action : public to_template_action<fixed, blending_fade_alpha_manager>
{

public:
    blending_fade_alpha_to_action(int duration_frames, fixed final_fade_alpha) :
        to_template_action(duration_frames, final_fade_alpha)
    {
        BTN_ASSERT(final_fade_alpha >= 0 && final_fade_alpha <= 1, "Invalid final fade alpha: ", final_fade_alpha);
    }

    [[nodiscard]] fixed final_fade_alpha() const
    {
        return final_property();
    }
};


class blending_fade_alpha_loop_action :
        public loop_template_action<fixed, blending_fade_alpha_manager>
{

public:
    blending_fade_alpha_loop_action(int duration_frames, fixed final_fade_alpha) :
        loop_template_action(duration_frames, final_fade_alpha)
    {
        BTN_ASSERT(final_fade_alpha >= 0 && final_fade_alpha <= 1, "Invalid final fade alpha: ", final_fade_alpha);
    }

    [[nodiscard]] fixed final_fade_alpha() const
    {
        return final_property();
    }
};


class blending_fade_alpha_toggle_action :
        public toggle_template_action<fixed, blending_fade_alpha_manager>
{

public:
    blending_fade_alpha_toggle_action(int duration_frames, fixed new_fade_alpha) :
        toggle_template_action(duration_frames, new_fade_alpha)
    {
        BTN_ASSERT(new_fade_alpha >= 0 && new_fade_alpha <= 1, "Invalid new fade alpha: ", new_fade_alpha);
    }

    [[nodiscard]] fixed new_fade_alpha() const
    {
        return new_property();
    }
};

}

#endif
