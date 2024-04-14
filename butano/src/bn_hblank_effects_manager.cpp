/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hblank_effects_manager.h"

#include "bn_vector.h"
#include "../hw/include/bn_hw_hblank_effects.h"

#include "bn_bg_palette_color_hbe_handler.h"
#include "bn_bg_palettes_transparent_color_hbe_handler.h"
#include "bn_blending_fade_alpha_hbe_handler.h"
#include "bn_blending_transparency_attributes_hbe_handler.h"
#include "bn_green_swap_hbe_handler.h"
#include "bn_mosaic_attributes_hbe_handler.h"
#include "bn_rect_window_horizontal_boundaries_hbe_handler.h"
#include "bn_rect_window_vertical_boundaries_hbe_handler.h"
#include "bn_regular_bg_attributes_hbe_handler.h"
#include "bn_affine_bg_attributes_hbe_handler.h"
#include "bn_regular_bg_horizontal_position_hbe_handler.h"
#include "bn_affine_bg_pivot_horizontal_position_hbe_handler.h"
#include "bn_regular_bg_vertical_position_hbe_handler.h"
#include "bn_affine_bg_pivot_vertical_position_hbe_handler.h"
#include "bn_affine_bg_pa_register_hbe_handler.h"
#include "bn_affine_bg_pb_register_hbe_handler.h"
#include "bn_affine_bg_pc_register_hbe_handler.h"
#include "bn_affine_bg_pd_register_hbe_handler.h"
#include "bn_affine_bg_dx_register_hbe_handler.h"
#include "bn_affine_bg_dy_register_hbe_handler.h"
#include "bn_sprite_affine_mat_pa_register_hbe_handler.h"
#include "bn_sprite_affine_mat_pb_register_hbe_handler.h"
#include "bn_sprite_affine_mat_pc_register_hbe_handler.h"
#include "bn_sprite_affine_mat_pd_register_hbe_handler.h"
#include "bn_sprite_first_attributes_hbe_handler.h"
#include "bn_sprite_regular_second_attributes_hbe_handler.h"
#include "bn_sprite_affine_second_attributes_hbe_handler.h"
#include "bn_sprite_third_attributes_hbe_handler.h"
#include "bn_sprite_horizontal_position_hbe_handler.h"
#include "bn_sprite_vertical_position_hbe_handler.h"
#include "bn_sprite_palette_color_hbe_handler.h"

#include "bn_hbes.cpp.h"
#include "bn_hbe_ptr.cpp.h"
#include "bn_bg_palette_color_hbe_ptr.cpp.h"
#include "bn_bg_palettes_transparent_color_hbe_ptr.cpp.h"
#include "bn_backdrop_color_hbe_ptr.cpp.h"
#include "bn_blending_fade_alpha_hbe_ptr.cpp.h"
#include "bn_blending_transparency_attributes_hbe_ptr.cpp.h"
#include "bn_green_swap_hbe_ptr.cpp.h"
#include "bn_mosaic_attributes_hbe_ptr.cpp.h"
#include "bn_rect_window_boundaries_hbe_ptr.cpp.h"
#include "bn_regular_bg_attributes_hbe_ptr.cpp.h"
#include "bn_affine_bg_attributes_hbe_ptr.cpp.h"
#include "bn_regular_bg_position_hbe_ptr.cpp.h"
#include "bn_affine_bg_pivot_position_hbe_ptr.cpp.h"
#include "bn_affine_bg_pa_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_pb_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_pc_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_pd_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_dx_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_dy_register_hbe_ptr.cpp.h"
#include "bn_affine_bg_mat_attributes_hbe_ptr.cpp.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.cpp.h"
#include "bn_sprite_affine_mat_pa_register_hbe_ptr.cpp.h"
#include "bn_sprite_affine_mat_pb_register_hbe_ptr.cpp.h"
#include "bn_sprite_affine_mat_pc_register_hbe_ptr.cpp.h"
#include "bn_sprite_affine_mat_pd_register_hbe_ptr.cpp.h"
#include "bn_sprite_affine_second_attributes_hbe_ptr.cpp.h"
#include "bn_sprite_first_attributes_hbe_ptr.cpp.h"
#include "bn_sprite_palette_color_hbe_ptr.cpp.h"
#include "bn_sprite_position_hbe_ptr.cpp.h"
#include "bn_sprite_regular_second_attributes_hbe_ptr.cpp.h"
#include "bn_sprite_third_attributes_hbe_ptr.cpp.h"

