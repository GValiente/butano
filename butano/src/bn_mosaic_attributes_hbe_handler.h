/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MOSAIC_ATTRIBUTES_HBE_HANDLER_H
#define BN_MOSAIC_ATTRIBUTES_HBE_HANDLER_H

#include "bn_display_manager.h"
#include "bn_mosaic_attributes.h"
#include "../hw/include/bn_hw_display.h"

namespace bn
{

class mosaic_attributes_hbe_handler
{

public:
    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t)
    {
        return hw::display::mosaic_register();
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const mosaic_attributes*>(input_values_ptr);
        display_manager::fill_mosaic_hblank_effect_attributes(attributes_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        display_manager::reload_mosaic();
    }
};

}

#endif
