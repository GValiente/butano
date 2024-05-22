/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_PALETTES_TRANSPARENT_COLOR_HBE_HANDLER_H
#define BN_BG_PALETTES_TRANSPARENT_COLOR_HBE_HANDLER_H

#include "bn_palettes_bank.h"
#include "bn_palettes_manager.h"
#include "../hw/include/bn_hw_palettes.h"

namespace bn
{

class bg_palettes_transparent_color_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        const palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();

        if(bg_palettes_bank.global_effects_updated())
        {
            return true;
        }

        palettes_bank::commit_data commit_data = bg_palettes_bank.retrieve_commit_data();

        if(commit_data.colors_ptr)
        {
            return commit_data.offset == 0;
        }

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t)
    {
        return hw::palettes::bg_transparent_color_register();
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        palettes_manager::bg_palettes_bank().fill_hblank_effect_colors(
                    reinterpret_cast<const color*>(input_values_ptr), output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        palettes_manager::bg_palettes_bank().reload_transparent_color();
    }
};

}

#endif
