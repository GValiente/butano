/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DISPLAY_MANAGER_H
#define BN_DISPLAY_MANAGER_H

#include "bn_utility.h"
#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"
#include "bn_fixed_point_fwd.h"

namespace bn
{
    class camera_ptr;
    class mosaic_attributes;
    class blending_fade_alpha;
    class blending_transparency_attributes;
}

namespace bn::display_manager
{
    void init();

    void set_mode(int mode);

    [[nodiscard]] bool sprites_visible();

    void set_sprites_visible(bool visible);

    [[nodiscard]] bool bg_enabled(int bg);

    void set_bg_enabled(int bg, bool enabled);

    void disable_all_bgs();

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

    void reload_mosaic();

    void fill_mosaic_hblank_effect_attributes(const mosaic_attributes* mosaic_attributes_ptr, uint16_t* dest_ptr);

    [[nodiscard]] bool blending_top_bg_enabled(int bg);

    void set_blending_top_bg_enabled(int bg, bool enabled);

    [[nodiscard]] bool blending_bottom_bg_enabled(int bg);

    void set_blending_bottom_bg_enabled(int bg, bool enabled);

    [[nodiscard]] bool blending_bottom_sprites_enabled();

    void set_blending_bottom_sprites_enabled(bool enabled);

    [[nodiscard]] bool blending_bottom_backdrop_enabled();

    void set_blending_bottom_backdrop_enabled(bool enabled);

    [[nodiscard]] fixed blending_transparency_alpha();

    void set_blending_transparency_alpha(fixed transparency_alpha);

    [[nodiscard]] fixed blending_intensity_alpha();

    void set_blending_intensity_alpha(fixed intensity_alpha);

    void set_blending_transparency_and_intensity_alpha(fixed transparency_alpha, fixed intensity_alpha);

    [[nodiscard]] fixed blending_transparency_top_weight();

    void set_blending_transparency_top_weight(fixed top_weight);

    [[nodiscard]] fixed blending_transparency_bottom_weight();

    void set_blending_transparency_bottom_weight(fixed bottom_weight);

    void set_blending_transparency_weights(fixed top_weight, fixed bottom_weight);

    void reload_blending_transparency();

    void fill_blending_transparency_hblank_effect_attributes(
            const blending_transparency_attributes* blending_transparency_attributes_ptr, uint16_t* dest_ptr);

    [[nodiscard]] bool blending_fade_enabled();

    void set_blending_fade_enabled(bool enabled);

    void blending_disable_fade();

    [[nodiscard]] bool blending_fade_to_black();

    void set_blending_fade_to_black(bool fade_to_black);

    [[nodiscard]] fixed blending_fade_alpha();

    void set_blending_fade_alpha(fixed fade_alpha);

    void reload_blending_fade();

    void fill_blending_fade_hblank_effect_alphas(const class blending_fade_alpha* blending_fade_alphas_ptr,
                                                 uint16_t* dest_ptr);

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

    [[nodiscard]] pair<int, int> rect_window_hw_horizontal_boundaries(int window);

    [[nodiscard]] pair<int, int> rect_window_hw_vertical_boundaries(int window);

    void set_rect_window_top_left(int window, const fixed_point& top_left);

    void set_rect_window_bottom_right(int window, const fixed_point& bottom_right);

    [[nodiscard]] const optional<camera_ptr>& rect_window_camera(int window);

    void set_rect_window_camera(int window, camera_ptr&& camera);

    void remove_rect_window_camera(int window);

    void reload_rect_windows_boundaries();

    void fill_rect_window_hblank_effect_horizontal_boundaries(
            const pair<int, int>& base_horizontal_boundaries, const pair<fixed, fixed>* horizontal_boundaries_ptr,
            uint16_t* dest_ptr);

    void fill_rect_window_hblank_effect_vertical_boundaries(
            const pair<int, int>& base_vertical_boundaries, const pair<fixed, fixed>* vertical_boundaries_ptr,
            uint16_t* dest_ptr);

    [[nodiscard]] bool inside_window_enabled(int window);

    void set_inside_window_enabled(int window, bool enabled);

    [[nodiscard]] bool green_swap_enabled();

    void set_green_swap_enabled(bool enabled);

    void reload_green_swap();

    void fill_green_swap_hblank_effect_states(const bool* states_ptr, uint16_t* dest_ptr);

    void update_cameras();

    void update();

    void commit();

    void sleep();

    void wake_up();

    void stop();

    void set_show_mode();
}

#endif
