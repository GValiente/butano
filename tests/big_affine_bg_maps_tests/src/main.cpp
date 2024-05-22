/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_affine_bg_items_big_map.h"
#include "bn_affine_bg_items_border_map.h"

namespace
{
    void big_map_scene(const bn::string_view& title, const bn::affine_bg_item& item,
                       bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move BG",
            "A: move BG faster",
            "",
            "START: go to next scene",
        };

        common::info info(title, info_text_lines, text_generator);

        bn::affine_bg_ptr bg = item.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            int inc = bn::keypad::a_held() ? 8 : 1;

            if(bn::keypad::left_held())
            {
                bg.set_x(bg.x().right_shift_integer() - inc);
            }
            else if(bn::keypad::right_held())
            {
                bg.set_x(bg.x().right_shift_integer() + inc);
            }

            if(bn::keypad::up_held())
            {
                bg.set_y(bg.y().right_shift_integer() - inc);
            }
            else if(bn::keypad::down_held())
            {
                bg.set_y(bg.y().right_shift_integer() + inc);
            }

            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    while(true)
    {
        big_map_scene("4096x4096 affine BG", bn::affine_bg_items::big_map, text_generator);
        bn::core::update();

        big_map_scene("512x1024 borders affine BG", bn::affine_bg_items::border_map, text_generator);
        bn::core::update();
    }
}
