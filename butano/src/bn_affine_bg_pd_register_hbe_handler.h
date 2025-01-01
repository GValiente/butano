/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_PD_REGISTER_HBE_HANDLER_H
#define BN_AFFINE_BG_PD_REGISTER_HBE_HANDLER_H

#include "bn_display.h"
#include "bn_bgs_manager.h"
#include "bn_affine_bg_mat_attributes.h"
#include "../hw/include/bn_hw_bgs.h"
#include "../hw/include/bn_hw_memory.h"

namespace bn
{

class affine_bg_pd_register_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int hw_id = bgs_manager::hw_id(handle);
        int16_t* result = &hw::bgs::affine_mat_register(hw_id)->pd;
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const affine_bg_mat_attributes*>(input_values_ptr);

        for(int index = 0; index < display::height(); ++index)
        {
            output_values_ptr[index] = uint16_t(attributes_ptr[index].pd_register_value());
        }
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        bgs_manager::reload();
    }
};


class affine_bg_pd_register_values_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int hw_id = bgs_manager::hw_id(handle);
        int16_t* result = &hw::bgs::affine_mat_register(hw_id)->pd;
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto int_source = static_cast<const unsigned*>(input_values_ptr);
        auto int_destination = reinterpret_cast<unsigned*>(output_values_ptr);
        hw::memory::copy_words(int_source, display::height() / 2, int_destination);
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
