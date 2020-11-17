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

#include "bn_regular_bg_items_bg_384_320_bpp8.h"
#include "bn_regular_bg_items_bg_512_512_bpp8.h"

namespace
{
    void bg_384_320_bpp8_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("384x320 BPP8 regular BG", info_text_lines, text_generator);

        bn::regular_bg_ptr bg = bn::regular_bg_items::bg_384_320_bpp8.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                bg.set_x(bg.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                bg.set_x(bg.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                bg.set_y(bg.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                bg.set_y(bg.y() + 1);
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
        bg_384_320_bpp8_scene(text_generator);
        bn::core::update();
    }
}
