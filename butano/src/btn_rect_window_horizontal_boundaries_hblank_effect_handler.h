/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_RECT_WINDOW_HORIZONTAL_BOUNDARIES_HBLANK_EFFECT_HANDLER_H
#define BTN_RECT_WINDOW_HORIZONTAL_BOUNDARIES_HBLANK_EFFECT_HANDLER_H

#include "btn_any.h"
#include "btn_fixed.h"
#include "btn_display_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

class rect_window_horizontal_boundaries_hblank_effect_handler
{

public:
    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    static void setup_target(int, iany& target_last_value)
    {
        target_last_value = pair<int, int>();
    }

    [[nodiscard]] static bool target_updated(int target_id, iany& target_last_value)
    {
        pair<int, int>& last_value = target_last_value.value<pair<int, int>>();
        pair<int, int> new_value = display_manager::rect_window_hw_horizontal_boundaries(target_id);
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        return hw::display::window_horizontal_boundaries_register(target_id);
    }

    static void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        const pair<int, int>& last_value = target_last_value.value<pair<int, int>>();
        auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(input_values_ptr);
        display_manager::fill_rect_window_hblank_effect_horizontal_boundaries(
                    last_value, fixed_pairs_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        display_manager::reload_rect_windows_boundaries();
    }
};

}

#endif
