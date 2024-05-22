/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_THIRD_ATTRIBUTES_HBE_HANDLER_H
#define BN_SPRITE_THIRD_ATTRIBUTES_HBE_HANDLER_H

#include "bn_sprite_third_attributes.h"
#include "bn_sprites_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_third_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, void* target_last_value)
    {
        new(target_last_value) sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<sprite_shape_size*>(target_last_value);
        auto handle = reinterpret_cast<void*>(target_id);
        sprite_shape_size new_value = sprites_manager::shape_size(handle);
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::third_attributes_register(sprites_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t, const void* target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto last_value = reinterpret_cast<const sprite_shape_size*>(target_last_value);
        auto sprite_third_attributes_ptr = reinterpret_cast<const sprite_third_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_third_attributes(
                    *last_value, sprite_third_attributes_ptr, output_values_ptr);
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
