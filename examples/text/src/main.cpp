/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_math.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"

#include "fixed_8x8_sprite_font.h"
#include "fixed_8x16_sprite_font.h"
#include "variable_8x8_sprite_font.h"
#include "variable_8x16_sprite_font.h"

#include "btn_sprite_items_variable_8x16_font_red.h"
#include "btn_sprite_items_variable_8x16_font_blue.h"
#include "btn_sprite_items_variable_8x16_font_yellow.h"

namespace
{
    constexpr const btn::fixed text_y_inc = 14;
    constexpr const btn::fixed text_y_limit = (btn::display::height() / 2) - text_y_inc;

    void text_scene()
    {
        btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Text", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! btn::keypad::start_pressed())
        {
            btn::core::update();
        }
    }

    void text_fonts_scene()
    {
        btn::sprite_text_generator small_fixed_text_generator(fixed_8x8_sprite_font);
        small_fixed_text_generator.set_center_alignment();

        btn::sprite_text_generator small_variable_text_generator(variable_8x8_sprite_font);
        small_variable_text_generator.set_center_alignment();

        btn::sprite_text_generator big_fixed_text_generator(fixed_8x16_sprite_font);
        big_fixed_text_generator.set_center_alignment();

        btn::sprite_text_generator big_variable_text_generator(variable_8x16_sprite_font);
        big_variable_text_generator.set_center_alignment();

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        big_variable_text_generator.generate(0, -text_y_limit, "Text fonts", text_sprites);

        btn::fixed text_y = -text_y_inc - (text_y_inc / 2);
        small_fixed_text_generator.generate(0, text_y, "Small fixed width font", text_sprites);
        text_y += text_y_inc;

        small_variable_text_generator.generate(0, text_y, "Small variable width font", text_sprites);
        text_y += text_y_inc;

        big_fixed_text_generator.generate(0, text_y, "Big fixed width font", text_sprites);
        text_y += text_y_inc;

        big_variable_text_generator.generate(0, text_y, "Big variable width font", text_sprites);

        big_variable_text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! btn::keypad::start_pressed())
        {
            btn::core::update();
        }
    }

    void text_alignment_scene()
    {
        btn::sprite_text_generator text_generator(variable_8x16_sprite_font);

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        text_generator.set_center_alignment();
        text_generator.generate(0, -text_y_limit, "Text alignment", text_sprites);

        text_generator.set_left_alignment();
        text_generator.generate(0, -text_y_inc, "Left", text_sprites);

        text_generator.set_center_alignment();
        text_generator.generate(0, 0, "Center", text_sprites);

        text_generator.set_right_alignment();
        text_generator.generate(0, text_y_inc, "Right", text_sprites);

        text_generator.set_center_alignment();
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! btn::keypad::start_pressed())
        {
            btn::core::update();
        }
    }

    void text_colors_scene()
    {
        btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
        btn::sprite_palette_item original_palette_item = text_generator.palette_item();
        text_generator.set_center_alignment();

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Text colors", text_sprites);

        text_generator.set_palette_item(btn::sprite_items::variable_8x16_font_red.palette_item());
        text_generator.generate(0, -text_y_inc, "Red", text_sprites);

        text_generator.set_palette_item(btn::sprite_items::variable_8x16_font_blue.palette_item());
        text_generator.generate(0, 0, "Blue", text_sprites);

        text_generator.set_palette_item(btn::sprite_items::variable_8x16_font_yellow.palette_item());
        text_generator.generate(0, text_y_inc, "Yellow", text_sprites);

        text_generator.set_palette_item(original_palette_item);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! btn::keypad::start_pressed())
        {
            btn::core::update();
        }
    }

    void sprite_per_character_text_scene()
    {
        btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Sprite per character text", text_sprites);

        btn::vector<btn::sprite_ptr, 32> character_sprites;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(0, 0, "ONE SPRITE PER CHARACTER", character_sprites);

        text_generator.set_one_sprite_per_character(false);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        btn::fixed angle;
        btn::fixed angle_inc = 6;

        while(! btn::keypad::start_pressed())
        {
            angle += angle_inc;

            if(angle >= 360)
            {
                angle -= 360;
            }

            btn::fixed local_angle = angle;

            for(btn::sprite_ptr& character_sprite : character_sprites)
            {
                local_angle += angle_inc;

                if(local_angle >= 360)
                {
                    local_angle -= 360;
                }

                character_sprite.set_y(btn::degrees_sin(local_angle) * 16);
            }

            btn::core::update();
        }
    }

    void utf8_text_scene()
    {
        btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        btn::vector<btn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "UTF-8 text", text_sprites);
        text_generator.generate(0, 0, "UTF-8 characters: áéñÁÉÑ", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! btn::keypad::start_pressed())
        {
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    while(true)
    {
        text_scene();
        btn::core::update();

        text_fonts_scene();
        btn::core::update();

        text_alignment_scene();
        btn::core::update();

        text_colors_scene();
        btn::core::update();

        sprite_per_character_text_scene();
        btn::core::update();

        utf8_text_scene();
        btn::core::update();
    }
}
