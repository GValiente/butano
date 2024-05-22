/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_backdrop_color_hbe_ptr.h"

#include "bn_color.h"
#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

backdrop_color_hbe_ptr backdrop_color_hbe_ptr::create(const span<const color>& colors_ref)
{
    int id = hblank_effects_manager::create(
            colors_ref.data(), colors_ref.size(), 0,
            hblank_effects_manager::handler_type::BG_PALETTES_TRANSPARENT_COLOR);

    return backdrop_color_hbe_ptr(id);
}

optional<backdrop_color_hbe_ptr> backdrop_color_hbe_ptr::create_optional(
        const span<const color>& colors_ref)
{
    int id = hblank_effects_manager::create_optional(
            colors_ref.data(), colors_ref.size(), 0,
            hblank_effects_manager::handler_type::BG_PALETTES_TRANSPARENT_COLOR);

    optional<backdrop_color_hbe_ptr> result;

    if(id >= 0)
    {
        result = backdrop_color_hbe_ptr(id);
    }

    return result;
}

span<const color> backdrop_color_hbe_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, display::height());
}

void backdrop_color_hbe_ptr::set_colors_ref(const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void backdrop_color_hbe_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void backdrop_color_hbe_ptr::swap(backdrop_color_hbe_ptr& other)
{
    hbe_ptr::swap(other);
}

backdrop_color_hbe_ptr::backdrop_color_hbe_ptr(int id) :
    hbe_ptr(id)
{
}

}
