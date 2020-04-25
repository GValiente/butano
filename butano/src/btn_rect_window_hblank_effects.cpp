#include "btn_rect_window_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effect_handler.h"
#include "btn_display_manager.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

namespace
{
    class horizontal_boundaries_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        horizontal_boundaries_hblank_effect_handler() = default;

        [[nodiscard]] bool target_visible(int) final
        {
            return true;
        }

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = pair<fixed, fixed>();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            pair<fixed, fixed>& last_value = target_last_value.value<pair<fixed, fixed>>();
            pair<fixed, fixed> new_value = display_manager::rect_window_hw_horizontal_boundaries(target_id);
            bool updated = last_value.first.integer() != new_value.first.integer() ||
                    last_value.second.integer() != new_value.second.integer();
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            return hw::display::window_horizontal_boundaries_register(target_id);
        }

        void write_output_values(int, const iany& target_last_value, bool, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            const pair<fixed, fixed>& last_value = target_last_value.value<pair<fixed, fixed>>();
            auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(input_values_ptr);
            display_manager::fill_rect_window_hblank_effect_horizontal_boundaries(
                        last_value, fixed_pairs_ptr, output_values_ptr);
        }
    };


    class vertical_boundaries_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        vertical_boundaries_hblank_effect_handler() = default;

        [[nodiscard]] bool target_visible(int) final
        {
            return true;
        }

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = pair<fixed, fixed>();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            pair<fixed, fixed>& last_value = target_last_value.value<pair<fixed, fixed>>();
            pair<fixed, fixed> new_value = display_manager::rect_window_hw_vertical_boundaries(target_id);
            bool updated = last_value.first.integer() != new_value.first.integer() ||
                    last_value.second.integer() != new_value.second.integer();
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            return hw::display::window_vertical_boundaries_register(target_id);
        }

        void write_output_values(int, const iany& target_last_value, bool, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            const pair<fixed, fixed>& last_value = target_last_value.value<pair<fixed, fixed>>();
            auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(input_values_ptr);
            display_manager::fill_rect_window_hblank_effect_vertical_boundaries(
                        last_value, fixed_pairs_ptr, output_values_ptr);
        }
    };


    class static_data
    {

    public:
        horizontal_boundaries_hblank_effect_handler horizontal_boundaries_handler;
        vertical_boundaries_hblank_effect_handler vertical_boundaries_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

rect_window_boundaries_hblank_effect_ptr rect_window_boundaries_hblank_effect_ptr::create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                            data.horizontal_boundaries_handler);
    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::optional_create_horizontal(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::optional_create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     data.horizontal_boundaries_handler);
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
                                            data.vertical_boundaries_handler);
    return rect_window_boundaries_hblank_effect_ptr(id, window);
}

optional<rect_window_boundaries_hblank_effect_ptr> rect_window_boundaries_hblank_effect_ptr::optional_create_vertical(
        rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)
{
    int id = hblank_effects_manager::optional_create(deltas_ref.data(), deltas_ref.size(), window.id(),
                                                     data.vertical_boundaries_handler);
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
    auto values_ptr = reinterpret_cast<const pair<fixed, fixed>*>(hblank_effects_manager::values_ref(id()));
    return span<const pair<fixed, fixed>>(values_ptr, hblank_effects_manager::values_count(id()));
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
