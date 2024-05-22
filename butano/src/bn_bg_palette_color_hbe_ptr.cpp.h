/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_palette_color_hbe_ptr.h"

#include "bn_color.h"
#include "bn_display.h"
#include "bn_palette_target_id.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

bg_palette_color_hbe_ptr bg_palette_color_hbe_ptr::create(
        bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id,
                hblank_effects_manager::handler_type::BG_PALETTE_COLOR);
    return bg_palette_color_hbe_ptr(id, color_index, move(palette));
}

optional<bg_palette_color_hbe_ptr> bg_palette_color_hbe_ptr::create_optional(
        bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create_optional(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id,
                hblank_effects_manager::handler_type::BG_PALETTE_COLOR);
    optional<bg_palette_color_hbe_ptr> result;

    if(id >= 0)
    {
        result = bg_palette_color_hbe_ptr(id, color_index, move(palette));
    }

    return result;
}

span<const color> bg_palette_color_hbe_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, display::height());
}

void bg_palette_color_hbe_ptr::set_colors_ref(const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void bg_palette_color_hbe_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void bg_palette_color_hbe_ptr::swap(bg_palette_color_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _palette.swap(other._palette);
    bn::swap(_color_index, other._color_index);
}

bg_palette_color_hbe_ptr::bg_palette_color_hbe_ptr(int id, int color_index, bg_palette_ptr&& palette) :
    hbe_ptr(id),
    _palette(move(palette)),
    _color_index(int16_t(color_index))
{
}

}
