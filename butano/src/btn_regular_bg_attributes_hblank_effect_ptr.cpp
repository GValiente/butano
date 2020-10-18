#include "btn_regular_bg_attributes_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
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
            auto handle = reinterpret_cast<void*>(target_id);
            return bgs_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            last_value_type& last_value = target_last_value.value<last_value_type>();
            last_value_type new_value = last_value_type(target_id);
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::bgs::attributes_register(*bgs_manager::hw_id(handle));
        }

        void write_output_values(int target_id, const iany&, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            auto regular_bg_attributes_ptr = reinterpret_cast<const regular_bg_attributes*>(input_values_ptr);
            bgs_manager::fill_hblank_effect_regular_attributes(
                        handle, regular_bg_attributes_ptr, output_values_ptr);
        }

        void show(int) final
        {
        }

        void cleanup(int) final
        {
            bgs_manager::reload();
        }

    private:
        class alignas(int) last_value_type
        {

        public:
            explicit last_value_type(const regular_bg_map_ptr& target_map_ptr) :
                _map_dimensions(target_map_ptr.dimensions()),
                _tiles_cbb(target_map_ptr.tiles().cbb()),
                _bpp_mode(target_map_ptr.bpp_mode())
            {
            }

            explicit last_value_type(void* handle) :
                last_value_type(bgs_manager::map(handle))
            {
            }

            explicit last_value_type(int target_id) :
                last_value_type(reinterpret_cast<void*>(target_id))
            {
            }

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;

        private:
            size _map_dimensions;
            int _tiles_cbb;
            palette_bpp_mode _bpp_mode;
        };
    };


    class static_data
    {

    public:
        attributes_hblank_effect_handler attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

regular_bg_attributes_hblank_effect_ptr regular_bg_attributes_hblank_effect_ptr::create(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), int(bg.handle()),
                                            data.attributes_handler);
    return regular_bg_attributes_hblank_effect_ptr(id, move(bg));
}

optional<regular_bg_attributes_hblank_effect_ptr> regular_bg_attributes_hblank_effect_ptr::create_optional(
        regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), int(bg.handle()),
                                                     data.attributes_handler);
    optional<regular_bg_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_attributes_hblank_effect_ptr(id, move(bg));
    }

    return result;
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(
        regular_bg_attributes_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _bg(move(other._bg))
{
}

regular_bg_attributes_hblank_effect_ptr& regular_bg_attributes_hblank_effect_ptr::operator=(
        regular_bg_attributes_hblank_effect_ptr&& other) noexcept
{
    _bg = move(other._bg);
    hblank_effect_ptr::operator=(move(other));
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
