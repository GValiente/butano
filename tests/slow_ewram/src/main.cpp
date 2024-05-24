/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_colors.h"
#include "bn_format.h"
#include "bn_memory.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_music_items.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::colors::gray);

    bn::string_view info_text_lines[] = {
        bn::memory::fast_ewram() ? "Detected speed: FAST" : "Detected speed: SLOW",
        "",
    };

    common::info info("Slow EWRAM test", info_text_lines, text_generator);
    info.set_show_always(true);

    bn::music_items::fruit.play();

    bn::vector<bn::sprite_ptr, 8> cpu_sprites;
    bn::fixed max_cpu_usage;
    int frame_counter = 0;

    while(true)
    {
        max_cpu_usage = bn::max(max_cpu_usage, bn::core::last_cpu_usage());
        ++frame_counter;

        if(frame_counter == 60)
        {
            bn::fixed max_cpu_usage_pct = max_cpu_usage * 100;
            int max_cpu_usage_pct_int = max_cpu_usage_pct.right_shift_integer();
            int max_cpu_usage_pct_dec = ((max_cpu_usage_pct - max_cpu_usage_pct_int) * 100).right_shift_integer();
            cpu_sprites.clear();
            text_generator.set_right_alignment();
            text_generator.generate(
                112, 64, bn::format<16>("CPU: {}.{}%", max_cpu_usage_pct_int, max_cpu_usage_pct_dec),
                cpu_sprites);
            max_cpu_usage = 0;
            frame_counter = 0;
        }

        bn::core::update();
    }
}
