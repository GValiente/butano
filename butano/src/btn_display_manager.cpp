#include "btn_display_manager.h"

#include "btn_vector.h"
#include "btn_camera.h"
#include "btn_fixed_point.h"
#include "../hw/include/btn_hw_bgs.h"
#include "../hw/include/btn_hw_display.h"

namespace btn::display_manager
{

namespace
{
    class static_data
    {

    public:
        bool bg_enabled[hw::bgs::count()] = {};
        fixed sprites_mosaic_horizontal_stretch;
        fixed sprites_mosaic_vertical_stretch;
        fixed bgs_mosaic_horizontal_stretch;
        fixed bgs_mosaic_vertical_stretch;
        bool blending_bgs[hw::bgs::count()] = {};
        fixed blending_transparency_alpha = 1;
        fixed blending_intensity_alpha = 0;
        unsigned windows_flags[hw::display::windows_count()];
        fixed_point rect_windows_boundaries[hw::display::rect_windows_count() * 2];
        bool rect_windows_ignore_camera[hw::display::rect_windows_count() * 2] = {};
        bool inside_window_enabled[hw::display::inside_windows_count()] = {};
        bool commit_bg[hw::bgs::count()] = {};
        bool green_swap_enabled = false;
        bool commit_mosaic = false;
        bool commit_blending_bgs = true;
        bool commit_blending_alphas = true;
        bool commit_windows_flags = true;
        bool commit_windows_boundaries = false;
        bool commit_inside_window[hw::display::inside_windows_count()] = {};
        bool commit_green_swap = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::display::init();

    unsigned initial_window_flags =
            unsigned(hw::display::window_flag::BG_0) |
            unsigned(hw::display::window_flag::BG_1) |
            unsigned(hw::display::window_flag::BG_2) |
            unsigned(hw::display::window_flag::BG_3) |
            unsigned(hw::display::window_flag::SPRITES) |
            unsigned(hw::display::window_flag::BLENDING);

    for(unsigned& window_flags : data.windows_flags)
    {
        window_flags = initial_window_flags;
    }

    for(bool& inside_window_enabled : data.inside_window_enabled)
    {
        inside_window_enabled = true;
    }

