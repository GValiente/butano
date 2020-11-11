/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_regular_bg_attributes_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_regular_bg_attributes.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

regular_bg_attributes_hblank_effect_ptr regular_bg_attributes_hblank_effect_ptr::create(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), int(bg.handle()),
                                            hblank_effects_manager::handler_type::REGULAR_BG_ATTRIBUTES);
    return regular_bg_attributes_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_attributes_hblank_effect_ptr> regular_bg_attributes_hblank_effect_ptr::create_optional(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), int(bg.handle()),
                                                     hblank_effects_manager::handler_type::REGULAR_BG_ATTRIBUTES);
    optional<regular_bg_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_attributes_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

span<const regular_bg_attributes> regular_bg_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const regular_bg_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const regular_bg_attributes>(values_ptr, display::height());
}

void regular_bg_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const regular_bg_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void regular_bg_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_attributes_hblank_effect_ptr::swap(regular_bg_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    _bg.swap(other._bg);
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(int id, regular_bg_ptr&& bg) :
    hblank_effect_ptr(id),
    _bg(move(bg))
{
}

}
