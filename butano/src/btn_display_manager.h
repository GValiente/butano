#ifndef BTN_DISPLAY_MANAGER_H
#define BTN_DISPLAY_MANAGER_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"

namespace btn
{
    class fixed_point;
}

namespace btn::display_manager
{
    void init();

    [[nodiscard]] bool bg_enabled(int bg);

    void set_bg_enabled(int bg, bool enabled);

    [[nodiscard]] fixed sprites_mosaic_horizontal_stretch();

    void set_sprites_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed sprites_mosaic_vertical_stretch();

    void set_sprites_mosaic_vertical_stretch(fixed stretch);

    void set_sprites_mosaic_stretch(fixed stretch);

    void set_sprites_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch);

    [[nodiscard]] fixed bgs_mosaic_horizontal_stretch();

    void set_bgs_mosaic_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed bgs_mosaic_vertical_stretch();

    void set_bgs_mosaic_vertical_stretch(fixed stretch);

    void set_bgs_mosaic_stretch(fixed stretch);

    void set_bgs_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch);

    [[nodiscard]] bool blending_bg_enabled(int bg);

    void set_blending_bg_enabled(int bg, bool enabled);

    [[nodiscard]] fixed blending_transparency_alpha();

    void set_blending_transparency_alpha(fixed transparency_alpha);

    [[nodiscard]] fixed blending_intensity_alpha();

    void set_blending_intensity_alpha(fixed intensity_alpha);

    [[nodiscard]] bool show_bg_in_window(int window, int bg);

    void set_show_bg_in_window(int window, int bg, bool show);

    void set_show_bg_in_all_windows(int bg, bool show);

    [[nodiscard]] bool show_sprites_in_window(int window);

    void set_show_sprites_in_window(int window, bool show);

    [[nodiscard]] bool show_blending_in_window(int window);

    void set_show_blending_in_window(int window, bool show);

    [[nodiscard]] const fixed_point& rect_window_top_left(int window);

    [[nodiscard]] const fixed_point& rect_window_bottom_right(int window);

    [[nodiscard]] pair<int, int> rect_window_hw_horizontal_boundaries(int window);

    [[nodiscard]] pair<int, int> rect_window_hw_vertical_boundaries(int window);

    void set_rect_window_top_left(int window, const fixed_point& top_left);

    void set_rect_window_bottom_right(int window, const fixed_point& bottom_right);

    [[nodiscard]] bool rect_window_ignore_camera(int window);

    void set_rect_window_ignore_camera(int window, bool ignore_camera);

    [[nodiscard]] bool inside_window_enabled(int window);

    void set_inside_window_enabled(int window, bool enabled);

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void update_camera();

    void commit();

    void sleep();

    void wake_up();

    void set_show_mode();
}

#endif
