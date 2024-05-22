/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_blending_transparency_attributes_hbe_ptr.h"

#include "bn_display.h"
#include "bn_blending_transparency_attributes.h"
#include "bn_display_manager.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

blending_transparency_attributes_hbe_ptr blending_transparency_attributes_hbe_ptr::create(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    BN_BASIC_ASSERT(! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), 0,
                                            hblank_effects_manager::handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES);
    return blending_transparency_attributes_hbe_ptr(id);
}

optional<blending_transparency_attributes_hbe_ptr> blending_transparency_attributes_hbe_ptr::create_optional(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    BN_BASIC_ASSERT(! display_manager::blending_fade_enabled(),
                    "Transparency and fade blendings can't be enabled at the same time");

    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::BLENDING_TRANSPARENCY_ATTRIBUTES);
    optional<blending_transparency_attributes_hbe_ptr> result;

    if(id >= 0)
    {
        result = blending_transparency_attributes_hbe_ptr(id);
    }

    return result;
}

span<const blending_transparency_attributes> blending_transparency_attributes_hbe_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const blending_transparency_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const blending_transparency_attributes>(values_ptr, display::height());
}

void blending_transparency_attributes_hbe_ptr::set_attributes_ref(
        const span<const blending_transparency_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void blending_transparency_attributes_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

blending_transparency_attributes_hbe_ptr::blending_transparency_attributes_hbe_ptr(int id) :
    hbe_ptr(id)
{
}

}
