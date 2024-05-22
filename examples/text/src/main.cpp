/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "fixed_32x64_sprite_font.h"

#include "bn_sprite_items_variable_8x16_font_red.h"
#include "bn_sprite_items_variable_8x16_font_blue.h"
#include "bn_sprite_items_variable_8x16_font_yellow.h"

#include "common_fixed_8x8_sprite_font.h"
#include "common_fixed_8x16_sprite_font.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;

    void text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Text", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void text_fonts_scene()
    {
        bn::sprite_text_generator small_fixed_text_generator(common::fixed_8x8_sprite_font);
        small_fixed_text_generator.set_center_alignment();

        bn::sprite_text_generator small_variable_text_generator(common::variable_8x8_sprite_font);
        small_variable_text_generator.set_center_alignment();

        bn::sprite_text_generator big_fixed_text_generator(common::fixed_8x16_sprite_font);
        big_fixed_text_generator.set_center_alignment();

        bn::sprite_text_generator big_variable_text_generator(common::variable_8x16_sprite_font);
        big_variable_text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        big_variable_text_generator.generate(0, -text_y_limit, "Text fonts", text_sprites);

        bn::fixed text_y = -text_y_inc - (text_y_inc / 2);
        small_fixed_text_generator.generate(0, text_y, "Small fixed width font", text_sprites);
        text_y += text_y_inc;

        small_variable_text_generator.generate(0, text_y, "Small variable width font", text_sprites);
        text_y += text_y_inc;

        big_fixed_text_generator.generate(0, text_y, "Big fixed width font", text_sprites);
        text_y += text_y_inc;

        big_variable_text_generator.generate(0, text_y, "Big variable width font", text_sprites);

        big_variable_text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void text_alignment_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

        bn::vector<bn::sprite_ptr, 32> text_sprites;
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

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void text_colors_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        bn::sprite_palette_item original_palette_item = text_generator.palette_item();
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Text colors", text_sprites);

        text_generator.set_palette_item(bn::sprite_items::variable_8x16_font_red.palette_item());
        text_generator.generate(0, -text_y_inc, "Red", text_sprites);

        text_generator.set_palette_item(bn::sprite_items::variable_8x16_font_blue.palette_item());
        text_generator.generate(0, 0, "Blue", text_sprites);

        text_generator.set_palette_item(bn::sprite_items::variable_8x16_font_yellow.palette_item());
        text_generator.generate(0, text_y_inc, "Yellow", text_sprites);

        text_generator.set_palette_item(original_palette_item);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void sprite_per_character_text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Sprite per character text", text_sprites);

        bn::vector<bn::sprite_ptr, 32> character_sprites;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(0, 0, "ONE SPRITE PER CHARACTER", character_sprites);

        text_generator.set_one_sprite_per_character(false);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        bn::fixed angle;
        bn::fixed angle_inc = 6;

        while(! bn::keypad::start_pressed())
        {
            angle += angle_inc;

            if(angle >= 360)
            {
                angle -= 360;
            }

            bn::fixed local_angle = angle;

            for(bn::sprite_ptr& character_sprite : character_sprites)
            {
                local_angle += angle_inc;

                if(local_angle >= 360)
                {
                    local_angle -= 360;
                }

                character_sprite.set_y(bn::degrees_lut_sin(local_angle) * 16);
            }

            bn::core::update();
        }
    }

    void huge_text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "Huge text", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        bn::sprite_text_generator huge_text_generator(fixed_32x64_sprite_font);
        huge_text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> huge_text_sprites;
        huge_text_generator.generate(0, 0, "HUGE", text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }

    void utf8_text_scene()
    {
        bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
        text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> text_sprites;
        text_generator.generate(0, -text_y_limit, "UTF-8 text", text_sprites);
        text_generator.generate(0, 0, "UTF-8 characters: áéñÁÉÑ", text_sprites);
        text_generator.generate(0, text_y_limit, "START: go to next scene", text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        text_scene();
        bn::core::update();

        text_fonts_scene();
        bn::core::update();

        text_alignment_scene();
        bn::core::update();

        text_colors_scene();
        bn::core::update();

        sprite_per_character_text_scene();
        bn::core::update();

        huge_text_scene();
        bn::core::update();

        utf8_text_scene();
        bn::core::update();
    }
}
