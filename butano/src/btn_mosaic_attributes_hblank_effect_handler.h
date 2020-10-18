#ifndef BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H
#define BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_HANDLER_H

#include "btn_any_fwd.h"
#include "btn_display_manager.h"
#include "btn_mosaic_attributes.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

class mosaic_attributes_hblank_effect_handler
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
        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int)
    {
        return hw::display::mosaic_register();
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        auto attributes_ptr = reinterpret_cast<const mosaic_attributes*>(input_values_ptr);
        display_manager::fill_mosaic_hblank_effect_attributes(attributes_ptr, output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        display_manager::reload_mosaic();
    }
};

}

#endif