    for(bool& commit_inside_window : data.commit_inside_window)
    {
        commit_inside_window = true;
    }
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

void set_sprites_mosaic_stretch(fixed stretch)
{
    data.sprites_mosaic_horizontal_stretch = stretch;
    data.sprites_mosaic_vertical_stretch = stretch;
    data.commit_mosaic = true;
}

void set_sprites_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch)
{
    data.sprites_mosaic_horizontal_stretch = horizontal_stretch;
    data.sprites_mosaic_vertical_stretch = vertical_stretch;
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

void set_bgs_mosaic_stretch(fixed stretch)
{
    data.bgs_mosaic_horizontal_stretch = stretch;
    data.bgs_mosaic_vertical_stretch = stretch;
    data.commit_mosaic = true;
}

void set_bgs_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch)
{
    data.bgs_mosaic_horizontal_stretch = horizontal_stretch;
    data.bgs_mosaic_vertical_stretch = vertical_stretch;
    data.commit_mosaic = true;
}

bool blending_bg_enabled(int bg)
{
    return data.blending_bgs[bg];
}

void set_blending_bg_enabled(int bg, bool enabled)
{
    data.blending_bgs[bg] = enabled;
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

bool show_bg_in_window(int window, int bg)
{
    return data.windows_flags[window] & (unsigned(hw::display::window_flag::BG_0) << bg);
}

void set_show_bg_in_window(int window, int bg, bool show)
{
    if(show)
    {
        data.windows_flags[window] |= (unsigned(hw::display::window_flag::BG_0) << bg);
    }
    else
    {
        data.windows_flags[window] &= ~(unsigned(hw::display::window_flag::BG_0) << bg);
    }

    data.commit_windows_flags = true;
}

void set_show_bg_in_all_windows(int bg, bool show)
{
    unsigned bg_window_flag = unsigned(hw::display::window_flag::BG_0) << bg;

    if(show)
    {
        for(unsigned& window_flags : data.windows_flags)
        {
            window_flags |= bg_window_flag;
        }
    }
    else
    {
        for(unsigned& window_flags : data.windows_flags)
        {
            window_flags &= ~bg_window_flag;
        }
    }

    data.commit_windows_flags = true;
}

bool show_sprites_in_window(int window)
{
    return data.windows_flags[window] & unsigned(hw::display::window_flag::SPRITES);
}

void set_show_sprites_in_window(int window, bool show)
{
    if(show)
    {
        data.windows_flags[window] |= unsigned(hw::display::window_flag::SPRITES);
    }
    else
    {
        data.windows_flags[window] &= ~unsigned(hw::display::window_flag::SPRITES);
    }

    data.commit_windows_flags = true;
}

bool show_blending_in_window(int window)
{
    return data.windows_flags[window] & unsigned(hw::display::window_flag::BLENDING);
}

void set_show_blending_in_window(int window, bool show)
{
    if(show)
    {
        data.windows_flags[window] |= unsigned(hw::display::window_flag::BLENDING);
    }
    else
    {
        data.windows_flags[window] &= ~unsigned(hw::display::window_flag::BLENDING);
    }

    data.commit_windows_flags = true;
}

const fixed_point& rect_window_top_left(int window)
{
    return data.rect_windows_boundaries[window * 2];
}

const fixed_point& rect_window_bottom_right(int window)
{
    return data.rect_windows_boundaries[(window * 2) + 1];
}

void set_rect_window_boundaries(int window, const fixed_point& top_left, const fixed_point& bottom_right)
{
    int index = window * 2;
    data.rect_windows_boundaries[index] = top_left;
    data.rect_windows_boundaries[index + 1] = bottom_right;
    data.commit_windows_boundaries = true;
}

bool rect_window_ignore_camera(int window)
{
    return data.rect_windows_ignore_camera[window];
}

void set_rect_window_ignore_camera(int window, bool ignore_camera)
{
    data.rect_windows_ignore_camera[window] = ignore_camera;
    data.commit_windows_boundaries = true;
}

bool inside_window_enabled(int window)
{
    return data.inside_window_enabled[window];
}

void set_inside_window_enabled(int window, bool enabled)
{
    data.inside_window_enabled[window] = enabled;
    data.commit_inside_window[window] = true;
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

void update_camera()
{
    for(bool rect_window_ignore_camera : data.rect_windows_ignore_camera)
    {
        if(rect_window_ignore_camera)
        {
            data.commit_windows_boundaries = true;
            return;
        }
    }
}

void commit()
{
    for(int index = 0; index < hw::bgs::count(); ++index)
    {
        bool& commit_bg = data.commit_bg[index];

        if(commit_bg)
        {
            hw::display::set_bg_enabled(index, data.bg_enabled[index]);
            commit_bg = false;
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

    if(data.commit_windows_flags)
    {
        hw::display::set_windows_flags(data.windows_flags[0]);
        data.commit_windows_flags = false;
    }

    for(int index = 0; index < hw::display::inside_windows_count(); ++index)
    {
        bool& commit_inside_window = data.commit_inside_window[index];

        if(commit_inside_window)
        {
            hw::display::set_inside_window_enabled(index, data.inside_window_enabled[index]);
            commit_inside_window = false;
        }
    }

    if(data.commit_windows_boundaries)
    {
        fixed_point camera_position = camera::position();
        const fixed_point* windows_boundaries = data.rect_windows_boundaries;
        const bool* windows_ignore_camera = data.rect_windows_ignore_camera;
        constexpr int boundaries_count = hw::display::rect_windows_count() * 2;
        point hw_windows_boundaries[boundaries_count];
        int display_width = hw::display::width();
        int display_height = hw::display::height();
        int half_display_width = hw::display::width() / 2;
        int half_display_height = hw::display::height() / 2;

        for(int index = 0; index < boundaries_count; ++index)
        {
            fixed_point window_boundaries = windows_boundaries[index];

            if(! windows_ignore_camera[index])
            {
                window_boundaries -= camera_position;
            }

            point& hw_window_boundaries = hw_windows_boundaries[index];
            hw_window_boundaries.set_x(clamp(window_boundaries.x().integer() + half_display_width, 0, display_width));
            hw_window_boundaries.set_y(clamp(window_boundaries.y().integer() + half_display_height, 0, display_height));
        }

        hw::display::set_windows_boundaries(hw_windows_boundaries[0]);
        data.commit_windows_boundaries = false;
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
