/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sp_direct_bitmap_bg_pc_register_hbe_ptr.h"

#include "bn_display.h"
#include "bn_affine_bg_mat_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

sp_direct_bitmap_bg_pc_register_hbe_ptr sp_direct_bitmap_bg_pc_register_hbe_ptr::create(
        sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), intptr_t(bg.handle()),
                                            hblank_effects_manager::handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES);
    return sp_direct_bitmap_bg_pc_register_hbe_ptr(id, true, move(bg));
}

sp_direct_bitmap_bg_pc_register_hbe_ptr sp_direct_bitmap_bg_pc_register_hbe_ptr::create(
        sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), intptr_t(bg.handle()),
                                            hblank_effects_manager::handler_type::AFFINE_BG_PC_REGISTER_VALUES);
    return sp_direct_bitmap_bg_pc_register_hbe_ptr(id, false, move(bg));
}

optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> sp_direct_bitmap_bg_pc_register_hbe_ptr::create_optional(
        sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), intptr_t(bg.handle()),
                                                     hblank_effects_manager::handler_type::AFFINE_BG_PC_REGISTER_ATTRIBUTES);
    optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> result;

    if(id >= 0)
    {
        result = sp_direct_bitmap_bg_pc_register_hbe_ptr(id, true, move(bg));
    }

    return result;
}

optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> sp_direct_bitmap_bg_pc_register_hbe_ptr::create_optional(
        sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), intptr_t(bg.handle()),
                                                     hblank_effects_manager::handler_type::AFFINE_BG_PC_REGISTER_VALUES);
    optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> result;

    if(id >= 0)
    {
        result = sp_direct_bitmap_bg_pc_register_hbe_ptr(id, false, move(bg));
    }

    return result;
}

span<const affine_bg_mat_attributes> sp_direct_bitmap_bg_pc_register_hbe_ptr::attributes_ref() const
{
    BN_BASIC_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const affine_bg_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const affine_bg_mat_attributes>(attributes_ptr, display::height());
}

span<const int16_t> sp_direct_bitmap_bg_pc_register_hbe_ptr::values_ref() const
{
    BN_BASIC_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, display::height());
}

void sp_direct_bitmap_bg_pc_register_hbe_ptr::set_attributes_ref(
        const span<const affine_bg_mat_attributes>& attributes_ref)
{
    BN_BASIC_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sp_direct_bitmap_bg_pc_register_hbe_ptr::set_values_ref(const span<const int16_t>& values_ref)
{
    BN_BASIC_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sp_direct_bitmap_bg_pc_register_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sp_direct_bitmap_bg_pc_register_hbe_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sp_direct_bitmap_bg_pc_register_hbe_ptr::swap(sp_direct_bitmap_bg_pc_register_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    bn::swap(_from_attributes, other._from_attributes);
    _bg.swap(other._bg);
}

sp_direct_bitmap_bg_pc_register_hbe_ptr::sp_direct_bitmap_bg_pc_register_hbe_ptr(
        int id, bool from_attributes, sp_direct_bitmap_bg_ptr&& bg) :
    hbe_ptr(id),
    _from_attributes(from_attributes),
    _bg(move(bg))
{
}

}