namespace bn::hblank_effects_manager
{

namespace
{
    constexpr int max_items = BN_CFG_HBES_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= 8);

    constexpr int max_uint32_output_values = hw::hblank_effects::max_uint32_entries();
    constexpr int max_uint16_output_values = max(max_items - max_uint32_output_values, 1);

    using hw_entries = hw::hblank_effects::entries;

    [[nodiscard]] bool _is_uint32(handler_type handler)
    {
        switch(handler)
        {

        case handler_type::BG_PALETTE_COLOR:
            return false;

        case handler_type::BG_PALETTES_TRANSPARENT_COLOR:
            return false;

        case handler_type::BLENDING_FADE_ALPHA:
            return false;

        case handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES:
            return false;

        case handler_type::GREEN_SWAP:
            return false;

        case handler_type::MOSAIC_ATTRIBUTES:
            return false;

        case handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
            return false;

        case handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
            return false;

        case handler_type::REGULAR_BG_ATTRIBUTES:
            return false;

        case handler_type::AFFINE_BG_ATTRIBUTES:
            return false;

        case handler_type::REGULAR_BG_HORIZONTAL_POSITION:
            return false;

        case handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION:
            return true;

        case handler_type::REGULAR_BG_VERTICAL_POSITION:
            return false;

        case handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION:
            return true;

        case handler_type::AFFINE_BG_PA_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::AFFINE_BG_PA_REGISTER_VALUES:
            return false;

        case handler_type::AFFINE_BG_PB_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::AFFINE_BG_PB_REGISTER_VALUES:
            return false;

        case handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::AFFINE_BG_PC_REGISTER_VALUES:
            return false;

        case handler_type::AFFINE_BG_PD_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::AFFINE_BG_PD_REGISTER_VALUES:
            return false;

        case handler_type::AFFINE_BG_DX_REGISTER_ATTRIBUTES:
            return true;

        case handler_type::AFFINE_BG_DX_REGISTER_VALUES:
            return true;

        case handler_type::AFFINE_BG_DY_REGISTER_ATTRIBUTES:
            return true;

        case handler_type::AFFINE_BG_DY_REGISTER_VALUES:
            return true;

        case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_VALUES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_VALUES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES:
            return false;

        case handler_type::SPRITE_FIRST_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_AFFINE_SECOND_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_THIRD_ATTRIBUTES:
            return false;

        case handler_type::SPRITE_HORIZONTAL_POSITION:
            return false;

        case handler_type::SPRITE_VERTICAL_POSITION:
            return false;

        case handler_type::SPRITE_PALETTE_COLOR:
            return false;

        default:
            BN_ERROR("Unknown handler: ", int(handler));
            return false;
        }
    }

    class uint16_output_values_type
    {

    public:
        alignas(int) uint16_t a[display::height()];
        alignas(int) uint16_t b[display::height()];
        bool a_active = false;
    };

    class uint32_output_values_type
    {

    public:
        alignas(int) uint16_t a[display::height() * 2];
        alignas(int) uint16_t b[display::height() * 2];
        bool a_active = false;
    };

    class item_type
    {

    public:
        int target_last_value[3];
        const void* values_ptr = nullptr;
        intptr_t target_id = 0;
        unsigned usages = 0;
        uint16_t* output_register = nullptr;
        uint16_output_values_type* uint16_output_values = nullptr;
        uint32_output_values_type* uint32_output_values = nullptr;
        handler_type handler;
        bool visible: 1 = false;
        bool update: 1 = false;
        bool on_screen: 1 = false;
        bool output_values_written: 1 = false;

