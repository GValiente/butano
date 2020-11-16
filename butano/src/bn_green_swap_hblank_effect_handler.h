/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GREEN_SWAP_HBLANK_EFFECT_HANDLER_H
#define BN_GREEN_SWAP_HBLANK_EFFECT_HANDLER_H

#include "bn_any_fwd.h"
#include "bn_display_manager.h"
#include "../hw/include/bn_hw_display.h"

namespace bn
{

class green_swap_hblank_effect_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int)
    {
        return hw::display::green_swap_register();
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto states_ptr = reinterpret_cast<const bool*>(input_values_ptr);
        display_manager::fill_green_swap_hblank_effect_states(states_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        display_manager::reload_green_swap();
    }
};

}

#endif
