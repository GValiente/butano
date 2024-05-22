/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_mosaic_attributes_hbe_ptr.h"

#include "bn_display.h"
#include "bn_mosaic_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

mosaic_attributes_hbe_ptr mosaic_attributes_hbe_ptr::create(const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), 0,
                                            hblank_effects_manager::handler_type::MOSAIC_ATTRIBUTES);
    return mosaic_attributes_hbe_ptr(id);
}

optional<mosaic_attributes_hbe_ptr> mosaic_attributes_hbe_ptr::create_optional(
        const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::MOSAIC_ATTRIBUTES);
    optional<mosaic_attributes_hbe_ptr> result;

    if(id >= 0)
    {
        result = mosaic_attributes_hbe_ptr(id);
    }

    return result;
}

span<const mosaic_attributes> mosaic_attributes_hbe_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const mosaic_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const mosaic_attributes>(values_ptr, display::height());
}

void mosaic_attributes_hbe_ptr::set_attributes_ref(const span<const mosaic_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void mosaic_attributes_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

mosaic_attributes_hbe_ptr::mosaic_attributes_hbe_ptr(int id) :
    hbe_ptr(id)
{
}

}
