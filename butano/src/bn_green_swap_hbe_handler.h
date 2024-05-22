/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GREEN_SWAP_HBE_HANDLER_H
#define BN_GREEN_SWAP_HBE_HANDLER_H

#include "bn_display_manager.h"
#include "../hw/include/bn_hw_display.h"

namespace bn
{

class green_swap_hbe_handler
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
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t)
    {
        return hw::display::green_swap_register();
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto states_ptr = reinterpret_cast<const bool*>(input_values_ptr);
        display_manager::fill_green_swap_hblank_effect_states(states_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        display_manager::reload_green_swap();
    }
};

}

#endif
