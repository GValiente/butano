#ifndef BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_HANDLER_H
#define BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_HANDLER_H

#include "btn_any_fwd.h"
#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

class bg_palettes_transparent_color_hblank_effect_handler
{

public:
    static void setup_target(int, iany&)
    {
    }

    [[nodiscard]] static bool target_visible(int)
    {
        return true;
    }

    [[nodiscard]] static bool target_updated(int, iany&)
    {
        if(optional<palettes_bank::commit_data> commit_data =
                palettes_manager::bg_palettes_bank().retrieve_commit_data())
        {
            return commit_data->offset == 0;
        }

        return false;
    }

    [[nodiscard]] static uint16_t* output_register(int)
    {
        return hw::palettes::bg_transparent_color_register();
    }

    static void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr)
    {
        palettes_manager::bg_palettes_bank().fill_hblank_effect_colors(
                    reinterpret_cast<const color*>(input_values_ptr), output_values_ptr);
    }

    static void show(int)
    {
    }

    static void cleanup(int)
    {
        palettes_manager::bg_palettes_bank().reload(0);
    }
};

}

#endif
