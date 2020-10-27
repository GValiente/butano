/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H
#define BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H

#include "btn_any.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_second_attributes_last_value.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

class sprite_affine_second_attributes_hblank_effect_handler
{

public:
    static void setup_target(int, iany& target_last_value)
    {
        target_last_value = sprite_second_attributes_last_value();
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int target_id, iany& target_last_value)
    {
        BTN_ASSERT(sprites_manager::affine_mat(reinterpret_cast<void*>(target_id)), "Sprite is not affine");

        sprite_second_attributes_last_value& last_value = target_last_value.value<sprite_second_attributes_last_value>();
        sprite_second_attributes_last_value new_value(target_id);
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::second_attributes_register(*sprites_manager::hw_id(handle));
    }

    static void write_output_values(int target_id, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        const sprite_second_attributes_last_value& last_value = target_last_value.value<sprite_second_attributes_last_value>();
        auto second_attributes_ptr = reinterpret_cast<const sprite_affine_second_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_affine_second_attributes(
                    handle, last_value.hw_x, last_value.size, second_attributes_ptr, output_values_ptr);
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
