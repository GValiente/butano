/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_VERTICAL_POSITION_HBLANK_EFFECT_HANDLER_H
#define BN_SPRITE_VERTICAL_POSITION_HBLANK_EFFECT_HANDLER_H

#include "bn_any.h"
#include "bn_sprites_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_vertical_position_hblank_effect_handler
{

public:
    static void setup_target(int, iany& target_last_value)
    {
        target_last_value = 0;
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int target_id, iany& target_last_value)
    {
        int& last_value = target_last_value.value<int>();
        auto handle = reinterpret_cast<void*>(target_id);
        int new_value = sprites_manager::hw_position(handle).y();
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::first_attributes_register(*sprites_manager::hw_id(handle));
    }

    static void write_output_values(int target_id, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        int last_value = target_last_value.value<int>();
        auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_vertical_positions(handle, last_value, fixed_values_ptr,
                                                               output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        sprites_manager::reload(handle);
    }
};

}

#endif
