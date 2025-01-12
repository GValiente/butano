/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_PIVOT_HORIZONTAL_POSITION_HBE_HANDLER_H
#define BN_AFFINE_BG_PIVOT_HORIZONTAL_POSITION_HBE_HANDLER_H

#include "bn_bgs_manager.h"
#include "bn_affine_bg_mat_attributes.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class affine_bg_pivot_horizontal_position_hbe_handler
{

public:
    static void setup_target(intptr_t target_id, void* target_last_value)
    {
        ::new(static_cast<void*>(target_last_value)) last_value_type(target_id);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<last_value_type*>(target_last_value);
        last_value_type new_value = last_value_type(target_id);
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int hw_id = bgs_manager::hw_id(handle);
        int* result = &hw::bgs::affine_mat_register(hw_id)->dx;
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(intptr_t target_id, const void*, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
        auto result_ptr = reinterpret_cast<unsigned*>(output_values_ptr);
        bgs_manager::fill_hblank_effect_pivot_horizontal_positions(handle, fixed_values_ptr, result_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        bgs_manager::reload();
    }

private:
    class last_value_type
    {

    public:
        explicit last_value_type(const affine_bg_mat_attributes& mat_attributes) :
            _dx(mat_attributes.dx_register_value()),
            _pa(mat_attributes.pa_register_value()),
            _pb(mat_attributes.pb_register_value())
        {
        }

        explicit last_value_type(void* handle) :
            last_value_type(bgs_manager::mat_attributes(handle))
        {
        }

        explicit last_value_type(intptr_t target_id) :
            last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;

    private:
        int _dx;
        int _pa;
        int _pb;
    };
};

}

#endif
