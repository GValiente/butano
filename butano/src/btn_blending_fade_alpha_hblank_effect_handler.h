/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_HANDLER_H
#define BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_HANDLER_H

#include "btn_any_fwd.h"
#include "btn_display_manager.h"
#include "btn_blending_fade_alpha.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

class blending_fade_alpha_hblank_effect_handler
{

public:
    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
                   "Blending transparency and fade can't be enabled at the same time");
        BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
                   "Blending intensity and fade can't be enabled at the same time");

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int)
    {
        return hw::display::blending_fade_register();
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto values_ptr = reinterpret_cast<const blending_fade_alpha*>(input_values_ptr);
        display_manager::fill_blending_fade_hblank_effect_alphas(values_ptr, output_values_ptr);
    }

    static void show(int)
    {
        display_manager::blending_enable_fade();
    }

    static void cleanup(int)
    {
        display_manager::blending_disable_fade();
        display_manager::reload_blending_transparency();
    }
};

}

#endif
