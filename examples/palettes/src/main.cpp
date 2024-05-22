/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_display.h"
#include "bn_color_effect.h"
#include "bn_regular_bg_ptr.h"
#include "bn_bg_palette_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_palette_actions.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_bg_palette_color_hbe_ptr.h"
#include "bn_bg_palettes_transparent_color_hbe_ptr.h"

#include "bn_sprite_items_cavegirl.h"
#include "bn_regular_bg_items_village.h"
#include "bn_sprite_palette_items_cavegirl_alt.h"
#include "bn_sprite_items_cavegirl_green.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void palette_swap_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: swap palette",
            "",
            "START: go to next scene",
        };

        common::info info("Palette swap", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        const bn::sprite_palette_item& palette_item = bn::sprite_items::cavegirl.palette_item();
        const bn::sprite_palette_item& alt_palette_item = bn::sprite_palette_items::cavegirl_alt;
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_colors(alt_palette_item);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                if(cavegirl_palette.colors() == palette_item.colors_ref())
                {
                    cavegirl_palette.set_colors(alt_palette_item);
                }
                else
                {
                    cavegirl_palette.set_colors(palette_item);
                }
            }

            info.update();
            bn::core::update();
        }
    }

    void inverted_palette_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: invert palette",
            "",
            "START: go to next scene",
        };

        common::info info("Inverted palette", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_inverted(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                cavegirl_palette.set_inverted(! cavegirl_palette.inverted());
            }

            info.update();
            bn::core::update();
        }
    }

    void inverted_palette_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Inverted palette actions", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palette_ptr village_palette = village_bg.palette();
        bn::bg_palette_inverted_toggle_action invert_action(village_palette, 60);

        while(! bn::keypad::start_pressed())
        {
            invert_action.update();
            info.update();
            bn::core::update();
        }
    }

    void palette_grayscale_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease grayscale intensity",
            "RIGHT: increase grayscale intensity",
            "",
            "START: go to next scene",
        };

        common::info info("Palette grayscale", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_grayscale_intensity(1);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed grayscale_intensity = cavegirl_palette.grayscale_intensity();

            if(bn::keypad::left_held())
            {
                cavegirl_palette.set_grayscale_intensity(bn::max(grayscale_intensity - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                cavegirl_palette.set_grayscale_intensity(bn::min(grayscale_intensity + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }
    }

    void palette_grayscale_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Palette grayscale actions", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palette_ptr village_palette = village_bg.palette();
        bn::bg_palette_grayscale_loop_action grayscale_action(village_palette, 120, 1);

        while(! bn::keypad::start_pressed())
        {
            grayscale_action.update();
            info.update();
            bn::core::update();
        }
    }

    void palette_fade_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease fade intensity",
            "RIGHT: increase fade intensity",
            "",
            "START: go to next scene",
        };

        common::info info("Palette fade", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palette_ptr village_palette = village_bg.palette();
        village_palette.set_fade(bn::colors::red, 0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed fade_intensity = village_palette.fade_intensity();

            if(bn::keypad::left_held())
            {
                village_palette.set_fade_intensity(bn::max(fade_intensity - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                village_palette.set_fade_intensity(bn::min(fade_intensity + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }
    }

    void palette_fade_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Palette fade actions", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_fade_color(bn::colors::red);

        bn::sprite_palette_fade_loop_action fade_action(cavegirl_palette, 120, 1);

        while(! bn::keypad::start_pressed())
        {
            fade_action.update();
            info.update();
            bn::core::update();
        }
    }

    void palette_hue_shift_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease hue shift intensity",
            "RIGHT: increase hue shift intensity",
            "",
            "START: go to next scene",
        };

        common::info info("Palette hue shift", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_hue_shift_intensity(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed hue_shift_intensity = cavegirl_palette.hue_shift_intensity();

            if(bn::keypad::left_held())
            {
                cavegirl_palette.set_hue_shift_intensity(bn::max(hue_shift_intensity - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                cavegirl_palette.set_hue_shift_intensity(bn::min(hue_shift_intensity + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }
    }

    void palette_hue_shift_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Palette hue shift actions", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palette_ptr village_palette = village_bg.palette();
        bn::bg_palette_hue_shift_loop_action hue_shift_action(village_palette, 120, 1);

        while(! bn::keypad::start_pressed())
        {
            hue_shift_action.update();
            info.update();
            bn::core::update();
        }
    }

    void palette_rotate_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease colors shift",
            "RIGHT: increase colors shift",
            "",
            "START: go to next scene",
        };

        common::info info("Palette rotate", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl_green.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        int rotate_range_size = cavegirl_palette.rotate_range_size();

        while(! bn::keypad::start_pressed())
        {
            int rotate_count = cavegirl_palette.rotate_count();

            if(bn::keypad::left_pressed())
            {
                cavegirl_palette.set_rotate_count(bn::max(rotate_count - 1, 1 - rotate_range_size));
            }
            else if(bn::keypad::right_pressed())
            {
                cavegirl_palette.set_rotate_count(bn::min(rotate_count + 1, rotate_range_size - 1));
            }

            info.update();
            bn::core::update();
        }
    }

    void palette_rotate_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Palette rotate actions", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl_green.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        bn::sprite_palette_rotate_by_action rotate_action(cavegirl_palette, 4, 1);

        while(! bn::keypad::start_pressed())
        {
            rotate_action.update();
            info.update();
            bn::core::update();
        }
    }

    void global_brightness_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease brightness",
            "RIGHT: increase brightness",
            "",
            "START: go to next scene",
        };

        common::info info("Global brightness", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palettes::set_brightness(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed brightness = bn::bg_palettes::brightness();

            if(bn::keypad::left_held())
            {
                bn::bg_palettes::set_brightness(bn::max(brightness - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::bg_palettes::set_brightness(bn::min(brightness + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::bg_palettes::set_brightness(0);
    }

    void global_brightness_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Global brightness actions", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palettes_brightness_loop_action brightness_action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            brightness_action.update();
            info.update();
            bn::core::update();
        }

        bn::sprite_palettes::set_brightness(0);
    }

    void global_contrast_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease contrast",
            "RIGHT: increase contrast",
            "",
            "START: go to next scene",
        };

        common::info info("Global contrast", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palettes::set_contrast(1);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed contrast = bn::sprite_palettes::contrast();

            if(bn::keypad::left_held())
            {
                bn::sprite_palettes::set_contrast(bn::max(contrast - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::sprite_palettes::set_contrast(bn::min(contrast + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::sprite_palettes::set_contrast(0);
    }

    void global_contrast_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Global contrast actions", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palettes_contrast_loop_action contrast_action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            contrast_action.update();
            info.update();
            bn::core::update();
        }

        bn::bg_palettes::set_contrast(0);
    }

    void global_intensity_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease intensity",
            "RIGHT: increase intensity",
            "",
            "START: go to next scene",
        };

        common::info info("Global intensity", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        bn::bg_palettes::set_intensity(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed intensity = bn::bg_palettes::intensity();

            if(bn::keypad::left_held())
            {
                bn::bg_palettes::set_intensity(bn::max(intensity - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::bg_palettes::set_intensity(bn::min(intensity + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::bg_palettes::set_intensity(0);
    }

    void global_intensity_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Global intensity actions", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palettes_intensity_loop_action intensity_action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            intensity_action.update();
            info.update();
            bn::core::update();
        }

        bn::sprite_palettes::set_intensity(0);
    }

    void transparent_color_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Transparent color H-Blank effect", info_text_lines, text_generator);

        alignas(int) bn::color colors[bn::display::height()];
        bn::fixed max_red_inc(16);

        for(int index = 0, amplitude = bn::display::height() / 2; index < amplitude; ++index)
        {
            bn::fixed red_inc = max_red_inc - ((index * max_red_inc) / amplitude);
            bn::color color(bn::min(16 + red_inc.right_shift_integer(), 31), 0, 16);
            colors[(bn::display::height() / 2) + index] = color;
            colors[(bn::display::height() / 2) - index - 1] = color;
        }

        bn::bg_palettes_transparent_color_hbe_ptr colors_hbe =
                bn::bg_palettes_transparent_color_hbe_ptr::create(colors);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void palette_color_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Palette color H-Blank effect", info_text_lines, text_generator);

        bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
        alignas(int) bn::color colors[bn::display::height()];
        bn::fixed max_green_dec(16);

        for(int index = 0, amplitude = bn::display::height() / 2; index < amplitude; ++index)
        {
            bn::fixed green_dec = (index * max_green_dec) / amplitude;
            bn::color color(31, 21 - green_dec.right_shift_integer(), 11);
            colors[(bn::display::height() / 2) + index] = color;
            colors[(bn::display::height() / 2) - index - 1] = color;
        }

        bn::bg_palette_color_hbe_ptr colors_hbe =
                bn::bg_palette_color_hbe_ptr::create(village_bg.palette(), 1, colors);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void palette_blend_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease blend weight",
            "RIGHT: increase blend weight",
            "",
            "START: go to next scene",
        };

        common::info info("Palette blend", info_text_lines, text_generator);

        bn::sprite_ptr cavegirl_sprite = bn::sprite_items::cavegirl.create_sprite(0, 0);
        bn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();

        const bn::sprite_palette_item& first_source_palette_item = bn::sprite_items::cavegirl.palette_item();
        const bn::sprite_palette_item& second_source_palette_item = bn::sprite_palette_items::cavegirl_alt;

        alignas(int) bn::array<bn::color, 16> dest_palette_colors;
        bn::sprite_palette_item dest_palette_item(dest_palette_colors, first_source_palette_item.bpp());

        bn::fixed blend_weight = 0.5;

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                blend_weight = bn::max(blend_weight - 0.01, bn::fixed(0));
            }
            else if(bn::keypad::right_held())
            {
                blend_weight = bn::min(blend_weight + 0.01, bn::fixed(1));
            }

            bn::color_effect::blend(first_source_palette_item.colors_ref(), second_source_palette_item.colors_ref(),
                                    blend_weight, dest_palette_colors);
            cavegirl_palette.set_colors(dest_palette_item);

            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        palette_swap_scene(text_generator);
        bn::core::update();

        inverted_palette_scene(text_generator);
        bn::core::update();

        inverted_palette_actions_scene(text_generator);
        bn::core::update();

        palette_grayscale_scene(text_generator);
        bn::core::update();

        palette_grayscale_actions_scene(text_generator);
        bn::core::update();

        palette_fade_scene(text_generator);
        bn::core::update();

        palette_fade_actions_scene(text_generator);
        bn::core::update();

        palette_hue_shift_scene(text_generator);
        bn::core::update();

        palette_hue_shift_actions_scene(text_generator);
        bn::core::update();

        palette_rotate_scene(text_generator);
        bn::core::update();

        palette_rotate_actions_scene(text_generator);
        bn::core::update();

        global_brightness_scene(text_generator);
        bn::core::update();

        global_brightness_actions_scene(text_generator);
        bn::core::update();

        global_contrast_scene(text_generator);
        bn::core::update();

        global_contrast_actions_scene(text_generator);
        bn::core::update();

        global_intensity_scene(text_generator);
        bn::core::update();

        global_intensity_actions_scene(text_generator);
        bn::core::update();

        transparent_color_hbe_scene(text_generator);
        bn::core::update();

        palette_color_hbe_scene(text_generator);
        bn::core::update();

        palette_blend_scene(text_generator);
        bn::core::update();
    }
}
