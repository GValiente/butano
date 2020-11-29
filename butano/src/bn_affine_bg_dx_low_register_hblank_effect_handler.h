/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_DX_LOW_REGISTER_HBLANK_EFFECT_HANDLER_H
#define BN_AFFINE_BG_DX_LOW_REGISTER_HBLANK_EFFECT_HANDLER_H

#include "bn_display.h"
#include "bn_any_fwd.h"
#include "bn_bgs_manager.h"
#include "bn_affine_bg_mat_attributes.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class affine_bg_dx_low_register_attributes_hblank_effect_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int hw_id = *bgs_manager::hw_id(handle);
        int* result = &hw::bgs::affine_mat_register(hw_id)->dx;
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const affine_bg_mat_attributes*>(input_values_ptr);
        int pb_sum = 0;

        for(int index = 0; index < display::height(); ++index)
        {
            const affine_bg_mat_attributes& attributes = attributes_ptr[index];
            int result = attributes.dx_register_value() + pb_sum;
            output_values_ptr[index] = uint16_t(result);
            pb_sum += attributes.pb_register_value();
        }
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        bgs_manager::reload();
    }
};


class affine_bg_dx_low_register_values_hblank_effect_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int hw_id = *bgs_manager::hw_id(handle);
        int* result = &hw::bgs::affine_mat_register(hw_id)->dx;
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto values_ptr = reinterpret_cast<const int*>(input_values_ptr);

        for(int index = 0; index < display::height(); ++index)
        {
            output_values_ptr[index] = uint16_t(values_ptr[index]);
        }
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
