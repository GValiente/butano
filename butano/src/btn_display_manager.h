#ifndef BTN_DISPLAY_MANAGER_H
#define BTN_DISPLAY_MANAGER_H

#include "btn_common.h"

namespace btn::display_manager
{
    void init();

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void commit();

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
