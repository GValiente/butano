/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BLENDING_FADE_ALPHA_HBE_HANDLER_H
#define BN_BLENDING_FADE_ALPHA_HBE_HANDLER_H

#include "bn_any_fwd.h"
#include "bn_display_manager.h"
#include "bn_blending_fade_alpha.h"
#include "../hw/include/bn_hw_display.h"

namespace bn
{

class blending_fade_alpha_hbe_handler
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
        BN_ASSERT(display_manager::blending_transparency_alpha() == 1,
                   "Transparency and fade blendings can't be enabled at the same time");
        BN_ASSERT(display_manager::blending_intensity_alpha() == 0,
                   "Intensity and fade blendings can't be enabled at the same time");

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
