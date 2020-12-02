/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_MAT_PC_REGISTER_HBE_HANDLER_H
#define BN_SPRITE_AFFINE_MAT_PC_REGISTER_HBE_HANDLER_H

#include "bn_memory.h"
#include "bn_display.h"
#include "bn_any_fwd.h"
#include "bn_sprite_affine_mats_manager.h"
#include "../hw/include/bn_hw_sprite_affine_mats.h"

namespace bn
{

class sprite_affine_mat_pc_register_attributes_hbe_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        int16_t* result = hw::sprite_affine_mats::pc_register(target_id);
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const affine_mat_attributes*>(input_values_ptr);

        for(int index = 0; index < display::height(); ++index)
        {
            output_values_ptr[index] = uint16_t(attributes_ptr[index].pc_register_value());
        }
    }

    static void show(int)
    {
    }

    static void cleanup(int target_id)
    {
        sprite_affine_mats_manager::reload(target_id);
    }
};


class sprite_affine_mat_pc_register_values_hbe_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int target_id)
    {
        int16_t* result = hw::sprite_affine_mats::pc_register(target_id);
        return reinterpret_cast<uint16_t*>(result);
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto int_source = static_cast<const unsigned*>(input_values_ptr);
        auto int_destination = reinterpret_cast<unsigned*>(output_values_ptr);
        memory::copy(*int_source, display::height() / 2, *int_destination);
    }

    static void show(int)
    {
    }

    static void cleanup(int target_id)
    {
        sprite_affine_mats_manager::reload(target_id);
    }
};

}

#endif
