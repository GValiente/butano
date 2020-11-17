/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

#include "bn_regular_bg_items_bg_384_320.h"
#include "bn_regular_bg_items_bg_1536_1280.h"

namespace
{
    void bg_384_320_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("384x320 regular BG", info_text_lines, text_generator);

        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_384_320.create_bg(0, 0);
        bg.set_x(72);
        bg.set_y(80);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed() || (bn::keypad::a_held() && bn::keypad::left_held()))
            {
                bg.set_x(bg.x() - 8);
            }
            else if(bn::keypad::right_pressed() || (bn::keypad::a_held() && bn::keypad::right_held()))
            {
                bg.set_x(bg.x() + 8);
            }

            if(bn::keypad::up_pressed() || (bn::keypad::a_held() && bn::keypad::up_held()))
            {
                bg.set_y(bg.y() - 8);
            }
            else if(bn::keypad::down_pressed() || (bn::keypad::a_held() && bn::keypad::down_held()))
            {
                bg.set_y(bg.y() + 8);
            }

            info.update();
            bn::core::update();
        }
    }

    void bg_1536_1280_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("1536x1280 regular BG", info_text_lines, text_generator);

        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_1536_1280.create_bg(0, 0);
        bg.set_x(320 - 56 + 384);
        bg.set_y(240 + 320);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed() || (bn::keypad::a_held() && bn::keypad::left_held()))
            {
                bg.set_x(bg.x() - 8);
            }
            else if(bn::keypad::right_pressed() || (bn::keypad::a_held() && bn::keypad::right_held()))
            {
                bg.set_x(bg.x() + 8);
            }

            if(bn::keypad::up_pressed() || (bn::keypad::a_held() && bn::keypad::up_held()))
            {
                bg.set_y(bg.y() - 8);
            }
            else if(bn::keypad::down_pressed() || (bn::keypad::a_held() && bn::keypad::down_held()))
            {
                bg.set_y(bg.y() + 8);
            }

            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);

    while(true)
    {
        bg_1536_1280_scene(text_generator);
        bn::core::update();
    }
}
