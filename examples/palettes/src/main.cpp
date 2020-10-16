#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_colors.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_regular_bg_ptr.h"
#include "btn_bg_palette_actions.h"
#include "btn_bg_palettes_actions.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_palette_actions.h"
#include "btn_sprite_palettes_actions.h"
#include "btn_bg_palette_color_hblank_effect_ptr.h"
#include "btn_bg_palettes_transparent_color_hblank_effect_ptr.h"

#include "btn_sprite_items_cavegirl.h"
#include "btn_regular_bg_items_village.h"
#include "btn_sprite_items_cavegirl_alt.h"
#include "btn_sprite_items_cavegirl_green.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void palette_swap_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: swap palette",
            "",
            "START: go to next scene",
        };

        info info("Palette swap", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::span<const btn::color> cavegirl_colors = btn::sprite_items::cavegirl.palette_item().colors();
        btn::span<const btn::color> cavegirl_alt_colors = btn::sprite_items::cavegirl_alt.palette_item().colors();
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_colors(cavegirl_alt_colors);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                if(cavegirl_palette.colors() == cavegirl_colors)
                {
                    cavegirl_palette.set_colors(cavegirl_alt_colors);
                }
                else
                {
                    cavegirl_palette.set_colors(cavegirl_colors);
                }
            }

            info.update();
            btn::core::update();
        }
    }

    void inverted_palette_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: invert palette",
            "",
            "START: go to next scene",
        };

        info info("Inverted palette", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_inverted(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                cavegirl_palette.set_inverted(! cavegirl_palette.inverted());
            }

            info.update();
            btn::core::update();
        }
    }

    void inverted_palette_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Inverted palette actions", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palette_ptr village_palette = village_bg.palette();
        btn::bg_palette_inverted_toggle_action invert_action(village_palette, 60);

        while(! btn::keypad::start_pressed())
        {
            invert_action.update();
            info.update();
            btn::core::update();
        }
    }

    void palette_grayscale_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease grayscale intensity",
            "RIGHT: increase grayscale intensity",
            "",
            "START: go to next scene",
        };

        info info("Palette grayscale", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_grayscale_intensity(1);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed grayscale_intensity = cavegirl_palette.grayscale_intensity();

            if(btn::keypad::left_held())
            {
                cavegirl_palette.set_grayscale_intensity(btn::max(grayscale_intensity - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                cavegirl_palette.set_grayscale_intensity(btn::min(grayscale_intensity + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }
    }

    void palette_grayscale_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Palette grayscale actions", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palette_ptr village_palette = village_bg.palette();
        btn::bg_palette_grayscale_loop_action grayscale_action(village_palette, 120, 1);

        while(! btn::keypad::start_pressed())
        {
            grayscale_action.update();
            info.update();
            btn::core::update();
        }
    }

    void palette_fade_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease fade intensity",
            "RIGHT: increase fade intensity",
            "",
            "START: go to next scene",
        };

        info info("Palette fade", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palette_ptr village_palette = village_bg.palette();
        village_palette.set_fade(btn::colors::red, 0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed fade_intensity = village_palette.fade_intensity();

            if(btn::keypad::left_held())
            {
                village_palette.set_fade_intensity(btn::max(fade_intensity - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                village_palette.set_fade_intensity(btn::min(fade_intensity + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }
    }

    void palette_fade_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Palette fade actions", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        cavegirl_palette.set_fade_color(btn::colors::red);

        btn::sprite_palette_fade_loop_action fade_action(cavegirl_palette, 120, 1);

        while(! btn::keypad::start_pressed())
        {
            fade_action.update();
            info.update();
            btn::core::update();
        }
    }

    void palette_rotate_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease colors shift",
            "RIGHT: increase colors shift",
            "",
            "START: go to next scene",
        };

        info info("Palette rotate", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl_green.create_sprite(0, 0);
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        int palette_colors_count = cavegirl_palette.colors_count();

        while(! btn::keypad::start_pressed())
        {
            int rotate_count = cavegirl_palette.rotate_count();

            if(btn::keypad::left_pressed())
            {
                cavegirl_palette.set_rotate_count(btn::max(rotate_count - 1, 2 - palette_colors_count));
            }
            else if(btn::keypad::right_pressed())
            {
                cavegirl_palette.set_rotate_count(btn::min(rotate_count + 1, palette_colors_count - 2));
            }

            info.update();
            btn::core::update();
        }
    }

    void palette_rotate_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Palette rotate actions", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl_green.create_sprite(0, 0);
        btn::sprite_palette_ptr cavegirl_palette = cavegirl_sprite.palette();
        btn::sprite_palette_rotate_by_action rotate_action(cavegirl_palette, 4, 1);

        while(! btn::keypad::start_pressed())
        {
            rotate_action.update();
            info.update();
            btn::core::update();
        }
    }

    void global_brightness_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease brightness",
            "RIGHT: increase brightness",
            "",
            "START: go to next scene",
        };

        info info("Global brightness", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palettes::set_brightness(0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed brightness = btn::bg_palettes::brightness();

            if(btn::keypad::left_held())
            {
                btn::bg_palettes::set_brightness(btn::max(brightness - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::bg_palettes::set_brightness(btn::min(brightness + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::bg_palettes::set_brightness(0);
    }

    void global_brightness_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Global brightness actions", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palettes_brightness_loop_action brightness_action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            brightness_action.update();
            info.update();
            btn::core::update();
        }

        btn::sprite_palettes::set_brightness(0);
    }

    void global_contrast_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease contrast",
            "RIGHT: increase contrast",
            "",
            "START: go to next scene",
        };

        info info("Global contrast", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palettes::set_contrast(1);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed contrast = btn::sprite_palettes::contrast();

            if(btn::keypad::left_held())
            {
                btn::sprite_palettes::set_contrast(btn::max(contrast - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::sprite_palettes::set_contrast(btn::min(contrast + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::sprite_palettes::set_contrast(0);
    }

    void global_contrast_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Global contrast actions", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palettes_contrast_loop_action contrast_action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            contrast_action.update();
            info.update();
            btn::core::update();
        }

        btn::bg_palettes::set_contrast(0);
    }

    void global_intensity_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease intensity",
            "RIGHT: increase intensity",
            "",
            "START: go to next scene",
        };

        info info("Global intensity", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::bg_palettes::set_intensity(0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed intensity = btn::bg_palettes::intensity();

            if(btn::keypad::left_held())
            {
                btn::bg_palettes::set_intensity(btn::max(intensity - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::bg_palettes::set_intensity(btn::min(intensity + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::bg_palettes::set_intensity(0);
    }

    void global_intensity_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Global intensity actions", info_text_lines, text_generator);

        btn::sprite_ptr cavegirl_sprite = btn::sprite_items::cavegirl.create_sprite(0, 0);
        btn::sprite_palettes_intensity_loop_action intensity_action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            intensity_action.update();
            info.update();
            btn::core::update();
        }

        btn::sprite_palettes::set_intensity(0);
    }

    void transparent_color_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Transparent color H-Blank effect", info_text_lines, text_generator);

        btn::array<btn::color, btn::display::height()> colors;
        btn::fixed max_red_inc(16);

        for(int index = 0, amplitude = btn::display::height() / 2; index < amplitude; ++index)
        {
            btn::fixed red_inc = max_red_inc - ((index * max_red_inc) / amplitude);
            btn::color color(btn::min(16 + red_inc.right_shift_integer(), 31), 0, 16);
            colors[(btn::display::height() / 2) + index] = color;
            colors[(btn::display::height() / 2) - index - 1] = color;
        }

        btn::bg_palettes_transparent_color_hblank_effect_ptr hblank_effect =
                btn::bg_palettes_transparent_color_hblank_effect_ptr::create(colors);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void palette_color_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Palette color H-Blank effect", info_text_lines, text_generator);

        btn::regular_bg_ptr village_bg = btn::regular_bg_items::village.create_bg(0, 0);
        btn::array<btn::color, btn::display::height()> colors;
        btn::fixed max_green_dec(16);

        for(int index = 0, amplitude = btn::display::height() / 2; index < amplitude; ++index)
        {
            btn::fixed green_dec = (index * max_green_dec) / amplitude;
            btn::color color(31, 21 - green_dec.right_shift_integer(), 11);
            colors[(btn::display::height() / 2) + index] = color;
            colors[(btn::display::height() / 2) - index - 1] = color;
        }

        btn::bg_palette_color_hblank_effect_ptr hblank_effect =
                btn::bg_palette_color_hblank_effect_ptr::create(village_bg.palette(), 1, colors);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    while(true)
    {
        palette_swap_scene(text_generator);
        btn::core::update();

        inverted_palette_scene(text_generator);
        btn::core::update();

        inverted_palette_actions_scene(text_generator);
        btn::core::update();

        palette_grayscale_scene(text_generator);
        btn::core::update();

        palette_grayscale_actions_scene(text_generator);
        btn::core::update();

        palette_fade_scene(text_generator);
        btn::core::update();

        palette_fade_actions_scene(text_generator);
        btn::core::update();

        palette_rotate_scene(text_generator);
        btn::core::update();

        palette_rotate_actions_scene(text_generator);
        btn::core::update();

        global_brightness_scene(text_generator);
        btn::core::update();

        global_brightness_actions_scene(text_generator);
        btn::core::update();

        global_contrast_scene(text_generator);
        btn::core::update();

        global_contrast_actions_scene(text_generator);
        btn::core::update();

        global_intensity_scene(text_generator);
        btn::core::update();

        global_intensity_actions_scene(text_generator);
        btn::core::update();

        transparent_color_hblank_effect_scene(text_generator);
        btn::core::update();

        palette_color_hblank_effect_scene(text_generator);
        btn::core::update();
    }
}
