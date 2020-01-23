#ifndef BTN_HW_DISPLAY_H
#define BTN_HW_DISPLAY_H

#include "tonc.h"
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

    inline void init()
    {
        REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D;
    }

    inline void set_bg_enabled(int bg, bool enabled)
    {
        if(enabled)
        {
            REG_DISPCNT |= unsigned(DCNT_BG0 << bg);
        }
        else
        {
            REG_DISPCNT &= unsigned(DCNT_BG0 << bg);
        }
    }

    inline void set_mosaic(int sprites_horizontal_stretch, int sprites_vertical_stretch,
                           int bgs_horizontal_stretch, int bgs_vertical_stretch)
    {
        REG_MOSAIC = MOS_BUILD(unsigned(bgs_horizontal_stretch), unsigned(bgs_vertical_stretch),
                               unsigned(sprites_horizontal_stretch), unsigned(sprites_vertical_stretch));
    }

    inline void set_green_swap_enabled(bool enabled)
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

    inline void sleep()
    {
        REG_DISPCNT |= DCNT_BLANK;
    }

    inline void wake_up()
    {
        REG_DISPCNT &= unsigned(~DCNT_BLANK);
    }

    inline void set_show_mode()
    {
        REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
    }
}

#endif
