/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBE_HANDLER_H
#define BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBE_HANDLER_H

#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprites_manager.h"
#include "bn_sprite_second_attributes_last_value.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_affine_second_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, void* target_last_value)
    {
        new(target_last_value) sprite_second_attributes_last_value();
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        BN_BASIC_ASSERT(sprites_manager::affine_mat(reinterpret_cast<void*>(target_id)), "Sprite is not affine");

        auto last_value = reinterpret_cast<sprite_second_attributes_last_value*>(target_last_value);
        sprite_second_attributes_last_value new_value(target_id);
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::second_attributes_register(sprites_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t target_id, const void* target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        auto last_value = reinterpret_cast<const sprite_second_attributes_last_value*>(target_last_value);
        auto second_attributes_ptr = reinterpret_cast<const sprite_affine_second_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_affine_second_attributes(
                    handle, last_value->hw_x, last_value->size, second_attributes_ptr, output_values_ptr);
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
