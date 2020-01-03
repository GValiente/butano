#include "btn_display_manager.h"

#include "../hw/include/btn_hw_display.h"

namespace btn::display_manager
{

namespace
{
    class static_data
    {

    public:
        bool green_swap_enabled = false;
        bool commit = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::display::init();
}

bool green_swap_enabled()
{
    return data.green_swap_enabled;
}

void set_green_swap_enabled(bool enabled)
{
    data.green_swap_enabled = enabled;
    data.commit = true;
}

void commit()
{
    if(data.commit)
    {
        hw::display::set_green_swap_enabled(data.green_swap_enabled);
        data.commit = false;
    }
}

void sleep()
{
    hw::display::sleep();
}

void wake_up()
{
    hw::display::wake_up();
}

void set_show_mode()
{
    hw::display::set_show_mode();
}

}
