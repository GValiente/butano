/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_HORIZONTAL_POSITION_HBE_HANDLER_H
#define BN_REGULAR_BG_HORIZONTAL_POSITION_HBE_HANDLER_H

#include "bn_bgs_manager.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class regular_bg_horizontal_position_hbe_handler
{

public:
    static void setup_target(intptr_t, void* target_last_value)
    {
        ::new(static_cast<void*>(target_last_value)) int(0);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<int*>(target_last_value);
        auto handle = reinterpret_cast<void*>(target_id);
        int new_value = bgs_manager::hw_position(handle).x();
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::bgs::regular_horizontal_position_register(bgs_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t, const void* target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto last_value = reinterpret_cast<const int*>(target_last_value);
        auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
        bgs_manager::fill_hblank_effect_regular_positions(*last_value, fixed_values_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        bgs_manager::reload();
    }
};

}

#endif
