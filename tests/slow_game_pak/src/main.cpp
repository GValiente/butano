/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_colors.h"
#include "bn_format.h"
#include "bn_bg_palettes.h"
#include "bn_config_game_pak.h"
#include "bn_sprite_text_generator.h"

#include "../../butano/hw/include/bn_hw_tonc.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_music_items.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::colors::gray);

    bn::string<32> first_wait_state = "First wait state: ";

    switch(REG_WAITCNT & 0b1100)
    {

    case BN_GAME_PAK_WAIT_STATE_FIRST_3:
        first_wait_state += '3';
        break;

    case BN_GAME_PAK_WAIT_STATE_FIRST_2:
        first_wait_state += '2';
        break;

    case BN_GAME_PAK_WAIT_STATE_FIRST_8:
        first_wait_state += '8';
        break;

    default:
        first_wait_state += '4';
        break;
    }

    bn::string<32> second_wait_state = "Second wait state: ";

    switch(REG_WAITCNT & 0b10000)
    {

    case BN_GAME_PAK_WAIT_STATE_SECOND_1:
        second_wait_state += '1';
        break;

    default:
        second_wait_state += '2';
        break;
    }

    bn::string<64> prefetch = "Prefetch: ";
    prefetch += bn::to_string<64>(REG_WAITCNT & 0x4000 ? "true" : "false");

    bn::string<64> waitcnt = "WAITCNT register: ";
    waitcnt += bn::to_string<64>(REG_WAITCNT);

    bn::string_view info_text_lines[] = {
        bn::core::slow_game_pak() ? "Detected speed: SLOW" : "Detected speed: FAST",
        first_wait_state,
        second_wait_state,
        prefetch,
        waitcnt,
    };

    common::info info("Slow game pak test", info_text_lines, text_generator);
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
