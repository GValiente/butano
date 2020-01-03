#include "../include/btn_hw_display.h"

#include "tonc.h"

namespace btn::hw::display
{

void init()
{
    // Show sprites in background mode 0:
    REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;
}

void set_green_swap_enabled(bool enabled)
{
    if(enabled)
    {
        REG_DISPCNT |= 0x10000;
    }
    else
    {
        REG_DISPCNT &= ~0x10000;
    }
}

void sleep()
{
    REG_DISPCNT |= DCNT_BLANK;
}

void wake_up()
{
    REG_DISPCNT &= unsigned(~DCNT_BLANK);
}

void set_show_mode()
{
    // Show Background 2 in mode 3:
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
}

}
