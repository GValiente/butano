/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dp_direct_bitmap_bg_pivot_position_hbe_ptr.h"

#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

dp_direct_bitmap_bg_pivot_position_hbe_ptr dp_direct_bitmap_bg_pivot_position_hbe_ptr::create_horizontal(
        dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(
            deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
            hblank_effects_manager::handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION);

    return dp_direct_bitmap_bg_pivot_position_hbe_ptr(id, move(bg));
}

dp_direct_bitmap_bg_pivot_position_hbe_ptr dp_direct_bitmap_bg_pivot_position_hbe_ptr::create_vertical(
        dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(
            deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
            hblank_effects_manager::handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION);

    return dp_direct_bitmap_bg_pivot_position_hbe_ptr(id, move(bg));
}

optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> dp_direct_bitmap_bg_pivot_position_hbe_ptr::create_horizontal_optional(
        dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(
            deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
            hblank_effects_manager::handler_type::AFFINE_BG_PIVOT_HORIZONTAL_POSITION);

    optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = dp_direct_bitmap_bg_pivot_position_hbe_ptr(id, move(bg));
    }

    return result;
}

optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> dp_direct_bitmap_bg_pivot_position_hbe_ptr::create_vertical_optional(
        dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(
            deltas_ref.data(), deltas_ref.size(), intptr_t(bg.handle()),
            hblank_effects_manager::handler_type::AFFINE_BG_PIVOT_VERTICAL_POSITION);

    optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = dp_direct_bitmap_bg_pivot_position_hbe_ptr(id, move(bg));
    }

    return result;
}

span<const fixed> dp_direct_bitmap_bg_pivot_position_hbe_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void dp_direct_bitmap_bg_pivot_position_hbe_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void dp_direct_bitmap_bg_pivot_position_hbe_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void dp_direct_bitmap_bg_pivot_position_hbe_ptr::swap(dp_direct_bitmap_bg_pivot_position_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _bg.swap(other._bg);
}

dp_direct_bitmap_bg_pivot_position_hbe_ptr::dp_direct_bitmap_bg_pivot_position_hbe_ptr(
        int id, dp_direct_bitmap_bg_ptr&& bg) :
    hbe_ptr(id),
    _bg(move(bg))
{
}

}
