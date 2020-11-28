/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_pivot_position_hblank_effect_ptr.h"

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

affine_bg_pivot_position_hblank_effect_ptr affine_bg_pivot_position_hblank_effect_ptr::create_horizontal(
        affine_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    return affine_bg_pivot_position_hblank_effect_ptr(
                affine_bg_pivot_position_high_hblank_effect_ptr::create_horizontal(bg, deltas_ref),
                affine_bg_pivot_position_low_hblank_effect_ptr::create_horizontal(bg, deltas_ref));
}

optional<affine_bg_pivot_position_hblank_effect_ptr> affine_bg_pivot_position_hblank_effect_ptr::create_horizontal_optional(
        affine_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    optional<affine_bg_pivot_position_hblank_effect_ptr> result;

    if(auto high = affine_bg_pivot_position_high_hblank_effect_ptr::create_horizontal_optional(bg, deltas_ref))
    {
        if(auto low = affine_bg_pivot_position_low_hblank_effect_ptr::create_horizontal_optional(move(bg), deltas_ref))
        {
            result = affine_bg_pivot_position_hblank_effect_ptr(move(*high), move(*low));
        }
    }

    return result;
}

affine_bg_pivot_position_hblank_effect_ptr affine_bg_pivot_position_hblank_effect_ptr::create_vertical(
        affine_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    return affine_bg_pivot_position_hblank_effect_ptr(
                affine_bg_pivot_position_high_hblank_effect_ptr::create_vertical(bg, deltas_ref),
                affine_bg_pivot_position_low_hblank_effect_ptr::create_vertical(bg, deltas_ref));
}

optional<affine_bg_pivot_position_hblank_effect_ptr> affine_bg_pivot_position_hblank_effect_ptr::create_vertical_optional(
        affine_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    optional<affine_bg_pivot_position_hblank_effect_ptr> result;

    if(auto high = affine_bg_pivot_position_high_hblank_effect_ptr::create_vertical_optional(bg, deltas_ref))
    {
        if(auto low = affine_bg_pivot_position_low_hblank_effect_ptr::create_vertical_optional(move(bg), deltas_ref))
        {
            result = affine_bg_pivot_position_hblank_effect_ptr(move(*high), move(*low));
        }
    }

    return result;
}

void affine_bg_pivot_position_hblank_effect_ptr::set_visible(bool visible)
{
    _high_hblank_effect_ptr.set_visible(visible);
    _low_hblank_effect_ptr.set_visible(visible);
}

span<const fixed> affine_bg_pivot_position_hblank_effect_ptr::deltas_ref() const
{
    return _high_hblank_effect_ptr.deltas_ref();
}

void affine_bg_pivot_position_hblank_effect_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    _high_hblank_effect_ptr.set_deltas_ref(deltas_ref);
    _low_hblank_effect_ptr.set_deltas_ref(deltas_ref);
}

void affine_bg_pivot_position_hblank_effect_ptr::reload_deltas_ref()
{
    _high_hblank_effect_ptr.reload_deltas_ref();
    _low_hblank_effect_ptr.reload_deltas_ref();
}

void affine_bg_pivot_position_hblank_effect_ptr::swap(affine_bg_pivot_position_hblank_effect_ptr& other)
{
    _high_hblank_effect_ptr.swap(other._high_hblank_effect_ptr);
    _low_hblank_effect_ptr.swap(other._low_hblank_effect_ptr);
}

affine_bg_pivot_position_hblank_effect_ptr::affine_bg_pivot_position_hblank_effect_ptr(
        affine_bg_pivot_position_high_hblank_effect_ptr&& high_hblank_effect_ptr,
        affine_bg_pivot_position_low_hblank_effect_ptr&& low_hblank_effect_ptr) :
    _high_hblank_effect_ptr(move(high_hblank_effect_ptr)),
    _low_hblank_effect_ptr(move(low_hblank_effect_ptr))
{
}

}
