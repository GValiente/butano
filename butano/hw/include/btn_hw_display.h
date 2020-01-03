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

    void set_green_swap_enabled(bool enabled);

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
