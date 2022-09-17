/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_THIRD_ATTRIBUTES_HBE_HANDLER_H
#define BN_SPRITE_THIRD_ATTRIBUTES_HBE_HANDLER_H

#include "bn_any.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprites_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_third_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, iany& target_last_value)
    {
        target_last_value = sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, iany& target_last_value)
    {
        sprite_shape_size& last_value = target_last_value.value<sprite_shape_size>();
        auto handle = reinterpret_cast<void*>(target_id);
        sprite_shape_size new_value = sprites_manager::shape_size(handle);
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::third_attributes_register(sprites_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        const sprite_shape_size& shape_size = target_last_value.value<sprite_shape_size>();
        auto sprite_third_attributes_ptr = reinterpret_cast<const sprite_third_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_third_attributes(
                    shape_size, sprite_third_attributes_ptr, output_values_ptr);
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
