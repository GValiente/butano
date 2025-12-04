/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_display_manager.h"

#include "bn_display.h"
#include "bn_mosaic_attributes.h"
#include "bn_bgs_manager.h"
#include "bn_sprites_manager.h"
#include "../hw/include/bn_hw_display.h"

#include "bn_window.cpp.h"
#include "bn_blending.cpp.h"
#include "bn_bgs_mosaic.cpp.h"
#include "bn_green_swap.cpp.h"
#include "bn_rect_window.cpp.h"
#include "bn_inside_window.cpp.h"
#include "bn_sprites_mosaic.cpp.h"

namespace bn::display_manager
{

namespace
{
    class static_data
    {

    public:
        bool enabled_bgs[hw::bgs::count()] = {};
        fixed sprites_mosaic_horizontal_stretch;
        fixed sprites_mosaic_vertical_stretch;
        fixed bgs_mosaic_horizontal_stretch;
        fixed bgs_mosaic_vertical_stretch;
        fixed blending_transparency_alpha = 1;
        fixed blending_intensity_alpha = 0;
        fixed blending_transparency_top_weight = -1;
        fixed blending_transparency_bottom_weight = -1;
        fixed blending_fade_alpha = 0;
        unsigned windows_flags[hw::display::windows_count()];
        fixed_point rect_windows_boundaries[hw::display::rect_windows_count() * 2];
        point rect_windows_hw_boundaries[hw::display::rect_windows_count() * 2];
        optional<camera_ptr> rect_windows_camera[hw::display::rect_windows_count()] = {};
        uint16_t display_cnt;
        uint16_t mosaic_cnt;
        uint16_t blending_cnt;
        uint16_t blending_transparency_cnt;
        bool inside_windows_enabled[hw::display::inside_windows_count()] = {};
        uint8_t mode = 0;
        bool commit = true;
        bool commit_display = true;
        bool bitmap_page_flipped = false;
        bool sprites_visible = true;
        bool green_swap_enabled = false;
        bool update_mosaic = true;
        hw::display::blending_mode blending_mode;
        bool blending_top_bgs[hw::bgs::count()] = {};
        bool blending_bottom_bgs[hw::bgs::count()] = {};
        bool blending_bottom_sprites = true;
        bool blending_bottom_backdrop = true;
        uint8_t blending_top_layer = 0;
        uint8_t blending_bottom_layer = 0;
        bool blending_fade_enabled = false;
        bool blending_fade_to_black = true;
        bool update_blending_layers = true;
        bool update_blending_mode = true;
        bool update_blending_transparency = true;
        bool update_windows_visible_bgs = false;
        bool commit_windows_flags = true;
        bool commit_windows_boundaries = false;
        bool commit_green_swap = false;
    };

    alignas(static_data) BN_DATA_EWRAM_BSS char data_buffer[sizeof(static_data)];

    [[nodiscard]] static_data& data_ref()
    {
        return *reinterpret_cast<static_data*>(data_buffer);
    }

    [[nodiscard]] pair<int, int> _blending_hw_weights(fixed top_weight, fixed bottom_weight)
    {
        int hw_top_weight = top_weight.data() >> 8;
        int hw_bottom_weight = (1 - bottom_weight).data() >> 8;
        return make_pair(hw_top_weight, 16 - hw_bottom_weight);
    }

