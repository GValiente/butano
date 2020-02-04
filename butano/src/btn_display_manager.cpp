#include "btn_display_manager.h"

#include "btn_fixed.h"
#include "btn_vector.h"
#include "../hw/include/btn_hw_bgs.h"
#include "../hw/include/btn_hw_display.h"

namespace btn::display_manager
{

namespace
{
    class static_data
    {

    public:
        bool bg_enabled[hw::bgs::count()] = { false };
        fixed sprites_mosaic_horizontal_stretch;
        fixed sprites_mosaic_vertical_stretch;
        fixed bgs_mosaic_horizontal_stretch;
        fixed bgs_mosaic_vertical_stretch;
        bool blending_bgs[hw::bgs::count()] = { false };
        fixed blending_transparency_alpha = 1;
        fixed blending_intensity_alpha = 0;
        bool commit_bg[hw::bgs::count()] = { false };
        bool green_swap_enabled = false;
        bool commit_mosaic = false;
        bool commit_blending_bgs = true;
        bool commit_blending_alphas = true;
        bool commit_green_swap = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::display::init();
}

bool bg_enabled(int bg)
{
    return data.bg_enabled[bg];
}

void set_bg_enabled(int bg, bool enabled)
{
    data.bg_enabled[bg] = enabled;
    data.commit_bg[bg] = true;
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

bool blending_bg_enabled(int bg_id)
{
    return data.blending_bgs[bg_id];
}

void set_blending_bg_enabled(int bg_id, bool enabled)
{
    data.blending_bgs[bg_id] = enabled;
    data.commit_blending_bgs = true;
}

fixed blending_transparency_alpha()
{
    return data.blending_transparency_alpha;
}

void set_blending_transparency_alpha(fixed transparency_alpha)
{
    data.blending_transparency_alpha = transparency_alpha;
    data.commit_blending_alphas = true;
}

fixed blending_intensity_alpha()
{
    return data.blending_intensity_alpha;
}

void set_blending_intensity_alpha(fixed intensity_alpha)
{
    data.blending_intensity_alpha = intensity_alpha;
    data.commit_blending_alphas = true;
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
    for(int index = 0; index < hw::bgs::count(); ++index)
    {
        if(data.commit_bg[index])
        {
            hw::display::set_bg_enabled(index, data.bg_enabled[index]);
            data.commit_bg[index] = false;
        }
    }

    if(data.commit_mosaic)
    {
        hw::display::set_mosaic(fixed_t<4>(data.sprites_mosaic_horizontal_stretch).value(),
                                fixed_t<4>(data.sprites_mosaic_vertical_stretch).value(),
                                fixed_t<4>(data.bgs_mosaic_horizontal_stretch).value(),
                                fixed_t<4>(data.bgs_mosaic_vertical_stretch).value());
        data.commit_mosaic = false;
    }

    if(data.commit_blending_bgs)
    {
        hw::display::set_blending_bgs(data.blending_bgs[0], hw::bgs::count());
        data.commit_blending_bgs = false;
    }

    if(data.commit_blending_alphas)
    {
        hw::display::set_blending_alphas(fixed_t<4>(data.blending_transparency_alpha).value(),
                                         fixed_t<4>(data.blending_intensity_alpha).value());
        data.commit_blending_alphas = false;
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
