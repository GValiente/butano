#ifndef BTN_DISPLAY_MANAGER_H
#define BTN_DISPLAY_MANAGER_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_config_camera.h"

namespace btn
{
    class fixed_point;
}

namespace btn::display_manager
{
    using bg_handle_type = void*;

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

    [[nodiscard]] bool show_bg_in_window(int window, bg_handle_type bg_handle);

    void set_show_bg_in_window(int window, bg_handle_type bg_handle, bool show);

    void set_show_bg_in_all_windows(bg_handle_type bg_handle, bool show);

    void update_windows_visible_bgs();

    [[nodiscard]] bool show_sprites_in_window(int window);

    void set_show_sprites_in_window(int window, bool show);

    [[nodiscard]] bool show_blending_in_window(int window);

    void set_show_blending_in_window(int window, bool show);

    [[nodiscard]] bool show_all_in_window(int window);

    void set_show_all_in_window(int window);

    [[nodiscard]] bool show_nothing_in_window(int window);

    void set_show_nothing_in_window(int window);

    [[nodiscard]] const fixed_point& rect_window_top_left(int window);

    [[nodiscard]] const fixed_point& rect_window_bottom_right(int window);

    [[nodiscard]] pair<fixed, fixed> rect_window_hw_horizontal_boundaries(int window);

    [[nodiscard]] pair<fixed, fixed> rect_window_hw_vertical_boundaries(int window);

    void set_rect_window_top_left(int window, const fixed_point& top_left);

    void set_rect_window_bottom_right(int window, const fixed_point& bottom_right);

    #if BTN_CFG_CAMERA_ENABLED
        [[nodiscard]] bool rect_window_ignore_camera(int window);

        void set_rect_window_ignore_camera(int window, bool ignore_camera);
    #endif

    void fill_rect_window_hblank_effect_horizontal_boundaries(
            pair<fixed, fixed> base_horizontal_boundaries, const pair<fixed, fixed>* horizontal_boundaries_ptr,
            uint16_t* dest_ptr);

    void fill_rect_window_hblank_effect_vertical_boundaries(
            pair<fixed, fixed> base_vertical_boundaries, const pair<fixed, fixed>* vertical_boundaries_ptr,
            uint16_t* dest_ptr);

    [[nodiscard]] bool inside_window_enabled(int window);

    void set_inside_window_enabled(int window, bool enabled);

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void fill_green_swap_hblank_effect_states(const bool* states_ptr, uint16_t* dest_ptr);

    #if BTN_CFG_CAMERA_ENABLED
        void update_camera();
    #endif

    void update();

    void commit();

    void sleep();

    void wake_up();

    void stop();

    void set_show_mode();
}

#endif
