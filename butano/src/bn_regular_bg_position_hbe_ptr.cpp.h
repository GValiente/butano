/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_regular_bg_position_hbe_ptr.h"

#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

regular_bg_position_hbe_ptr regular_bg_position_hbe_ptr::create_horizontal(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
                                            hblank_effects_manager::handler_type::REGULAR_BG_HORIZONTAL_POSITION);
    return regular_bg_position_hbe_ptr(id, move(bg));
}

regular_bg_position_hbe_ptr regular_bg_position_hbe_ptr::create_vertical(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
                                            hblank_effects_manager::handler_type::REGULAR_BG_VERTICAL_POSITION);
    return regular_bg_position_hbe_ptr(id, move(bg));
}

optional<regular_bg_position_hbe_ptr> regular_bg_position_hbe_ptr::create_horizontal_optional(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
                                                     hblank_effects_manager::handler_type::REGULAR_BG_HORIZONTAL_POSITION);
    optional<regular_bg_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hbe_ptr(id, move(bg));
    }

    return result;
}

optional<regular_bg_position_hbe_ptr> regular_bg_position_hbe_ptr::create_vertical_optional(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
                                                     hblank_effects_manager::handler_type::REGULAR_BG_VERTICAL_POSITION);
    optional<regular_bg_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hbe_ptr(id, move(bg));
    }

    return result;
}

span<const fixed> regular_bg_position_hbe_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void regular_bg_position_hbe_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void regular_bg_position_hbe_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_position_hbe_ptr::swap(regular_bg_position_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _bg.swap(other._bg);
}

regular_bg_position_hbe_ptr::regular_bg_position_hbe_ptr(int id, regular_bg_ptr&& bg) :
    hbe_ptr(id),
    _bg(move(bg))
{
}

}