        void setup_target()
        {
            switch(handler)
            {

            case handler_type::BG_PALETTE_COLOR:
                bg_palette_color_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::BG_PALETTES_TRANSPARENT_COLOR:
                bg_palettes_transparent_color_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::BLENDING_FADE_ALPHA:
                blending_fade_alpha_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES:
                blending_transparency_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::GREEN_SWAP:
                green_swap_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::MOSAIC_ATTRIBUTES:
                mosaic_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                rect_window_horizontal_boundaries_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                rect_window_vertical_boundaries_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::REGULAR_BG_ATTRIBUTES:
                regular_bg_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_ATTRIBUTES:
                affine_bg_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::REGULAR_BG_HORIZONTAL_POSITION:
                regular_bg_horizontal_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION:
                affine_bg_pivot_horizontal_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::REGULAR_BG_VERTICAL_POSITION:
                regular_bg_vertical_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION:
                affine_bg_pivot_vertical_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_ATTRIBUTES:
                affine_bg_pa_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_VALUES:
                affine_bg_pa_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_ATTRIBUTES:
                affine_bg_pb_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_VALUES:
                affine_bg_pb_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES:
                affine_bg_pc_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_VALUES:
                affine_bg_pc_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_ATTRIBUTES:
                affine_bg_pd_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_VALUES:
                affine_bg_pd_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_ATTRIBUTES:
                affine_bg_dx_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_VALUES:
                affine_bg_dx_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_ATTRIBUTES:
                affine_bg_dy_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_VALUES:
                affine_bg_dy_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pa_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES:
                sprite_affine_mat_pa_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pb_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_VALUES:
                sprite_affine_mat_pb_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pc_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_VALUES:
                sprite_affine_mat_pc_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pd_register_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES:
                sprite_affine_mat_pd_register_values_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_FIRST_ATTRIBUTES:
                sprite_first_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES:
                sprite_regular_second_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_AFFINE_SECOND_ATTRIBUTES:
                sprite_affine_second_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_THIRD_ATTRIBUTES:
                sprite_third_attributes_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_HORIZONTAL_POSITION:
                sprite_horizontal_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_VERTICAL_POSITION:
                sprite_vertical_position_hbe_handler::setup_target(target_id, target_last_value);
                break;

            case handler_type::SPRITE_PALETTE_COLOR:
                sprite_palette_color_hbe_handler::setup_target(target_id, target_last_value);
                break;

            default:
                BN_ERROR("Unknown handler: ", int(handler));
                break;
            }
        }

