#include "btn_regular_bg_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_regular_bg_attributes.h"
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
            return bgs_manager::visible(target_id);
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            fixed new_value = bgs_manager::hw_position(target_id).x();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            return hw::bgs::regular_horizontal_position_register(target_id);
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_horizontal_positions(last_value, fixed_values_ptr, output_values_ptr);
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
            return bgs_manager::visible(target_id);
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            fixed new_value = bgs_manager::hw_position(target_id).y();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            return hw::bgs::regular_vertical_position_register(target_id);
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_vertical_positions(last_value, fixed_values_ptr, output_values_ptr);
        }
    };


    class attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        attributes_hblank_effect_handler() = default;

        void setup_target(int target_id, iany& target_last_value) final
        {
            target_last_value = last_value_type(target_id);
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            return bgs_manager::visible(target_id);
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            last_value_type& last_value = target_last_value.value<last_value_type>();
            last_value_type new_value = last_value_type(target_id);
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            return hw::bgs::attributes_register(target_id);
        }

        void write_output_values(int target_id, const iany&, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            auto regular_bg_attributes_ptr = reinterpret_cast<const regular_bg_attributes*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_regular_attributes(
                        target_id, regular_bg_attributes_ptr, output_values_ptr);
        }

    private:
        class alignas(alignof(int)) last_value_type
        {

        public:
            explicit last_value_type(const regular_bg_map_ptr& target_map_ptr) :
                _map_dimensions(target_map_ptr.dimensions()),
                _tiles_cbb(target_map_ptr.tiles().cbb()),
                _bpp_mode(target_map_ptr.bpp_mode())
            {
            }

            explicit last_value_type(int target_id) :
                last_value_type(bgs_manager::map(target_id))
            {
            }

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b)
            {
                return a._map_dimensions == b._map_dimensions && a._tiles_cbb == b._tiles_cbb &&
                        a._bpp_mode == b._bpp_mode;
            }

            [[nodiscard]] friend bool operator!=(const last_value_type& a, const last_value_type& b)
            {
                return ! (a == b);
            }

        private:
            size _map_dimensions;
            int _tiles_cbb;
            palette_bpp_mode _bpp_mode;
        };
    };


    class static_data
    {

    public:
        horizontal_position_hblank_effect_handler horizontal_position_handler;
        vertical_position_hblank_effect_handler vertical_position_handler;
        attributes_hblank_effect_handler attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

regular_bg_position_hblank_effect_ptr regular_bg_position_hblank_effect_ptr::create_horizontal(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), bg.id(),
                                            data.horizontal_position_handler);
    return regular_bg_position_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::optional_create_horizontal(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::optional_create(deltas_ref.data(), deltas_ref.size(), bg.id(),
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
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), bg.id(),
                                            data.vertical_position_handler);
    return regular_bg_position_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::optional_create_vertical(
        regular_bg_ptr bg, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::optional_create(deltas_ref.data(), deltas_ref.size(), bg.id(),
                                                     data.vertical_position_handler);
    optional<regular_bg_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(
        const regular_bg_position_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _bg(other._bg)
{
}

regular_bg_position_hblank_effect_ptr& regular_bg_position_hblank_effect_ptr::operator=(
        const regular_bg_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _bg = other._bg;
    return *this;
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(
        regular_bg_position_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _bg(move(other._bg))
{
}

regular_bg_position_hblank_effect_ptr& regular_bg_position_hblank_effect_ptr::operator=(
        regular_bg_position_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _bg = move(other._bg);
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

regular_bg_attributes_hblank_effect_ptr regular_bg_attributes_hblank_effect_ptr::create(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), bg.id(),
                                            data.attributes_handler);
    return regular_bg_attributes_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_attributes_hblank_effect_ptr> regular_bg_attributes_hblank_effect_ptr::optional_create(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(), bg.id(),
                                                     data.attributes_handler);
    optional<regular_bg_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_attributes_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(
        const regular_bg_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _bg(other._bg)
{
}

regular_bg_attributes_hblank_effect_ptr& regular_bg_attributes_hblank_effect_ptr::operator=(
        const regular_bg_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _bg = other._bg;
    return *this;
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(
        regular_bg_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _bg(move(other._bg))
{
}

regular_bg_attributes_hblank_effect_ptr& regular_bg_attributes_hblank_effect_ptr::operator=(
        regular_bg_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _bg = move(other._bg);
    return *this;
}

span<const regular_bg_attributes> regular_bg_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const regular_bg_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const regular_bg_attributes>(values_ptr, display::height());
}

void regular_bg_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const regular_bg_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void regular_bg_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_attributes_hblank_effect_ptr::swap(regular_bg_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_bg, other._bg);
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(int id, regular_bg_ptr&& bg) :
    hblank_effect_ptr(id),
    _bg(move(bg))
{
}

}
