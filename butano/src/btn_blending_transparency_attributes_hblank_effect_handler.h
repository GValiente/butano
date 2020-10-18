#ifndef BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H
#define BTN_BLENDING_TRANSPARENCY_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H

#include "btn_any_fwd.h"
#include "btn_blending_transparency_attributes.h"
#include "btn_display_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

class blending_transparency_attributes_hblank_effect_handler
{

public:
    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        BTN_ASSERT(! display_manager::blending_fade_enabled(),
                   "Blending transparency and fade can't be enabled at the same time");

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int)
    {
        return hw::display::blending_transparency_register();
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const blending_transparency_attributes*>(input_values_ptr);
        display_manager::fill_blending_transparency_hblank_effect_attributes(attributes_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        display_manager::reload_blending_transparency();
    }
};
}

#endif
