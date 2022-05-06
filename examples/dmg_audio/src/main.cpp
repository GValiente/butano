/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_dmg_music.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_dmg_music_items_s3m_template.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    constexpr bn::string_view info_text_lines[] = {
        "LEFT: decrease volume",
        "RIGHT: increase volume",
        "A: pause/resume music",
    };

    common::info info("DMG music", info_text_lines, text_generator);
    info.set_show_always(true);

    bn::dmg_music_items::s3m_template.play();

    while(true)
    {
        bn::fixed volume = bn::dmg_music::left_volume();

        if(bn::keypad::left_held())
        {
            bn::dmg_music::set_volume(bn::max(volume - 0.01, bn::fixed(0)));
        }
        else if(bn::keypad::right_held())
        {
            bn::dmg_music::set_volume(bn::min(volume + 0.01, bn::fixed(1)));
        }

        if(bn::keypad::a_pressed())
        {
            if(bn::dmg_music::paused())
            {
                bn::dmg_music::resume();
            }
            else
            {
                bn::dmg_music::pause();
            }
        }

        info.update();
        bn::core::update();
    }
}