        [[nodiscard]] bool check_update()
        {
            bool updated = update;
            update = false;

            switch(handler)
            {

            case handler_type::BG_PALETTE_COLOR:
                return _check_update_impl<bg_palette_color_hbe_handler>(updated);

            case handler_type::BG_PALETTES_TRANSPARENT_COLOR:
                return _check_update_impl<bg_palettes_transparent_color_hbe_handler>(updated);

            case handler_type::BLENDING_FADE_ALPHA:
                return _check_update_impl<blending_fade_alpha_hbe_handler>(updated);

            case handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES:
                return _check_update_impl<blending_transparency_attributes_hbe_handler>(updated);

            case handler_type::GREEN_SWAP:
                return _check_update_impl<green_swap_hbe_handler>(updated);

            case handler_type::MOSAIC_ATTRIBUTES:
                return _check_update_impl<mosaic_attributes_hbe_handler>(updated);

            case handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                return _check_update_impl<rect_window_horizontal_boundaries_hbe_handler>(updated);

            case handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                return _check_update_impl<rect_window_vertical_boundaries_hbe_handler>(updated);

            case handler_type::REGULAR_BG_ATTRIBUTES:
                return _check_update_impl<regular_bg_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_ATTRIBUTES:
                return _check_update_impl<affine_bg_attributes_hbe_handler>(updated);

            case handler_type::REGULAR_BG_HORIZONTAL_POSITION:
                return _check_update_impl<regular_bg_horizontal_position_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION:
                return _check_update_impl<affine_bg_pivot_horizontal_position_hbe_handler>(updated);

            case handler_type::REGULAR_BG_VERTICAL_POSITION:
                return _check_update_impl<regular_bg_vertical_position_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION:
                return _check_update_impl<affine_bg_pivot_vertical_position_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PA_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_pa_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PA_REGISTER_VALUES:
                return _check_update_impl<affine_bg_pa_register_values_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PB_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_pb_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PB_REGISTER_VALUES:
                return _check_update_impl<affine_bg_pb_register_values_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_pc_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PC_REGISTER_VALUES:
                return _check_update_impl<affine_bg_pc_register_values_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PD_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_pd_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_PD_REGISTER_VALUES:
                return _check_update_impl<affine_bg_pd_register_values_hbe_handler>(updated);

            case handler_type::AFFINE_BG_DX_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_dx_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_DX_REGISTER_VALUES:
                return _check_update_impl<affine_bg_dx_register_values_hbe_handler>(updated);

            case handler_type::AFFINE_BG_DY_REGISTER_ATTRIBUTES:
                return _check_update_impl<affine_bg_dy_register_attributes_hbe_handler>(updated);

            case handler_type::AFFINE_BG_DY_REGISTER_VALUES:
                return _check_update_impl<affine_bg_dy_register_values_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES:
                return _check_update_impl<sprite_affine_mat_pa_register_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES:
                return _check_update_impl<sprite_affine_mat_pa_register_values_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES:
                return _check_update_impl<sprite_affine_mat_pb_register_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_VALUES:
                return _check_update_impl<sprite_affine_mat_pb_register_values_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES:
                return _check_update_impl<sprite_affine_mat_pc_register_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_VALUES:
                return _check_update_impl<sprite_affine_mat_pc_register_values_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES:
                return _check_update_impl<sprite_affine_mat_pd_register_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES:
                return _check_update_impl<sprite_affine_mat_pd_register_values_hbe_handler>(updated);

            case handler_type::SPRITE_FIRST_ATTRIBUTES:
                return _check_update_impl<sprite_first_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES:
                return _check_update_impl<sprite_regular_second_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_AFFINE_SECOND_ATTRIBUTES:
                return _check_update_impl<sprite_affine_second_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_THIRD_ATTRIBUTES:
                return _check_update_impl<sprite_third_attributes_hbe_handler>(updated);

            case handler_type::SPRITE_HORIZONTAL_POSITION:
                return _check_update_impl<sprite_horizontal_position_hbe_handler>(updated);

            case handler_type::SPRITE_VERTICAL_POSITION:
                return _check_update_impl<sprite_vertical_position_hbe_handler>(updated);

            case handler_type::SPRITE_PALETTE_COLOR:
                return _check_update_impl<sprite_palette_color_hbe_handler>(updated);

            default:
                BN_ERROR("Unknown handler: ", int(handler));
                return false;
            }
        }

        void setup_entry(hw_entries& entries) const
        {
            if(_is_uint32(handler))
            {
                BN_BASIC_ASSERT(entries.uint32_entries_count < max_uint32_output_values,
                                "Too many 32 bits entries");

                hw::hblank_effects::uint32_entry& uint32_entry = entries.uint32_entries[entries.uint32_entries_count];
                const uint16_t* src = uint32_output_values->a_active ? uint32_output_values->a : uint32_output_values->b;
                uint32_entry.src = reinterpret_cast<const uint32_t*>(src);
                uint32_entry.dest = reinterpret_cast<uint32_t*>(output_register);
                ++entries.uint32_entries_count;
            }
            else
            {
                hw::hblank_effects::uint16_entry& uint16_entry = entries.uint16_entries[entries.uint16_entries_count];

                if(uint16_output_values)
                {
                    uint16_entry.src = uint16_output_values->a_active ? uint16_output_values->a : uint16_output_values->b;
                }
                else
                {
                    uint16_entry.src = uint32_output_values->a_active ? uint32_output_values->a : uint32_output_values->b;
                }

                uint16_entry.dest = output_register;
                ++entries.uint16_entries_count;
            }
        }