    void _update_rect_windows_hw_boundaries(int boundaries_index)
    {
        static_data& data = data_ref();
        fixed_point window_boundaries = data.rect_windows_boundaries[boundaries_index];
        int window_x = window_boundaries.x().shift_integer();
        int window_y = window_boundaries.y().shift_integer();

        const optional<camera_ptr>& camera = data.rect_windows_camera[boundaries_index / 2];

        if(const camera_ptr* camera_ptr = camera.get())
        {
            const fixed_point& camera_position = camera_ptr->position();
            window_x -= camera_position.x().shift_integer();
            window_y -= camera_position.y().shift_integer();
        }

        point& hw_window_boundaries = data.rect_windows_hw_boundaries[boundaries_index];
        hw_window_boundaries.set_x(clamp(window_x + (display::width() / 2), 0, display::width()));
        hw_window_boundaries.set_y(clamp(window_y + (display::height() / 2), 0, display::height()));
        data.commit_windows_boundaries = true;
        data.commit = true;
    }
}

void init()
{
    ::new(static_cast<void*>(data_buffer)) static_data();

    static_data& data = data_ref();
    unsigned initial_window_flags =
            unsigned(hw::display::window_flag::SPRITES) |
            unsigned(hw::display::window_flag::BLENDING);

    for(unsigned& window_flags : data.windows_flags)
    {
        window_flags = initial_window_flags;
    }

    for(bool& inside_windows_enabled : data.inside_windows_enabled)
    {
        inside_windows_enabled = true;
    }

    for(int index = 0, limit = hw::display::rect_windows_count() * 2; index < limit; ++index)
    {
        _update_rect_windows_hw_boundaries(index);
    }
}

void set_mode(int mode)
{
    static_data& data = data_ref();

    if(data.mode != mode)
    {
        data.mode = uint8_t(mode);
        data.commit_display = true;
        data.commit = true;
    }
}

uint16_t* bitmap_page()
{
    return hw::display::hidden_bitmap_page();
}

void flip_bitmap_page()
{
    static_data& data = data_ref();
    data.bitmap_page_flipped = ! data.bitmap_page_flipped;
    data.commit_display = true;
    data.commit = true;
}

void flip_bitmap_page_now()
{
    flip_bitmap_page();
    hw::display::flip_bitmap_page();
}

bool sprites_visible()
{
    return data_ref().sprites_visible;
}

void set_sprites_visible(bool visible)
{
    static_data& data = data_ref();

    if(data.sprites_visible != visible)
    {
        data.sprites_visible = visible;
        data.commit_display = true;
        data.commit = true;
    }
}

bool bg_enabled(int bg)
{
    return data_ref().enabled_bgs[bg];
}

void set_bg_enabled(int bg, bool enabled)
{
    static_data& data = data_ref();

    if(data.enabled_bgs[bg] != enabled)
    {
        data.enabled_bgs[bg] = enabled;
        data.commit_display = true;
        data.commit = true;
    }
}

void disable_all_bgs()
{
    static_data& data = data_ref();

    for(bool& enabled_bg : data.enabled_bgs)
    {
        enabled_bg = false;
    }

    data.commit_display = true;
    data.commit = true;
}

fixed sprites_mosaic_horizontal_stretch()
{
    return data_ref().sprites_mosaic_horizontal_stretch;
}

void set_sprites_mosaic_horizontal_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_stretch = data.sprites_mosaic_horizontal_stretch;
    data.sprites_mosaic_horizontal_stretch = stretch;

