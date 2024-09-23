/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_timer.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_timers.h"
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
        "A: restart",
    };

    common::info info("Timer", info_text_lines, text_generator);
    info.set_show_always(true);

    bn::vector<bn::sprite_ptr, 64> text_sprites;
    text_generator.set_center_alignment();

    bn::timer timer;
    uint64_t ticks = 0;

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            ticks = 0;
        }

        ticks += timer.elapsed_ticks_with_restart();

        uint64_t frames = ticks / bn::timers::ticks_per_frame();
        uint64_t seconds = ticks / bn::timers::ticks_per_second();
        text_sprites.clear();
        text_generator.generate(0, -16, "Ticks: " + bn::to_string<32>(ticks), text_sprites);
        text_generator.generate(0, 0, "Frames: " + bn::to_string<32>(frames), text_sprites);
        text_generator.generate(0, 16, "Seconds: " + bn::to_string<32>(seconds), text_sprites);

        info.update();
        bn::core::update();
    }
}