        void show()
        {
            switch(handler)
            {

            case handler_type::BG_PALETTE_COLOR:
                bg_palette_color_hbe_handler::show(target_id);
                break;

            case handler_type::BG_PALETTES_TRANSPARENT_COLOR:
                bg_palettes_transparent_color_hbe_handler::show(target_id);
                break;

            case handler_type::BLENDING_FADE_ALPHA:
                blending_fade_alpha_hbe_handler::show(target_id);
                break;

            case handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES:
                blending_transparency_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::GREEN_SWAP:
                green_swap_hbe_handler::show(target_id);
                break;

            case handler_type::MOSAIC_ATTRIBUTES:
                mosaic_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                rect_window_horizontal_boundaries_hbe_handler::show(target_id);
                break;

            case handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                rect_window_vertical_boundaries_hbe_handler::show(target_id);
                break;

            case handler_type::REGULAR_BG_ATTRIBUTES:
                regular_bg_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_ATTRIBUTES:
                affine_bg_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::REGULAR_BG_HORIZONTAL_POSITION:
                regular_bg_horizontal_position_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION:
                affine_bg_pivot_horizontal_position_hbe_handler::show(target_id);
                break;

            case handler_type::REGULAR_BG_VERTICAL_POSITION:
                regular_bg_vertical_position_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION:
                affine_bg_pivot_vertical_position_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_ATTRIBUTES:
                affine_bg_pa_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_VALUES:
                affine_bg_pa_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_ATTRIBUTES:
                affine_bg_pb_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_VALUES:
                affine_bg_pb_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES:
                affine_bg_pc_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_VALUES:
                affine_bg_pc_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_ATTRIBUTES:
                affine_bg_pd_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_VALUES:
                affine_bg_pd_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_ATTRIBUTES:
                affine_bg_dx_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_VALUES:
                affine_bg_dx_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_ATTRIBUTES:
                affine_bg_dy_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_VALUES:
                affine_bg_dy_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pa_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES:
                sprite_affine_mat_pa_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pb_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_VALUES:
                sprite_affine_mat_pb_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pc_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_VALUES:
                sprite_affine_mat_pc_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pd_register_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES:
                sprite_affine_mat_pd_register_values_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_FIRST_ATTRIBUTES:
                sprite_first_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES:
                sprite_regular_second_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_AFFINE_SECOND_ATTRIBUTES:
                sprite_affine_second_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_THIRD_ATTRIBUTES:
                sprite_third_attributes_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_HORIZONTAL_POSITION:
                sprite_horizontal_position_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_VERTICAL_POSITION:
                sprite_vertical_position_hbe_handler::show(target_id);
                break;

            case handler_type::SPRITE_PALETTE_COLOR:
                sprite_palette_color_hbe_handler::show(target_id);
                break;

            default:
                BN_ERROR("Unknown handler: ", int(handler));
                break;
            }
        }

        void cleanup()
        {
            switch(handler)
            {

            case handler_type::BG_PALETTE_COLOR:
                bg_palette_color_hbe_handler::cleanup(target_id);
                break;

            case handler_type::BG_PALETTES_TRANSPARENT_COLOR:
                bg_palettes_transparent_color_hbe_handler::cleanup(target_id);
                break;

            case handler_type::BLENDING_FADE_ALPHA:
                blending_fade_alpha_hbe_handler::cleanup(target_id);
                break;

            case handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES:
                blending_transparency_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::GREEN_SWAP:
                green_swap_hbe_handler::cleanup(target_id);
                break;

            case handler_type::MOSAIC_ATTRIBUTES:
                mosaic_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                rect_window_horizontal_boundaries_hbe_handler::cleanup(target_id);
                break;

            case handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                rect_window_vertical_boundaries_hbe_handler::cleanup(target_id);
                break;

            case handler_type::REGULAR_BG_ATTRIBUTES:
                regular_bg_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_ATTRIBUTES:
                affine_bg_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::REGULAR_BG_HORIZONTAL_POSITION:
                regular_bg_horizontal_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION:
                affine_bg_pivot_horizontal_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::REGULAR_BG_VERTICAL_POSITION:
                regular_bg_vertical_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION:
                affine_bg_pivot_vertical_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_ATTRIBUTES:
                affine_bg_pa_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PA_REGISTER_VALUES:
                affine_bg_pa_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_ATTRIBUTES:
                affine_bg_pb_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PB_REGISTER_VALUES:
                affine_bg_pb_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES:
                affine_bg_pc_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PC_REGISTER_VALUES:
                affine_bg_pc_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_ATTRIBUTES:
                affine_bg_pd_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_PD_REGISTER_VALUES:
                affine_bg_pd_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_ATTRIBUTES:
                affine_bg_dx_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_DX_REGISTER_VALUES:
                affine_bg_dx_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_ATTRIBUTES:
                affine_bg_dy_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::AFFINE_BG_DY_REGISTER_VALUES:
                affine_bg_dy_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pa_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES:
                sprite_affine_mat_pa_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pb_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PB_REGISTER_VALUES:
                sprite_affine_mat_pb_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pc_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PC_REGISTER_VALUES:
                sprite_affine_mat_pc_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES:
                sprite_affine_mat_pd_register_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES:
                sprite_affine_mat_pd_register_values_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_FIRST_ATTRIBUTES:
                sprite_first_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES:
                sprite_regular_second_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_AFFINE_SECOND_ATTRIBUTES:
                sprite_affine_second_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_THIRD_ATTRIBUTES:
                sprite_third_attributes_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_HORIZONTAL_POSITION:
                sprite_horizontal_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_VERTICAL_POSITION:
                sprite_vertical_position_hbe_handler::cleanup(target_id);
                break;

            case handler_type::SPRITE_PALETTE_COLOR:
                sprite_palette_color_hbe_handler::cleanup(target_id);
                break;

            default:
                BN_ERROR("Unknown handler: ", int(handler));
                break;
            }
        }

