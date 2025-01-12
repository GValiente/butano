/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_ATTRIBUTES_HBE_HANDLER_H
#define BN_AFFINE_BG_ATTRIBUTES_HBE_HANDLER_H

#include "bn_affine_bg_tiles_ptr.h"
#include "bn_affine_bg_attributes.h"
#include "bn_bgs_manager.h"
#include "../hw/include/bn_hw_bgs.h"

namespace bn
{

class affine_bg_attributes_hbe_handler
{

public:
    static void setup_target(intptr_t target_id, void* target_last_value)
    {
        ::new(static_cast<void*>(target_last_value)) last_value_type(target_id);
    }

    [[nodiscard]] static bool target_visible(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return bgs_manager::hw_id(handle) >= 0;
    }

    [[nodiscard]] static bool target_updated(intptr_t target_id, void* target_last_value)
    {
        auto last_value = reinterpret_cast<last_value_type*>(target_last_value);
        last_value_type new_value = last_value_type(target_id);
        bool updated = *last_value != new_value;
        *last_value = new_value;
        return updated;
    }

    [[nodiscard]] static uint16_t* output_register(intptr_t target_id)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        return hw::bgs::attributes_register(bgs_manager::hw_id(handle));
    }

    static void write_output_values(intptr_t target_id, const void*, const void* input_values_ptr,
                                    uint16_t* output_values_ptr)
    {
        auto handle = reinterpret_cast<void*>(target_id);
        auto affine_bg_attributes_ptr = reinterpret_cast<const affine_bg_attributes*>(input_values_ptr);
        bgs_manager::fill_hblank_effect_affine_attributes(handle, affine_bg_attributes_ptr, output_values_ptr);
    }

    static void show(intptr_t)
    {
    }

    static void cleanup(intptr_t)
    {
        bgs_manager::reload();
    }

private:
    class last_value_type
    {

    public:
        explicit last_value_type(const affine_bg_map_ptr& target_map_ptr) :
            _map_dimensions(target_map_ptr.dimensions()),
            _tiles_cbb(target_map_ptr.tiles().cbb())
        {
        }

        explicit last_value_type(void* handle) :
            last_value_type(bgs_manager::affine_map(handle))
        {
        }

        explicit last_value_type(intptr_t target_id) :
            last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;

    private:
        size _map_dimensions;
        int _tiles_cbb;
    };
};

}

#endif
