#ifndef BTN_HW_DISPLAY_H
#define BTN_HW_DISPLAY_H

#include "btn_size.h"

namespace btn::hw::display
{
    [[nodiscard]] constexpr int width()
    {
        return 240;
    }

    [[nodiscard]] constexpr int height()
    {
        return 160;
    }

    [[nodiscard]] constexpr size dimensions()
    {
        return size(width(), height());
    }

    void init();

    void set_bg_enabled(int bg, bool enabled);

    void set_mosaic(int sprites_horizontal_stretch, int sprites_vertical_stretch,
                    int bgs_horizontal_stretch, int bgs_vertical_stretch);

    void set_green_swap_enabled(bool enabled);

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
