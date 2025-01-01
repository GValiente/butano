/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_rect_window_boundaries_hbe_ptr.h"

#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

rect_window_boundaries_hbe_ptr rect_window_boundaries_hbe_ptr::create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                            hblank_effects_manager::handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES);
    return rect_window_boundaries_hbe_ptr(id, window);
}

rect_window_boundaries_hbe_ptr rect_window_boundaries_hbe_ptr::create_vertical(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                            hblank_effects_manager::handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES);
    return rect_window_boundaries_hbe_ptr(id, window);
}

optional<rect_window_boundaries_hbe_ptr> rect_window_boundaries_hbe_ptr::create_horizontal_optional(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     hblank_effects_manager::handler_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES);
    optional<rect_window_boundaries_hbe_ptr> result;

    if(id >= 0)
    {
        result = rect_window_boundaries_hbe_ptr(id, window);
    }

    return result;
}

optional<rect_window_boundaries_hbe_ptr> rect_window_boundaries_hbe_ptr::create_vertical_optional(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     hblank_effects_manager::handler_type::RECT_WINDOW_VERTICAL_BOUNDARIES);
    optional<rect_window_boundaries_hbe_ptr> result;

    if(id >= 0)
    {
        result = rect_window_boundaries_hbe_ptr(id, window);
    }

    return result;
}

span<const pair<fixed, fixed>> rect_window_boundaries_hbe_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const pair<fixed, fixed>*>(hblank_effects_manager::values_ref(id()));
    return span<const pair<fixed, fixed>>(values_ptr, display::height());
}

void rect_window_boundaries_hbe_ptr::set_deltas_ref(const span<const pair<fixed, fixed>>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void rect_window_boundaries_hbe_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void rect_window_boundaries_hbe_ptr::swap(rect_window_boundaries_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    bn::swap(_window, other._window);
}

rect_window_boundaries_hbe_ptr::rect_window_boundaries_hbe_ptr(int id, rect_window window) :
    hbe_ptr(id),
    _window(window)
{
}

}