    private:
        [[nodiscard]] __attribute__((noinline)) uint16_t* _output_values_ptr()
        {
            uint16_t* output_values_ptr;

            if(uint16_output_values)
            {
                if(uint16_output_values->a_active)
                {
                    output_values_ptr = uint16_output_values->b;
                    uint16_output_values->a_active = false;
                }
                else
                {
                    output_values_ptr = uint16_output_values->a;
                    uint16_output_values->a_active = true;
                }
            }
            else
            {
                if(uint32_output_values->a_active)
                {
                    output_values_ptr = uint32_output_values->b;
                    uint32_output_values->a_active = false;
                }
                else
                {
                    output_values_ptr = uint32_output_values->a;
                    uint32_output_values->a_active = true;
                }
            }

            return output_values_ptr;
        }

        template<class Handler>
        [[nodiscard]] bool _check_update_impl(bool updated)
        {
            bool old_on_screen = on_screen;
            bool new_on_screen = Handler::target_visible(target_id);
            on_screen = new_on_screen;

            if(new_on_screen)
            {
                updated |= Handler::target_updated(target_id, target_last_value);

                if(! output_values_written)
                {
                    updated = true;
                    output_values_written = true;
                }

                if(updated)
                {
                    uint16_t* output_values_ptr = _output_values_ptr();
                    Handler::write_output_values(target_id, target_last_value, values_ptr, output_values_ptr);
                }

                uint16_t* old_output_register = output_register;
                output_register = Handler::output_register(target_id);
                updated |= old_output_register != output_register;
            }

            updated |= old_on_screen != new_on_screen;
            return updated;
        }
    };

    class static_external_data
    {

    public:
        item_type items[max_items];
        uint16_output_values_type uint16_output_values_array[max_uint16_output_values];
        uint32_output_values_type uint32_output_values_array[max_uint32_output_values];
        vector<int8_t, max_items> free_item_indexes;
        vector<int8_t, max_uint16_output_values> free_uint16_output_values_indexes;
        vector<int8_t, max_uint32_output_values> free_uint32_output_values_indexes;
        int8_t first_visible_item_index = max_items - 1;
        int8_t last_visible_item_index = 0;
        bool visible_entries = false;
        bool entries_a_active = false;
        bool update = false;
        bool commit = false;
        bool enabled = false;
    };

    class static_internal_data
    {

    public:
        hw_entries entries_a;
        hw_entries entries_b;
    };

    BN_DATA_EWRAM_BSS static_external_data external_data;
    static_internal_data internal_data;

    void _update_visible_item_index(int item_index)
    {
        static_external_data& data = external_data;
        data.items[item_index].show();

        if(data.first_visible_item_index != max_items)
        {
            data.first_visible_item_index = int8_t(min(int(data.first_visible_item_index), item_index));
            data.last_visible_item_index = int8_t(max(int(data.last_visible_item_index), item_index));
        }
    }

