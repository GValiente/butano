/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_palette_color_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_color.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_palette_target_id.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

sprite_palette_color_hblank_effect_ptr sprite_palette_color_hblank_effect_ptr::create(
        sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id,
                hblank_effects_manager::handler_type::SPRITE_PALETTE_COLOR);
    return sprite_palette_color_hblank_effect_ptr(id, color_index, move(palette));
}

optional<sprite_palette_color_hblank_effect_ptr> sprite_palette_color_hblank_effect_ptr::create_optional(
        sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create_optional(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id,
                hblank_effects_manager::handler_type::SPRITE_PALETTE_COLOR);
    optional<sprite_palette_color_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_color_hblank_effect_ptr(id, color_index, move(palette));
    }

    return result;
}

sprite_palette_color_hblank_effect_ptr::sprite_palette_color_hblank_effect_ptr(
        sprite_palette_color_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _palette(move(other._palette)),
    _color_index(other._color_index)
{
}

sprite_palette_color_hblank_effect_ptr& sprite_palette_color_hblank_effect_ptr::operator=(
        sprite_palette_color_hblank_effect_ptr&& other) noexcept
{
    _palette = move(other._palette);
    _color_index = other._color_index;
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const color> sprite_palette_color_hblank_effect_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, display::height());
}

void sprite_palette_color_hblank_effect_ptr::set_colors_ref(
        const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void sprite_palette_color_hblank_effect_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_palette_color_hblank_effect_ptr::swap(sprite_palette_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    _palette.swap(other._palette);
    btn::swap(_color_index, other._color_index);
}

sprite_palette_color_hblank_effect_ptr::sprite_palette_color_hblank_effect_ptr(
        int id, int color_index, sprite_palette_ptr&& palette) :
    hblank_effect_ptr(id),
    _palette(move(palette)),
    _color_index(color_index)
{
}

}
