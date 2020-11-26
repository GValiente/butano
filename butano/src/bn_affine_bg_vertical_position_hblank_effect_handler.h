/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_VERTICAL_POSITION_HBLANK_EFFECT_HANDLER_H
#define BN_AFFINE_BG_VERTICAL_POSITION_HBLANK_EFFECT_HANDLER_H

#include "bn_any.h"
#include "bn_fixed_point.h"
#include "bn_bgs_manager.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class affine_bg_vertical_position_hblank_effect_handler
{

public:
    static void setup_target(int, iany& target_last_value)
    {
        target_last_value = 0;
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int target_id, iany& target_last_value)
    {
        int& last_value = target_last_value.value<int>();
        auto handle = reinterpret_cast<void*>(target_id);
        int new_value = bgs_manager::hw_position(handle).y();
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::bgs::affine_vertical_position_register(*bgs_manager::hw_id(handle));
    }

    static void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        int last_value = target_last_value.value<int>();
        auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
        bgs_manager::fill_hblank_effect_affine_positions(last_value, fixed_values_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        bgs_manager::reload();
    }
};

}

#endif
