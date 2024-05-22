/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_rumble.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    constexpr bn::string_view info_text_lines[] = {
        "A: hold to rumble",
    };

    common::info info("Rumble", info_text_lines, text_generator);

    bn::vector<bn::sprite_ptr, 4> text_sprites;

    while(true)
    {
        if(bn::keypad::a_held())
        {
            if(! bn::rumble::enabled())
            {
                bn::rumble::set_enabled(true);
                text_sprites.clear();
                text_generator.generate(0, 0, "Rumble!", text_sprites);
            }
        }
        else
        {
            if(bn::rumble::enabled())
            {
                bn::rumble::set_enabled(false);
                text_sprites.clear();
            }
        }

        info.update();
        bn::core::update();
    }
}
