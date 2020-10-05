#include "btn_blending_fade_alpha_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_display_manager.h"
#include "btn_blending_fade_alpha.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

namespace
{
    class values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        values_hblank_effect_handler() = default;

        [[nodiscard]] bool target_visible(int) final
        {
            return true;
        }

        void setup_target(int, iany&) final
        {
        }

        [[nodiscard]] bool target_updated(int, iany&) final
        {
            BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
                       "Blending transparency and fade can't be enabled at the same time");
            BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
                       "Blending intensity and fade can't be enabled at the same time");

            return false;
        }

        [[nodiscard]] uint16_t* output_register(int) final
        {
            return hw::display::blending_fade_register();
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            auto values_ptr = reinterpret_cast<const blending_fade_alpha*>(input_values_ptr);
            display_manager::fill_blending_fade_hblank_effect_alphas(values_ptr, output_values_ptr);
        }

        void show(int) final
        {
            display_manager::blending_enable_fade();
        }

        void cleanup(int) final
        {
            display_manager::blending_disable_fade();
            display_manager::reload_blending_transparency();
        }
    };

    class static_data
    {

    public:
        values_hblank_effect_handler handler;
    };

    BTN_DATA_EWRAM static_data data;
}

blending_fade_alpha_hblank_effect_ptr blending_fade_alpha_hblank_effect_ptr::create(
        const span<const blending_fade_alpha>& values_ref)
{
    BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time");
    BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), 0, data.handler);
    return blending_fade_alpha_hblank_effect_ptr(id);
}

optional<blending_fade_alpha_hblank_effect_ptr> blending_fade_alpha_hblank_effect_ptr::create_optional(
        const span<const blending_fade_alpha>& values_ref)
{
    BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time");
    BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), 0, data.handler);
    optional<blending_fade_alpha_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = blending_fade_alpha_hblank_effect_ptr(id);
    }

    return result;
}

span<const blending_fade_alpha> blending_fade_alpha_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const blending_fade_alpha*>(hblank_effects_manager::values_ref(id()));
    return span<const blending_fade_alpha>(values_ptr, display::height());
}

void blending_fade_alpha_hblank_effect_ptr::set_values_ref(const span<const blending_fade_alpha>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void blending_fade_alpha_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

blending_fade_alpha_hblank_effect_ptr::blending_fade_alpha_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
