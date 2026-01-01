/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BITMAP_BG_ATTRIBUTES_HBE_HANDLER_H
#define BN_BITMAP_BG_ATTRIBUTES_HBE_HANDLER_H

#include "bn_bitmap_bg_attributes.h"
#include "bn_bgs_manager.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class bitmap_bg_attributes_hbe_handler
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
        return hw::bgs::attributes_register(bgs_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t target_id, const void*, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        auto bitmap_bg_attributes_ptr = reinterpret_cast<const bitmap_bg_attributes*>(input_values_ptr);
        bgs_manager::fill_hblank_effect_bitmap_attributes(handle, bitmap_bg_attributes_ptr, output_values_ptr);
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
