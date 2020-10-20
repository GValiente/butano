/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_FIRST_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H
#define BTN_SPRITE_FIRST_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H

#include "btn_any.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprites_manager.h"
#include "btn_display_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

class sprite_first_attributes_hblank_effect_handler
{

public:
    static void setup_target(int, iany& target_last_value)
    {
        target_last_value = last_value_type();
    }

    [[nodiscard]] static bool target_visible(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return sprites_manager::hw_id(handle).has_value();
    }

    [[nodiscard]] static bool target_updated(int target_id, iany& target_last_value)
    {
        last_value_type& last_value = target_last_value.value<last_value_type>();
        last_value_type new_value(target_id);
        bool updated = last_value != new_value;
        last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::sprites::first_attributes_register(*sprites_manager::hw_id(handle));
    }

    static void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        const last_value_type& last_value = target_last_value.value<last_value_type>();
        auto sprite_first_attributes_ptr = reinterpret_cast<const sprite_first_attributes*>(input_values_ptr);
        sprites_manager::fill_hblank_effect_first_attributes(
                    last_value.hw_y, btn::sprite_shape(last_value.shape),
                    btn::palette_bpp_mode(last_value.bpp_mode), last_value.affine_mode,
                    sprite_first_attributes_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        sprites_manager::reload(handle);
    }

private:
    class alignas(int) last_value_type
    {

    public:
        int hw_y = -1;
        int affine_mode = -1;
        uint8_t shape = 0;
        uint8_t bpp_mode = 0;
        bool fade_enabled = false;

        last_value_type() = default;

        explicit last_value_type(void* handle) :
            hw_y(sprites_manager::hw_position(handle).y()),
            affine_mode(sprites_manager::affine_mode(handle)),
            shape(uint8_t(sprites_manager::shape(handle))),
            bpp_mode(uint8_t(sprites_manager::palette(handle).bpp_mode())),
            fade_enabled(display_manager::blending_fade_enabled())
        {
        }

        explicit last_value_type(int target_id) :
            last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;
    };
};

}

#endif
