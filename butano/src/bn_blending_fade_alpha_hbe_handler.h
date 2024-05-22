/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BLENDING_FADE_ALPHA_HBE_HANDLER_H
#define BN_BLENDING_FADE_ALPHA_HBE_HANDLER_H

#include "bn_display_manager.h"
#include "bn_blending_fade_alpha.h"
#include "../hw/include/bn_hw_display.h"

namespace bn
{

class blending_fade_alpha_hbe_handler
{

public:
    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        BN_BASIC_ASSERT(display_manager::blending_transparency_top_weight() == 1 &&
                        display_manager::blending_transparency_bottom_weight() == 0,
                        "Transparency and fade blendings can't be enabled at the same time");

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t)
    {
        return hw::display::blending_fade_register();
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto values_ptr = reinterpret_cast<const blending_fade_alpha*>(input_values_ptr);
        display_manager::fill_blending_fade_hblank_effect_alphas(values_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
        display_manager::set_blending_fade_enabled(true);
    }

    static void cleanup(intptr_t)
    {
        display_manager::reload_blending_fade();
    }
};

}

#endif
