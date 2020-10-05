#include "btn_regular_bg_position_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_bgs_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn
{

namespace
{
    class horizontal_position_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        horizontal_position_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = fixed();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return bgs_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            auto handle = reinterpret_cast<void*>(target_id);
            fixed new_value = bgs_manager::hw_position(handle).x();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::bgs::regular_horizontal_position_register(*bgs_manager::hw_id(handle));
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_horizontal_positions(last_value, fixed_values_ptr, output_values_ptr);
        }

        void show(int) final
        {
        }

        void cleanup(int) final
        {
            bgs_manager::reload();
        }
    };


    class vertical_position_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        vertical_position_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = fixed();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return bgs_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            auto handle = reinterpret_cast<void*>(target_id);
            fixed new_value = bgs_manager::hw_position(handle).y();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::bgs::regular_vertical_position_register(*bgs_manager::hw_id(handle));
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_vertical_positions(last_value, fixed_values_ptr, output_values_ptr);
        }

        void show(int) final
        {
        }

        void cleanup(int) final
        {
            bgs_manager::reload();
        }
    };


    class static_data
    {

    public:
        horizontal_position_hblank_effect_handler horizontal_position_handler;
        vertical_position_hblank_effect_handler vertical_position_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

regular_bg_position_hblank_effect_ptr regular_bg_position_hblank_effect_ptr::create_horizontal(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(bg.handle()),
                                            data.horizontal_position_handler);
    return regular_bg_position_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::create_horizontal_optional(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(bg.handle()),
                                                     data.horizontal_position_handler);
    optional<regular_bg_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

regular_bg_position_hblank_effect_ptr regular_bg_position_hblank_effect_ptr::create_vertical(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(bg.handle()),
                                            data.vertical_position_handler);
    return regular_bg_position_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::create_vertical_optional(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(bg.handle()),
                                                     data.vertical_position_handler);
    optional<regular_bg_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(
        regular_bg_position_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _bg(move(other._bg))
{
}

regular_bg_position_hblank_effect_ptr& regular_bg_position_hblank_effect_ptr::operator=(
        regular_bg_position_hblank_effect_ptr&& other) noexcept
{
    _bg = move(other._bg);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const fixed> regular_bg_position_hblank_effect_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void regular_bg_position_hblank_effect_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void regular_bg_position_hblank_effect_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_position_hblank_effect_ptr::swap(regular_bg_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_bg, other._bg);
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(int id, regular_bg_ptr&& bg) :
    hblank_effect_ptr(id),
    _bg(move(bg))
{
}

}
