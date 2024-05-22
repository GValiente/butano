/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_MAT_PC_REGISTER_HBE_HANDLER_H
#define BN_SPRITE_AFFINE_MAT_PC_REGISTER_HBE_HANDLER_H

#include "bn_display.h"
#include "bn_sprite_affine_mats_manager.h"
#include "../hw/include/bn_hw_memory.h"
#include "../hw/include/bn_hw_sprite_affine_mats.h"

namespace bn
{

class sprite_affine_mat_pc_register_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        int16_t* result = hw::sprite_affine_mats::pc_register(target_id);
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const affine_mat_attributes*>(input_values_ptr);

        for(int index = 0; index < display::height(); ++index)
        {
            output_values_ptr[index] = uint16_t(attributes_ptr[index].pc_register_value());
        }
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t target_id)
    {
        sprite_affine_mats_manager::reload(target_id);
    }
};


class sprite_affine_mat_pc_register_values_hbe_handler
{

public:
    static void setup_target(intptr_t, void*)
    {
    }

    [[nodiscard]] static bool target_visible(intptr_t)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(intptr_t, void*)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        int16_t* result = hw::sprite_affine_mats::pc_register(target_id);
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(intptr_t, const void*, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto int_source = static_cast<const unsigned*>(input_values_ptr);
        auto int_destination = reinterpret_cast<unsigned*>(output_values_ptr);
        hw::memory::copy_words(int_source, display::height() / 2, int_destination);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t target_id)
    {
        sprite_affine_mats_manager::reload(target_id);
    }
};

}

#endif