    if(fixed_t<4>(old_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

fixed sprites_mosaic_vertical_stretch()
{
    return data_ref().sprites_mosaic_vertical_stretch;
}

void set_sprites_mosaic_vertical_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_stretch = data.sprites_mosaic_vertical_stretch;
    data.sprites_mosaic_vertical_stretch = stretch;

    if(fixed_t<4>(old_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

void set_sprites_mosaic_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_horizontal_stretch = data.sprites_mosaic_horizontal_stretch;
    fixed old_vertical_stretch = data.sprites_mosaic_vertical_stretch;
    data.sprites_mosaic_horizontal_stretch = stretch;
    data.sprites_mosaic_vertical_stretch = stretch;

    if(fixed_t<4>(old_horizontal_stretch) != fixed_t<4>(stretch) ||
            fixed_t<4>(old_vertical_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

void set_sprites_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch)
{
    static_data& data = data_ref();
    fixed old_horizontal_stretch = data.sprites_mosaic_horizontal_stretch;
    fixed old_vertical_stretch = data.sprites_mosaic_vertical_stretch;
    data.sprites_mosaic_horizontal_stretch = horizontal_stretch;
    data.sprites_mosaic_vertical_stretch = vertical_stretch;

    if(fixed_t<4>(old_horizontal_stretch) != fixed_t<4>(horizontal_stretch) ||
            fixed_t<4>(old_vertical_stretch) != fixed_t<4>(vertical_stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

fixed bgs_mosaic_horizontal_stretch()
{
    return data_ref().bgs_mosaic_horizontal_stretch;
}

void set_bgs_mosaic_horizontal_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_stretch = data.bgs_mosaic_horizontal_stretch;
    data.bgs_mosaic_horizontal_stretch = stretch;

    if(fixed_t<4>(old_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

fixed bgs_mosaic_vertical_stretch()
{
    return data_ref().bgs_mosaic_vertical_stretch;
}

void set_bgs_mosaic_vertical_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_stretch = data.bgs_mosaic_vertical_stretch;
    data.bgs_mosaic_vertical_stretch = stretch;

    if(fixed_t<4>(old_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

void set_bgs_mosaic_stretch(fixed stretch)
{
    static_data& data = data_ref();
    fixed old_horizontal_stretch = data.bgs_mosaic_horizontal_stretch;
    fixed old_vertical_stretch = data.bgs_mosaic_vertical_stretch;
    data.bgs_mosaic_horizontal_stretch = stretch;
    data.bgs_mosaic_vertical_stretch = stretch;

    if(fixed_t<4>(old_horizontal_stretch) != fixed_t<4>(stretch) ||
            fixed_t<4>(old_vertical_stretch) != fixed_t<4>(stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

void set_bgs_mosaic_stretch(fixed horizontal_stretch, fixed vertical_stretch)
{
    static_data& data = data_ref();
    fixed old_horizontal_stretch = data.bgs_mosaic_horizontal_stretch;
    fixed old_vertical_stretch = data.bgs_mosaic_vertical_stretch;
    data.bgs_mosaic_horizontal_stretch = horizontal_stretch;
    data.bgs_mosaic_vertical_stretch = vertical_stretch;

    if(fixed_t<4>(old_horizontal_stretch) != fixed_t<4>(horizontal_stretch) ||
            fixed_t<4>(old_vertical_stretch) != fixed_t<4>(vertical_stretch))
    {
        data.update_mosaic = true;
        data.commit = true;
    }
}

void reload_mosaic()
{
    static_data& data = data_ref();
    data.update_mosaic = true;
    data.commit = true;
}

void fill_mosaic_hblank_effect_attributes(const mosaic_attributes* mosaic_attributes_ptr, uint16_t* dest_ptr)
{
    for(int index = 0; index < display::height(); ++index)
    {
        const mosaic_attributes& attributes = mosaic_attributes_ptr[index];
        hw::display::set_mosaic(min(fixed_t<4>(attributes.sprites_horizontal_stretch()).data(), 15),
                                min(fixed_t<4>(attributes.sprites_vertical_stretch()).data(), 15),
                                min(fixed_t<4>(attributes.bgs_horizontal_stretch()).data(), 15),
                                min(fixed_t<4>(attributes.bgs_vertical_stretch()).data(), 15), dest_ptr[index]);
    }
}

bool blending_top_bg_enabled(int bg)
{
    return data_ref().blending_top_bgs[bg];
}

void set_blending_top_bg_enabled(int bg, bool enabled)
{
    static_data& data = data_ref();

    if(data.blending_top_bgs[bg] != enabled)
    {
        data.blending_top_bgs[bg] = enabled;
        data.update_blending_layers = true;
        data.commit = true;
    }
}

bool blending_bottom_bg_enabled(int bg)
{
    return data_ref().blending_bottom_bgs[bg];
}

void set_blending_bottom_bg_enabled(int bg, bool enabled)
{
    static_data& data = data_ref();

    if(data.blending_bottom_bgs[bg] != enabled)
    {
        data.blending_bottom_bgs[bg] = enabled;
        data.update_blending_layers = true;
        data.commit = true;
    }
}

bool blending_bottom_sprites_enabled()
{
    return data_ref().blending_bottom_sprites;
}

void set_blending_bottom_sprites_enabled(bool enabled)
{
    static_data& data = data_ref();

    if(data.blending_bottom_sprites != enabled)
    {
        data.blending_bottom_sprites = enabled;
        data.update_blending_layers = true;
        data.commit = true;
    }
}

bool blending_bottom_backdrop_enabled()
{
    return data_ref().blending_bottom_backdrop;
}

void set_blending_bottom_backdrop_enabled(bool enabled)
{
    static_data& data = data_ref();

    if(data.blending_bottom_backdrop != enabled)
    {
        data.blending_bottom_backdrop = enabled;
        data.update_blending_layers = true;
        data.commit = true;
    }
}

fixed blending_transparency_alpha()
{
    static_data& data = data_ref();
    fixed top_weight = data.blending_transparency_top_weight;

    if(top_weight != -1)
    {
        return top_weight;
    }

    return data.blending_transparency_alpha;
}

void set_blending_transparency_alpha(fixed transparency_alpha)
{
    static_data& data = data_ref();

    if(data.blending_transparency_alpha != transparency_alpha ||
            data.blending_transparency_top_weight != -1 ||
            data.blending_transparency_bottom_weight != -1)
    {
        data.blending_transparency_alpha = transparency_alpha;
        data.blending_transparency_top_weight = -1;
        data.blending_transparency_bottom_weight = -1;
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

fixed blending_intensity_alpha()
{
    static_data& data = data_ref();
    fixed bottom_weight = data.blending_transparency_bottom_weight;

    if(bottom_weight != -1)
    {
        return bottom_weight;
    }

    return data.blending_intensity_alpha;
}

void set_blending_intensity_alpha(fixed intensity_alpha)
{
    static_data& data = data_ref();

    if(data.blending_intensity_alpha != intensity_alpha ||
            data.blending_transparency_top_weight != -1 ||
            data.blending_transparency_bottom_weight != -1)
    {
        data.blending_intensity_alpha = intensity_alpha;
        data.blending_transparency_top_weight = -1;
        data.blending_transparency_bottom_weight = -1;
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

void set_blending_transparency_and_intensity_alpha(fixed transparency_alpha, fixed intensity_alpha)
{
    static_data& data = data_ref();

    if(data.blending_transparency_alpha != transparency_alpha ||
            data.blending_intensity_alpha != intensity_alpha ||
            data.blending_transparency_top_weight != -1 ||
            data.blending_transparency_bottom_weight != -1)
    {
        data.blending_transparency_alpha = transparency_alpha;
        data.blending_intensity_alpha = intensity_alpha;
        data.blending_transparency_top_weight = -1;
        data.blending_transparency_bottom_weight = -1;
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

fixed blending_transparency_top_weight()
{
    static_data& data = data_ref();
    fixed result = data.blending_transparency_top_weight;

    if(result == -1)
    {
        result = data.blending_transparency_alpha;
    }

    return result;
}

void set_blending_transparency_top_weight(fixed top_weight)
{
    static_data& data = data_ref();
    fixed old_top_weight = data.blending_transparency_top_weight;
    data.blending_transparency_top_weight = top_weight;

    if(old_top_weight.data() >> 8 != top_weight.data() >> 8)
    {
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

fixed blending_transparency_bottom_weight()
{
    static_data& data = data_ref();
    fixed result = data.blending_transparency_bottom_weight;

    if(result == -1)
    {
        result = max(1 - data.blending_transparency_alpha, data.blending_intensity_alpha);
    }

    return result;
}

void set_blending_transparency_bottom_weight(fixed bottom_weight)
{
    static_data& data = data_ref();
    fixed old_bottom_weight = data.blending_transparency_bottom_weight;
    data.blending_transparency_bottom_weight = bottom_weight;

    if(old_bottom_weight.data() >> 8 != bottom_weight.data() >> 8)
    {
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

void set_blending_transparency_weights(fixed top_weight, fixed bottom_weight)
{
    static_data& data = data_ref();
    fixed old_top_weight = data.blending_transparency_top_weight;
    fixed old_bottom_weight = data.blending_transparency_bottom_weight;
    data.blending_transparency_top_weight = top_weight;
    data.blending_transparency_bottom_weight = bottom_weight;

    if(old_top_weight.data() >> 8 != top_weight.data() >> 8 ||
            old_bottom_weight.data() >> 8 != bottom_weight.data() >> 8)
    {
        data.update_blending_transparency = true;
        data.commit = true;
    }
}

void reload_blending_transparency()
{
    static_data& data = data_ref();
    data.update_blending_transparency = true;
    data.commit = true;
}

void fill_blending_transparency_hblank_effect_attributes(
        const blending_transparency_attributes* blending_transparency_attributes_ptr, uint16_t* dest_ptr)
{
    for(int index = 0; index < display::height(); ++index)
    {
        const blending_transparency_attributes& attributes = blending_transparency_attributes_ptr[index];
        pair<int, int> hw_weights = _blending_hw_weights(
                    attributes.transparency_top_weight(), attributes.transparency_bottom_weight());
        hw::display::set_blending_transparency(hw_weights.first, hw_weights.second, dest_ptr[index]);
    }
}

bool blending_fade_enabled()
{
    return data_ref().blending_fade_enabled;
}

void set_blending_fade_enabled(bool enabled)
{
    static_data& data = data_ref();

    if(data.blending_fade_enabled != enabled)
    {
        data.blending_fade_enabled = enabled;
        data.update_blending_mode = true;
        data.update_blending_layers = true;
        data.commit = true;
        sprites_manager::reload_blending();
    }
}

bool blending_fade_to_black()
{
    return data_ref().blending_fade_to_black;
}

void set_blending_fade_to_black(bool fade_to_black)
{
    static_data& data = data_ref();

    if(data.blending_fade_to_black != fade_to_black)
    {
        data.blending_fade_to_black = fade_to_black;

        if(data.blending_fade_enabled)
        {
            data.update_blending_mode = true;
            data.commit = true;
        }
    }
}

fixed blending_fade_alpha()
{
    return data_ref().blending_fade_alpha;
}

void set_blending_fade_alpha(fixed fade_alpha)
{
    static_data& data = data_ref();
    int old_fade_alpha = fixed_t<4>(data.blending_fade_alpha).data();
    data.blending_fade_alpha = fade_alpha;

    if(old_fade_alpha != fixed_t<4>(fade_alpha).data())
    {
        reload_blending_fade();
    }
}

void reload_blending_fade()
{
    static_data& data = data_ref();
    set_blending_fade_enabled(fixed_t<4>(data.blending_fade_alpha).data());
    data.commit = true;
}

void fill_blending_fade_hblank_effect_alphas(const class blending_fade_alpha* blending_fade_alphas_ptr,
                                             uint16_t* dest_ptr)
{
    for(int index = 0; index < display::height(); ++index)
    {
        const class blending_fade_alpha& alpha = blending_fade_alphas_ptr[index];
        hw::display::set_blending_fade(fixed_t<4>(alpha.value()).data(), dest_ptr[index]);
    }
}

void update_windows_visible_bgs()
{
    static_data& data = data_ref();
    data.update_windows_visible_bgs = true;
    data.commit = true;
}

bool show_sprites_in_window(int window)
{
    return data_ref().windows_flags[window] & unsigned(hw::display::window_flag::SPRITES);
}

void set_show_sprites_in_window(int window, bool show)
{
    static_data& data = data_ref();

    if(show)
    {
        data.windows_flags[window] |= unsigned(hw::display::window_flag::SPRITES);
    }
    else
    {
        data.windows_flags[window] &= ~unsigned(hw::display::window_flag::SPRITES);
    }

    data.commit_windows_flags = true;
    data.commit = true;
}

bool show_blending_in_window(int window)
{
    return data_ref().windows_flags[window] & unsigned(hw::display::window_flag::BLENDING);
}

void set_show_blending_in_window(int window, bool show)
{
    static_data& data = data_ref();

    if(show)
    {
        data.windows_flags[window] |= unsigned(hw::display::window_flag::BLENDING);
    }
    else
    {
        data.windows_flags[window] &= ~unsigned(hw::display::window_flag::BLENDING);
    }

    data.commit_windows_flags = true;
    data.commit = true;
}

bool show_all_in_window(int window)
{
    return data_ref().windows_flags[window] & unsigned(hw::display::window_flag::ALL);
}

void set_show_all_in_window(int window)
{
    static_data& data = data_ref();
    data.windows_flags[window] |= unsigned(hw::display::window_flag::ALL);
    data.commit_windows_flags = true;
    data.commit = true;
}

bool show_nothing_in_window(int window)
{
    unsigned window_flags = data_ref().windows_flags[window];
    return window_flags == 0 || window_flags == unsigned(hw::display::window_flag::BLENDING);
}

void set_show_nothing_in_window(int window)
{
    static_data& data = data_ref();
    data.windows_flags[window] &= ~unsigned(hw::display::window_flag::ALL);
    data.commit_windows_flags = true;
    data.commit = true;
}

const fixed_point& rect_window_top_left(int window)
{
    return data_ref().rect_windows_boundaries[window * 2];
}

const fixed_point& rect_window_bottom_right(int window)
{
    return data_ref().rect_windows_boundaries[(window * 2) + 1];
}

pair<int, int> rect_window_hw_horizontal_boundaries(int window)
{
    const point* hw_boundaries = data_ref().rect_windows_hw_boundaries;
    int index = window * 2;
    return make_pair(hw_boundaries[index].x(), hw_boundaries[index + 1].x());
}

pair<int, int> rect_window_hw_vertical_boundaries(int window)
{
    const point* hw_boundaries = data_ref().rect_windows_hw_boundaries;
    int index = window * 2;
    return make_pair(hw_boundaries[index].y(), hw_boundaries[index + 1].y());
}

void set_rect_window_top_left(int window, const fixed_point& top_left)
{
    int index = window * 2;
    fixed_point& old_top_left = data_ref().rect_windows_boundaries[index];
    point old_integer_top_left(old_top_left.x().shift_integer(), old_top_left.y().shift_integer());
    point new_integer_top_left(top_left.x().shift_integer(), top_left.y().shift_integer());
    old_top_left = top_left;

    if(old_integer_top_left != new_integer_top_left)
    {
        _update_rect_windows_hw_boundaries(index);
    }
}

void set_rect_window_bottom_right(int window, const fixed_point& bottom_right)
{
    int index = (window * 2) + 1;
    fixed_point& old_bottom_right = data_ref().rect_windows_boundaries[index];
    point old_integer_bottom_right(old_bottom_right.x().shift_integer(), old_bottom_right.y().shift_integer());
    point new_integer_bottom_right(bottom_right.x().shift_integer(), bottom_right.y().shift_integer());
    old_bottom_right = bottom_right;

    if(old_integer_bottom_right != new_integer_bottom_right)
    {
        _update_rect_windows_hw_boundaries(index);
    }
}

const optional<camera_ptr>& rect_window_camera(int window)
{
    return data_ref().rect_windows_camera[window];
}

void set_rect_window_camera(int window, camera_ptr&& camera)
{
    static_data& data = data_ref();

    if(data.rect_windows_camera[window] != camera)
    {
        data.rect_windows_camera[window] = move(camera);

        int index = window * 2;
        _update_rect_windows_hw_boundaries(index);
        _update_rect_windows_hw_boundaries(index + 1);
    }
}

void remove_rect_window_camera(int window)
{
    static_data& data = data_ref();

    if(data.rect_windows_camera[window])
    {
        data.rect_windows_camera[window].reset();

        int index = window * 2;
        _update_rect_windows_hw_boundaries(index);
        _update_rect_windows_hw_boundaries(index + 1);
    }
}

void reload_rect_windows_boundaries()
{
    static_data& data = data_ref();
    data.commit_windows_boundaries = true;
    data.commit = true;
}

void fill_rect_window_hblank_effect_horizontal_boundaries(
        const pair<int, int>& base_horizontal_boundaries, const pair<fixed, fixed>* horizontal_boundaries_ptr,
        uint16_t* dest_ptr)
{
    fixed base_first = base_horizontal_boundaries.first;
    fixed base_second = base_horizontal_boundaries.second;

    if(base_first == 0 && base_second == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = horizontal_boundaries_ptr[index].first;
            fixed second_fixed = horizontal_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::width());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::width());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else if(base_first == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = horizontal_boundaries_ptr[index].first;
            fixed second_fixed = base_second + horizontal_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::width());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::width());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else if(base_second == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = base_first + horizontal_boundaries_ptr[index].first;
            fixed second_fixed = horizontal_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::width());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::width());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = base_first + horizontal_boundaries_ptr[index].first;
            fixed second_fixed = base_second + horizontal_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::width());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::width());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
}

void fill_rect_window_hblank_effect_vertical_boundaries(
        const pair<int, int>& base_vertical_boundaries, const pair<fixed, fixed>* vertical_boundaries_ptr,
        uint16_t* dest_ptr)
{
    fixed base_first = base_vertical_boundaries.first;
    fixed base_second = base_vertical_boundaries.second;

    if(base_first == 0 && base_second == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = vertical_boundaries_ptr[index].first;
            fixed second_fixed = vertical_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::height());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::height());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else if(base_first == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = vertical_boundaries_ptr[index].first;
            fixed second_fixed = base_second + vertical_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::height());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::height());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else if(base_second == 0)
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = base_first + vertical_boundaries_ptr[index].first;
            fixed second_fixed = vertical_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::height());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::height());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
    else
    {
        for(int index = 0; index < display::height(); ++index)
        {
            fixed first_fixed = base_first + vertical_boundaries_ptr[index].first;
            fixed second_fixed = base_second + vertical_boundaries_ptr[index].second;
            int first_integer = clamp(first_fixed.shift_integer(), 0, display::height());
            int second_integer = clamp(second_fixed.shift_integer(), 0, display::height());
            hw::display::set_window_boundaries(first_integer, second_integer, dest_ptr[index]);
        }
    }
}

bool inside_window_enabled(int window)
{
    return data_ref().inside_windows_enabled[window];
}

void set_inside_window_enabled(int window, bool enabled)
{
    static_data& data = data_ref();

    if(data.inside_windows_enabled[window] != enabled)
    {
        data.inside_windows_enabled[window] = enabled;
        data.commit_display = true;
        data.commit = true;
    }
}

bool green_swap_enabled()
{
    return data_ref().green_swap_enabled;
}

void set_green_swap_enabled(bool enabled)
{
    static_data& data = data_ref();

    if(data.green_swap_enabled != enabled)
    {
        data.green_swap_enabled = enabled;
        data.commit_green_swap = true;
        data.commit = true;
    }
}

void reload_green_swap()
{
    static_data& data = data_ref();
    data.commit_green_swap = true;
    data.commit = true;
}

void fill_green_swap_hblank_effect_states(const bool* states_ptr, uint16_t* dest_ptr)
{
    for(int index = 0; index < display::height(); ++index)
    {
        hw::display::set_green_swap_enabled(states_ptr[index], dest_ptr[index]);
    }
}

void update_cameras()
{
    static_data& data = data_ref();

    for(int index = 0, limit = hw::display::rect_windows_count(); index < limit; ++index)
    {
        if(data.rect_windows_camera[index])
        {
            int boundaries_index = index * 2;
            _update_rect_windows_hw_boundaries(boundaries_index);
            _update_rect_windows_hw_boundaries(boundaries_index + 1);
        }
    }
}

void update()
{
    static_data& data = data_ref();

    if(data.commit)
    {
        bool update_blending_cnt = false;

        if(data.update_blending_mode)
        {
            if(data.blending_fade_enabled)
            {
                if(data.blending_fade_to_black)
                {
                    data.blending_mode = hw::display::blending_mode::FADE_TO_BLACK;
                }
                else
                {
                    data.blending_mode = hw::display::blending_mode::FADE_TO_WHITE;
                }
            }
            else
            {
                data.blending_mode = hw::display::blending_mode::TRANSPARENCY;
            }

            data.update_blending_mode = false;
            update_blending_cnt = true;
        }

        if(data.update_blending_layers)
        {
            bool fade = data.blending_mode != hw::display::blending_mode::TRANSPARENCY;
            data.blending_top_layer = hw::display::blending_layer(data.blending_top_bgs, fade, fade);
            data.blending_bottom_layer = hw::display::blending_layer(
                    data.blending_bottom_bgs, data.blending_bottom_sprites, data.blending_bottom_backdrop);
            data.update_blending_layers = false;
            update_blending_cnt = true;
        }

        if(update_blending_cnt)
        {
            hw::display::set_blending_cnt(
                    data.blending_top_layer, data.blending_bottom_layer, data.blending_mode, data.blending_cnt);
        }

        if(data.update_windows_visible_bgs)
        {
            data.update_windows_visible_bgs = false;
            data.commit_windows_flags = true;
            bgs_manager::update_windows_flags(data.windows_flags);
        }

        if(data.commit_display)
        {
            hw::display::set_display(data.mode, data.bitmap_page_flipped, data.sprites_visible, data.enabled_bgs,
                                     data.inside_windows_enabled, data.display_cnt);
        }

        if(data.update_mosaic)
        {
            data.update_mosaic = false;
            hw::display::set_mosaic(min(fixed_t<4>(data.sprites_mosaic_horizontal_stretch).data(), 15),
                                    min(fixed_t<4>(data.sprites_mosaic_vertical_stretch).data(), 15),
                                    min(fixed_t<4>(data.bgs_mosaic_horizontal_stretch).data(), 15),
                                    min(fixed_t<4>(data.bgs_mosaic_vertical_stretch).data(), 15), data.mosaic_cnt);
        }

        if(data.update_blending_transparency)
        {
            data.update_blending_transparency = false;

            pair<int, int> hw_weights = _blending_hw_weights(
                        blending_transparency_top_weight(), blending_transparency_bottom_weight());
            hw::display::set_blending_transparency(
                        hw_weights.first, hw_weights.second, data.blending_transparency_cnt);
        }
    }
}

void commit()
{
    static_data& data = data_ref();

    if(data.commit)
    {
        data.commit = false;

        if(data.commit_display)
        {
            hw::display::commit_display(data.display_cnt);
            data.commit_display = false;
        }

        hw::display::commit_mosaic(data.mosaic_cnt);
        hw::display::commit_blending_cnt(data.blending_cnt);
        hw::display::commit_blending_transparency(data.blending_transparency_cnt);
        hw::display::set_blending_fade(fixed_t<4>(data.blending_fade_alpha).data());

        if(data.commit_windows_flags)
        {
            hw::display::set_windows_flags(data.windows_flags);
            data.commit_windows_flags = false;
        }

        if(data.commit_windows_boundaries)
        {
            hw::display::set_windows_boundaries(data.rect_windows_hw_boundaries);
            data.commit_windows_boundaries = false;
        }

        if(data.commit_green_swap)
        {
            hw::display::set_green_swap_enabled(data.green_swap_enabled);
            data.commit_green_swap = false;
        }
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

void stop()
{
    static_data& data = data_ref();
    data.update_blending_mode = false;
    data.update_blending_layers = false;
    data.update_windows_visible_bgs = false;
    data.commit = false;
    hw::display::stop();
}

void set_show_mode()
{
    hw::display::set_show_mode();
}

}