    void _update_hidden_item_index(int item_index)
    {
        static_external_data& data = external_data;
        data.items[item_index].cleanup();

        if(item_index == data.first_visible_item_index || item_index == data.last_visible_item_index)
        {
            data.first_visible_item_index = max_items;
            data.last_visible_item_index = 0;
        }
    }

    [[nodiscard]] int _create(const void* values_ptr, intptr_t target_id, handler_type handler, bool optional)
    {
        BN_ASSERT(aligned<4>(values_ptr), "Values are not aligned");

        if(external_data.free_item_indexes.empty())
        {
            BN_BASIC_ASSERT(optional, "No more H-Blank effects available");
            return -1;
        }

        uint16_output_values_type* uint16_output_values = nullptr;
        uint32_output_values_type* uint32_output_values = nullptr;

        if(_is_uint32(handler))
        {
            if(! external_data.free_uint32_output_values_indexes.empty())
            {
                int output_values_index = external_data.free_uint32_output_values_indexes.back();
                external_data.free_uint32_output_values_indexes.pop_back();
                uint32_output_values = &external_data.uint32_output_values_array[output_values_index];
            }
            else
            {
                BN_BASIC_ASSERT(optional, "No more 32 bits H-Blank effects available");
                return -1;
            }
        }
        else
        {
            if(! external_data.free_uint16_output_values_indexes.empty())
            {
                int output_values_index = external_data.free_uint16_output_values_indexes.back();
                external_data.free_uint16_output_values_indexes.pop_back();
                uint16_output_values = &external_data.uint16_output_values_array[output_values_index];
            }
            else if(! external_data.free_uint32_output_values_indexes.empty())
            {
                int output_values_index = external_data.free_uint32_output_values_indexes.back();
                external_data.free_uint32_output_values_indexes.pop_back();
                uint32_output_values = &external_data.uint32_output_values_array[output_values_index];
            }
            else
            {
                BN_BASIC_ASSERT(optional, "No more 32 bits H-Blank effects available");
                return -1;
            }
        }

        int item_index = external_data.free_item_indexes.back();
        external_data.free_item_indexes.pop_back();

        item_type& new_item = external_data.items[item_index];
        new_item.values_ptr = values_ptr;
        new_item.target_id = target_id;
        new_item.usages = 1;
        new_item.output_register = nullptr;
        new_item.uint16_output_values = uint16_output_values;
        new_item.uint32_output_values = uint32_output_values;
        new_item.handler = handler;
        new_item.visible = true;
        new_item.update = true;
        new_item.on_screen = false;
        new_item.output_values_written = false;
        new_item.setup_target();

        _update_visible_item_index(item_index);
        external_data.update = true;

        return item_index;
    }
}

void init()
{
    new(&external_data) static_external_data();

    hw::hblank_effects::commit_entries(internal_data.entries_b);

    for(int index = max_items - 1; index >= 0; --index)
    {
        external_data.free_item_indexes.push_back(int8_t(index));
    }

    for(int index = max_uint16_output_values - 1; index >= 0; --index)
    {
        external_data.free_uint16_output_values_indexes.push_back(int8_t(index));
    }

    for(int index = max_uint32_output_values - 1; index >= 0; --index)
    {
        external_data.free_uint32_output_values_indexes.push_back(int8_t(index));
    }
}

int used_count()
{
    return external_data.free_item_indexes.available();
}

int available_count()
{
    return external_data.free_item_indexes.size();
}

void enable()
{
    if(external_data.enabled)
    {
        hw::hblank_effects::enable();
    }
}

void disable()
{
    if(external_data.enabled)
    {
        hw::hblank_effects::disable();
    }
}

int create(const void* values_ptr, [[maybe_unused]] int values_count, intptr_t target_id, handler_type handler)
{
    BN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());

    return _create(values_ptr, target_id, handler, false);
}

int create_optional(const void* values_ptr, [[maybe_unused]] int values_count, intptr_t target_id,
                    handler_type handler)
{
    BN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());

    return _create(values_ptr, target_id, handler, true);
}

