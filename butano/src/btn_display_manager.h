#ifndef BTN_DISPLAY_MANAGER_H
#define BTN_DISPLAY_MANAGER_H

#include "btn_fixed_fwd.h"

namespace btn::display_manager
{
    void init();

    [[nodiscard]] fixed sprites_mosaic_horizontal_stretch();

    void set_sprites_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed sprites_mosaic_vertical_stretch();

    void set_sprites_mosaic_vertical_stretch(fixed stretch);

    [[nodiscard]] fixed bgs_mosaic_horizontal_stretch();

    void set_bgs_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed bgs_mosaic_vertical_stretch();

    void set_bgs_mosaic_vertical_stretch(fixed stretch);

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void commit();

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
