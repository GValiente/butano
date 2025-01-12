/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_FIRST_ATTRIBUTES_HBE_HANDLER_H
#define BN_SPRITE_FIRST_ATTRIBUTES_HBE_HANDLER_H

#include "bn_sprite_first_attributes.h"
#include "bn_sprites_manager.h"
#include "bn_display_manager.h"
#include "../hw/include/bn_hw_sprites.h"

namespace bn
{

class sprite_first_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, void* target_last_value)
    {
        ::new(static_cast<void*>(target_last_value)) last_value_type();
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<last_value_type*>(target_last_value);
        last_value_type new_value(target_id);
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::first_attributes_register(sprites_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t, const void* target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto last_value = reinterpret_cast<const last_value_type*>(target_last_value);
        auto sprite_first_attributes_ptr = reinterpret_cast<const sprite_first_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_first_attributes(
                    last_value->hw_y, bn::sprite_shape(last_value->shape), bpp_mode(last_value->bpp),
                    last_value->view_mode, sprite_first_attributes_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        sprites_manager::reload(handle);
    }

private:
    class last_value_type
    {

    public:
        int hw_y = -1;
        int view_mode = -1;
        uint8_t shape = 0;
        uint8_t bpp = 0;
        bool fade_enabled = false;

        last_value_type() = default;

        explicit last_value_type(void* handle) :
            hw_y(sprites_manager::hw_position(handle).y()),
            view_mode(sprites_manager::view_mode(handle)),
            shape(uint8_t(sprites_manager::shape(handle))),
            bpp(uint8_t(sprites_manager::palette(handle).bpp())),
            fade_enabled(display_manager::blending_fade_enabled())
        {
        }

        explicit last_value_type(intptr_t target_id) :
            last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;
    };
};

}

#endif
