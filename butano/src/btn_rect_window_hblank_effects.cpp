#include "btn_rect_window_hblank_effects.h"

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

rect_window_boundaries_hblank_effect_ptr rect_window_boundaries_hblank_effect_ptr::create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES;
    int id = hblank_effects_manager::create(deltas_ref, target_type, window.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::optional_create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES;
    int id = hblank_effects_manager::create(deltas_ref, target_type, window.id());
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
    auto target_type = hblank_effects_manager::target_type::RECT_WINDOW_VERTICAL_BOUNDARIES;
    int id = hblank_effects_manager::create(deltas_ref, target_type, window.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::optional_create_vertical(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::RECT_WINDOW_VERTICAL_BOUNDARIES;
    int id = hblank_effects_manager::create(deltas_ref, target_type, window.id());
    optional<rect_window_boundaries_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = rect_window_boundaries_hblank_effect_ptr(id, window);
    }

    return result;
}

rect_window_boundaries_hblank_effect_ptr::rect_window_boundaries_hblank_effect_ptr(
        const rect_window_boundaries_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _window(other._window)
{
}

rect_window_boundaries_hblank_effect_ptr& rect_window_boundaries_hblank_effect_ptr::operator=(
        const rect_window_boundaries_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _window = other._window;
    return *this;
}

rect_window_boundaries_hblank_effect_ptr::rect_window_boundaries_hblank_effect_ptr(
        rect_window_boundaries_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _window(move(other._window))
{
}

rect_window_boundaries_hblank_effect_ptr& rect_window_boundaries_hblank_effect_ptr::operator=(
        rect_window_boundaries_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _window = move(other._window);
    return *this;
}

span<const pair<fixed, fixed>> rect_window_boundaries_hblank_effect_ptr::deltas_ref() const
{
    return hblank_effects_manager::fixed_pairs_ref(id());
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
