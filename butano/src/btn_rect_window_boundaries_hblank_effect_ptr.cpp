/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_rect_window_boundaries_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

rect_window_boundaries_hblank_effect_ptr rect_window_boundaries_hblank_effect_ptr::create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                            hblank_effects_manager::handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES);
    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::create_horizontal_optional(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     hblank_effects_manager::handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES);
    optional<rect_window_boundaries_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = rect_window_boundaries_hblank_effect_ptr(id, window);
    }

    return result;
}

rect_window_boundaries_hblank_effect_ptr rect_window_boundaries_hblank_effect_ptr::create_vertical(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                            hblank_effects_manager::handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES);
    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::create_vertical_optional(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     hblank_effects_manager::handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES);
    optional<rect_window_boundaries_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = rect_window_boundaries_hblank_effect_ptr(id, window);
    }

    return result;
}

span<const pair<fixed, fixed>> rect_window_boundaries_hblank_effect_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const pair<fixed, fixed>*>(hblank_effects_manager::values_ref(id()));
    return span<const pair<fixed, fixed>>(values_ptr, display::height());
}

void rect_window_boundaries_hblank_effect_ptr::set_deltas_ref(const span<const pair<fixed, fixed>>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void rect_window_boundaries_hblank_effect_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void rect_window_boundaries_hblank_effect_ptr::swap(rect_window_boundaries_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_window, other._window);
}

rect_window_boundaries_hblank_effect_ptr::rect_window_boundaries_hblank_effect_ptr(int id, rect_window window) :
    hblank_effect_ptr(id),
    _window(window)
{
}

}
