#include "btn_display.h"

#include "btn_display_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn::display
{

int width()
{
    return hw::display::width();
}

int height()
{
    return hw::display::height();
}

size dimensions()
{
    return hw::display::dimensions();
}

bool green_swap_enabled()
{
    return display_manager::green_swap_enabled();
}

void set_green_swap_enabled(bool enabled)
{
    display_manager::set_green_swap_enabled(enabled);
}

}
