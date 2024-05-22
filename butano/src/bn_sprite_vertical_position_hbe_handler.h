/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_VERTICAL_POSITION_HBE_HANDLER_H
#define BN_SPRITE_VERTICAL_POSITION_HBE_HANDLER_H

#include "bn_sprites_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_vertical_position_hbe_handler
{

public:
    static void setup_target(intptr_t, void* target_last_value)
    {
        new(target_last_value) int(0);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<int*>(target_last_value);
        auto handle = reinterpret_cast<void*>(target_id);
        int new_value = sprites_manager::hw_position(handle).y();
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::first_attributes_register(sprites_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t target_id, const void* target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        auto last_value = reinterpret_cast<const int*>(target_last_value);
        auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_vertical_positions(handle, *last_value, fixed_values_ptr,
                                                               output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        sprites_manager::reload(handle);
    }
};

}

#endif
