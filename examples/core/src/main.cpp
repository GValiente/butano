/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_music.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "bn_music_items.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void cpu_usage_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("CPU usage", info_text_lines, text_generator);

        bn::vector<bn::sprite_ptr, 2> text_sprites;
        bn::fixed max_cpu_usage;
        int counter = 1;
        bn::music_items::soda7_xcopy_ohc.play(0.5);

        while(! bn::keypad::start_pressed())
        {
            max_cpu_usage = bn::max(max_cpu_usage, bn::core::last_cpu_usage());
            --counter;

            if(! counter)
            {
                bn::string<32> text;
                bn::ostringstream text_stream(text);
                text_stream.append((max_cpu_usage * 100).right_shift_integer());
                text_stream.append("%");
                text_sprites.clear();
                text_generator.generate(0, 0, text, text_sprites);

                max_cpu_usage = 0;
                counter = 60;
            }

            info.update();
            bn::core::update();
        }

        bn::music::stop();
    }

    void sleep_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: sleep",
            "B: wake up",
            "",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Sleep", info_text_lines, text_generator);
        info.set_show_always(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                bn::core::sleep(bn::keypad::key_type::B);
            }

            info.update();
            bn::core::update();
        }
    }

    [[noreturn]] void reset_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: reset",
        };

        common::info info("Reset", info_text_lines, text_generator);

        while(true)
        {
            if(bn::keypad::start_pressed())
            {
                bn::core::reset();
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
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    cpu_usage_scene(text_generator);
    bn::core::update();

    sleep_scene(text_generator);
    bn::core::update();

    reset_scene(text_generator);
}
