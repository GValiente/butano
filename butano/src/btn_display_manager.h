#ifndef BTN_DISPLAY_MANAGER_H
#define BTN_DISPLAY_MANAGER_H

#include "btn_fixed_fwd.h"

namespace btn::display_manager
{
    void init();

    [[nodiscard]] bool bg_enabled(int bg);

    void set_bg_enabled(int bg, bool enabled);

    [[nodiscard]] fixed sprites_mosaic_horizontal_stretch();

    void set_sprites_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed sprites_mosaic_vertical_stretch();

    void set_sprites_mosaic_vertical_stretch(fixed stretch);

    [[nodiscard]] fixed bgs_mosaic_horizontal_stretch();

    void set_bgs_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed bgs_mosaic_vertical_stretch();

    void set_bgs_mosaic_vertical_stretch(fixed stretch);

    [[nodiscard]] bool blending_bg_enabled(int bg_id);

    void set_blending_bg_enabled(int bg_id, bool enabled);

    [[nodiscard]] fixed blending_transparency_alpha();

    void set_blending_transparency_alpha(fixed transparency_alpha);

    [[nodiscard]] fixed blending_intensity_alpha();

    void set_blending_intensity_alpha(fixed intensity_alpha);

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void commit();

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
