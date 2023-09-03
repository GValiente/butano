#include "bn_core.h"
#include "bn_colors.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_string.h"
#include "bn_config_game_pak.h"
#include "../../butano/hw/include/bn_hw_tonc.h"

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

    while(true)
    {
        bn::core::update();
    }
}
