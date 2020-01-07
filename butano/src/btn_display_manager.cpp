#include "btn_display_manager.h"

#include "btn_fixed.h"
#include "../hw/include/btn_hw_display.h"

namespace btn::display_manager
{

namespace
{
    class static_data
    {

    public:
        fixed sprites_mosaic_horizontal_stretch;
        fixed sprites_mosaic_vertical_stretch;
        fixed bgs_mosaic_horizontal_stretch;
        fixed bgs_mosaic_vertical_stretch;
        bool green_swap_enabled = false;
        bool commit_mosaic = false;
        bool commit_green_swap = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::display::init();
}

fixed sprites_mosaic_horizontal_stretch()
{
    return data.sprites_mosaic_horizontal_stretch;
}

void set_sprites_mosaic_horizontal_stretch(fixed stretch)
{
    data.sprites_mosaic_horizontal_stretch = stretch;
    data.commit_mosaic = true;
}

fixed sprites_mosaic_vertical_stretch()
{
    return data.sprites_mosaic_vertical_stretch;
}

void set_sprites_mosaic_vertical_stretch(fixed stretch)
{
    data.sprites_mosaic_vertical_stretch = stretch;
    data.commit_mosaic = true;
}

fixed bgs_mosaic_horizontal_stretch()
{
    return data.bgs_mosaic_horizontal_stretch;
}

void set_bgs_mosaic_horizontal_stretch(fixed stretch)
{
    data.bgs_mosaic_horizontal_stretch = stretch;
    data.commit_mosaic = true;
}

fixed bgs_mosaic_vertical_stretch()
{
    return data.bgs_mosaic_vertical_stretch;
}

void set_bgs_mosaic_vertical_stretch(fixed stretch)
{
    data.bgs_mosaic_vertical_stretch = stretch;
    data.commit_mosaic = true;
}

bool green_swap_enabled()
{
    return data.green_swap_enabled;
}

void set_green_swap_enabled(bool enabled)
{
    data.green_swap_enabled = enabled;
    data.commit_green_swap = true;
}

void commit()
{
    if(data.commit_mosaic)
    {
        hw::display::set_mosaic((data.sprites_mosaic_horizontal_stretch * 16).integer(),
                                (data.sprites_mosaic_vertical_stretch * 16).integer(),
                                (data.bgs_mosaic_horizontal_stretch * 16).integer(),
                                (data.bgs_mosaic_vertical_stretch * 16).integer());
        data.commit_mosaic = false;
    }

    if(data.commit_green_swap)
    {
        hw::display::set_green_swap_enabled(data.green_swap_enabled);
        data.commit_green_swap = false;
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
