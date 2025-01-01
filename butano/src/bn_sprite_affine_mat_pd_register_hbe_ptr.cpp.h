/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mat_pd_register_hbe_ptr.h"

#include "bn_display.h"
#include "bn_affine_mat_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

sprite_affine_mat_pd_register_hbe_ptr sprite_affine_mat_pd_register_hbe_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                            hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES);
    return sprite_affine_mat_pd_register_hbe_ptr(id, true, move(affine_mat));
}

sprite_affine_mat_pd_register_hbe_ptr sprite_affine_mat_pd_register_hbe_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat.id(),
                                            hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES);
    return sprite_affine_mat_pd_register_hbe_ptr(id, false, move(affine_mat));
}

optional<sprite_affine_mat_pd_register_hbe_ptr> sprite_affine_mat_pd_register_hbe_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                                     hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_ATTRIBUTES);
    optional<sprite_affine_mat_pd_register_hbe_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pd_register_hbe_ptr(id, true, move(affine_mat));
    }

    return result;
}

optional<sprite_affine_mat_pd_register_hbe_ptr> sprite_affine_mat_pd_register_hbe_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), affine_mat.id(),
                                                     hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PD_REGISTER_VALUES);
    optional<sprite_affine_mat_pd_register_hbe_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pd_register_hbe_ptr(id, false, move(affine_mat));
    }

    return result;
}

span<const affine_mat_attributes> sprite_affine_mat_pd_register_hbe_ptr::attributes_ref() const
{
    BN_BASIC_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const affine_mat_attributes>(attributes_ptr, display::height());
}

span<const int16_t> sprite_affine_mat_pd_register_hbe_ptr::values_ref() const
{
    BN_BASIC_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, display::height());
}

void sprite_affine_mat_pd_register_hbe_ptr::set_attributes_ref(
        const span<const affine_mat_attributes>& attributes_ref)
{
    BN_BASIC_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_pd_register_hbe_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BN_BASIC_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_pd_register_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pd_register_hbe_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pd_register_hbe_ptr::swap(sprite_affine_mat_pd_register_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    bn::swap(_from_attributes, other._from_attributes);
    _affine_mat.swap(other._affine_mat);
}

sprite_affine_mat_pd_register_hbe_ptr::sprite_affine_mat_pd_register_hbe_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat) :
    hbe_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat(move(affine_mat))
{
}

}
