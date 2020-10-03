#include "btn_mosaic_attributes_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_display_manager.h"
#include "btn_mosaic_attributes.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

namespace
{
    class attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        attributes_hblank_effect_handler() = default;

        [[nodiscard]] bool target_visible(int) final
        {
            return true;
        }

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = mosaic_attributes();
        }

        [[nodiscard]] bool target_updated(int, iany& target_last_value) final
        {
            mosaic_attributes& last_value = target_last_value.value<mosaic_attributes>();
            mosaic_attributes new_value(display_manager::sprites_mosaic_horizontal_stretch(),
                                        display_manager::sprites_mosaic_vertical_stretch(),
                                        display_manager::bgs_mosaic_horizontal_stretch(),
                                        display_manager::bgs_mosaic_vertical_stretch());
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int) final
        {
            return hw::display::mosaic_register();
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            auto attributes_ptr = reinterpret_cast<const mosaic_attributes*>(input_values_ptr);
            display_manager::fill_mosaic_hblank_effect_attributes(attributes_ptr, output_values_ptr);
        }

        void cleanup(int) final
        {
            display_manager::reload_mosaic();
        }
    };

    class static_data
    {

    public:
        attributes_hblank_effect_handler attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

mosaic_attributes_hblank_effect_ptr mosaic_attributes_hblank_effect_ptr::create(
        const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), 0, data.attributes_handler);
    return mosaic_attributes_hblank_effect_ptr(id);
}

optional<mosaic_attributes_hblank_effect_ptr> mosaic_attributes_hblank_effect_ptr::create_optional(
        const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), 0,
                                                     data.attributes_handler);
    optional<mosaic_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = mosaic_attributes_hblank_effect_ptr(id);
    }

    return result;
}

span<const mosaic_attributes> mosaic_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const mosaic_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const mosaic_attributes>(values_ptr, display::height());
}

void mosaic_attributes_hblank_effect_ptr::set_attributes_ref(const span<const mosaic_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void mosaic_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

mosaic_attributes_hblank_effect_ptr::mosaic_attributes_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