void increase_usages(int id)
{
    item_type& item = external_data.items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = external_data.items[id];
    --item.usages;

    if(! item.usages)
    {
        if(item.visible)
        {
            item.visible = false;

            _update_hidden_item_index(id);
            external_data.update = true;
        }

        if(item.uint16_output_values)
        {
            int output_values_index = item.uint16_output_values - external_data.uint16_output_values_array;
            external_data.free_uint16_output_values_indexes.push_back(int8_t(output_values_index));
        }
        else
        {
            int output_values_index = item.uint32_output_values - external_data.uint32_output_values_array;
            external_data.free_uint32_output_values_indexes.push_back(int8_t(output_values_index));
        }

        external_data.free_item_indexes.push_back(int8_t(id));
        item.update = false;
    }
}

const void* values_ref(int id)
{
    const item_type& item = external_data.items[id];
    return item.values_ptr;
}

void set_values_ref(int id, const void* values_ptr, [[maybe_unused]] int values_count)
{
    BN_ASSERT(values_count >= display::height(),
              "Invalid values count: ", values_count, " - ", display::height());
    BN_ASSERT(aligned<4>(values_ptr), "Values are not aligned");

    item_type& item = external_data.items[id];
    item.values_ptr = values_ptr;
    item.update = true;

    if(item.visible)
    {
        external_data.update = true;
    }
}

void reload_values_ref(int id)
{
    item_type& item = external_data.items[id];
    item.update = true;

    if(item.visible)
    {
        external_data.update = true;
    }
}

[[nodiscard]] bool visible(int id)
{
    const item_type& item = external_data.items[id];
    return item.visible;
}

void set_visible(int id, bool visible)
{
    item_type& item = external_data.items[id];

    if(visible != item.visible)
    {
        item.visible = visible;
        external_data.update = true;

        if(visible)
        {
            _update_visible_item_index(id);
        }
        else
        {
            _update_hidden_item_index(id);
        }
    }
}

void update()
{
    bool update = external_data.update;
    external_data.update = false;

    int first_visible_item_index = external_data.first_visible_item_index;
    int last_visible_item_index = external_data.last_visible_item_index;

    if(first_visible_item_index < max_items)
    {
        for(int item_index = first_visible_item_index; item_index <= last_visible_item_index; ++item_index)
        {
            item_type& item = external_data.items[item_index];

            if(item.visible)
            {
                update |= item.check_update();
            }
        }
    }
    else
    {
        for(int item_index = 0; item_index < max_items; ++item_index)
        {
            item_type& item = external_data.items[item_index];

            if(item.visible)
            {
                update |= item.check_update();
                first_visible_item_index = min(first_visible_item_index, item_index);
                last_visible_item_index = item_index;
            }
        }

        if(first_visible_item_index == max_items)
        {
            external_data.first_visible_item_index = max_items - 1;
            external_data.last_visible_item_index = 0;
        }
        else
        {
            external_data.first_visible_item_index = int8_t(first_visible_item_index);
            external_data.last_visible_item_index = int8_t(last_visible_item_index);
        }
    }

    if(update)
    {
        hw_entries* entries;
        bool visible_entries = false;

        if(external_data.entries_a_active)
        {
            entries = &internal_data.entries_b;
            external_data.entries_a_active = false;
        }
        else
        {
            entries = &internal_data.entries_a;
            external_data.entries_a_active = true;
        }

        entries->uint16_entries_count = 0;
        entries->uint32_entries_count = 0;

        for(int item_index = first_visible_item_index; item_index <= last_visible_item_index; ++item_index)
        {
            const item_type& item = external_data.items[item_index];

            if(item.visible && item.on_screen)
            {
                item.setup_entry(*entries);
                visible_entries = true;
            }
        }

        external_data.visible_entries = visible_entries;
        external_data.commit = true;
    }
}

bool commit()
{
    if(external_data.commit)
    {
        external_data.commit = false;

        if(external_data.visible_entries)
        {
            hw_entries* entries = external_data.entries_a_active ? &internal_data.entries_a : &internal_data.entries_b;
            hw::hblank_effects::commit_entries(*entries);

            if(! external_data.enabled)
            {
                external_data.enabled = true;
                hw::hblank_effects::enable();
            }
        }
        else
        {
            if(external_data.enabled)
            {
                external_data.enabled = false;
                hw::hblank_effects::disable();
            }
        }
    }

    return external_data.enabled;
}

}
