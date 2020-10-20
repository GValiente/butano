/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_blending_transparency_attributes_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending_transparency_attributes.h"
#include "btn_display_manager.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

blending_transparency_attributes_hblank_effect_ptr blending_transparency_attributes_hblank_effect_ptr::create(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    BTN_ASSERT(! display_manager::blending_fade_enabled(),
               "Blending transparency and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), 0,
                                            hblank_effects_manager::handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES);
    return blending_transparency_attributes_hblank_effect_ptr(id);
}

optional<blending_transparency_attributes_hblank_effect_ptr> blending_transparency_attributes_hblank_effect_ptr::create_optional(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    BTN_ASSERT(! display_manager::blending_fade_enabled(),
               "Blending transparency and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES);
    optional<blending_transparency_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = blending_transparency_attributes_hblank_effect_ptr(id);
    }

    return result;
}

span<const blending_transparency_attributes> blending_transparency_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const blending_transparency_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const blending_transparency_attributes>(values_ptr, display::height());
}

void blending_transparency_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void blending_transparency_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

blending_transparency_attributes_hblank_effect_ptr::blending_transparency_attributes_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
