/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_attributes_hbe_ptr.h"

#include "bn_display.h"
#include "bn_affine_bg_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

affine_bg_attributes_hbe_ptr affine_bg_attributes_hbe_ptr::create(
        affine_bg_ptr bg, const span<const affine_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(
                attributes_ref.data(), attributes_ref.size(), intptr_t(bg.handle()),
                hblank_effects_manager::handler_type::AFFINE_BG_ATTRIBUTES);

    return affine_bg_attributes_hbe_ptr(id, move(bg));
}

optional<affine_bg_attributes_hbe_ptr> affine_bg_attributes_hbe_ptr::create_optional(
        affine_bg_ptr bg, const span<const affine_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(
                attributes_ref.data(), attributes_ref.size(), intptr_t(bg.handle()),
                hblank_effects_manager::handler_type::AFFINE_BG_ATTRIBUTES);

    optional<affine_bg_attributes_hbe_ptr> result;

    if(id >= 0)
    {
        result = affine_bg_attributes_hbe_ptr(id, move(bg));
    }

    return result;
}

span<const affine_bg_attributes> affine_bg_attributes_hbe_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const affine_bg_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const affine_bg_attributes>(values_ptr, display::height());
}

void affine_bg_attributes_hbe_ptr::set_attributes_ref(const span<const affine_bg_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void affine_bg_attributes_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void affine_bg_attributes_hbe_ptr::swap(affine_bg_attributes_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _bg.swap(other._bg);
}

affine_bg_attributes_hbe_ptr::affine_bg_attributes_hbe_ptr(int id, affine_bg_ptr&& bg) :
    hbe_ptr(id),
    _bg(move(bg))
{
}

}
