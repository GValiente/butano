/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_PALETTE_COLOR_HBE_HANDLER_H
#define BN_BG_PALETTE_COLOR_HBE_HANDLER_H

#include "bn_palettes_bank.h"
#include "bn_palettes_manager.h"
#include "bn_palette_target_id.h"
#include "../hw/include/bn_hw_palettes.h"

namespace bn
{

class bg_palette_color_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void*)
    {
        const palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();

        if(bg_palettes_bank.global_effects_updated())
        {
            return true;
        }

        palettes_bank::commit_data commit_data = bg_palettes_bank.retrieve_commit_data();

        if(int count = commit_data.count)
        {
            palette_target_id palette_target_id(target_id);
            int target_color = palette_target_id.params.final_color_index;
            int first_color = commit_data.offset;
            int last_color = first_color + count - 1;
            return target_color >= first_color && target_color <= last_color;
        }

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        palette_target_id palette_target_id(target_id);
        return hw::palettes::bg_color_register(palette_target_id.params.final_color_index);
    }

    static void write_output_values(intptr_t target_id, const void*, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        palette_target_id palette_target_id(target_id);
        int palette_id = palette_target_id.params.palette_id;
        palettes_manager::bg_palettes_bank().fill_hblank_effect_colors(
                    palette_id, reinterpret_cast<const color*>(input_values_ptr), output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t target_id)
    {
        palette_target_id palette_target_id(target_id);
        int palette_id = palette_target_id.params.palette_id;
        palettes_manager::bg_palettes_bank().reload(palette_id);
    }
};

}

#endif
