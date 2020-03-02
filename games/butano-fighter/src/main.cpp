#include "btn_core.h"
#include "btn_music.h"
#include "btn_keypad.h"
#include "btn_blending.h"
#include "btn_regular_bg_actions.h"
#include "btn_sprite_text_generator.h"

#include "btn_music_items.h"
#include "btn_clouds_bg_item.h"
#include "btn_stage_1_bg_item.h"

#include "bf_hero.h"
#include "bf_stats.h"
#include "bf_scoreboard.h"
#include "bf_hero_bullets.h"
#include "bf_sprite_fonts.h"

#include "btn_math.h"
#include "btn_bgs_mosaic.h"
#include "btn_regular_bg_hblank_effects.h"

int main()
{
    btn::core::init();

    btn::regular_bg_ptr ground_bg = btn::regular_bg_ptr::create(0, 0, btn::bg_items::stage_1);

    btn::fixed hblank_deltas[160];
    int speed = 1024;
    int amplitude = 4;

    for(int index = 0; index < 160; ++index)
    {
        int sin = btn::lut_sin((index * speed) % 65536).value();
        hblank_deltas[index] = (sin / (4096 / (1 << amplitude))) - (1 << (amplitude - 1));
    }

    btn::regular_bg_position_hblank_effect_ptr ground_bg_hblank_effect =
            btn::regular_bg_position_hblank_effect_ptr::create_horizontal(
                ground_bg, btn::span<const btn::fixed>(hblank_deltas, 160));

    btn::bgs_mosaic::set_horizontal_stretch(0.1);

    btn::regular_bg_move_by_action ground_bg_move_action(btn::move(ground_bg), 0, 0.5);
    ground_bg_move_action.run();

    btn::regular_bg_ptr clouds_bg = btn::regular_bg_ptr::create(0, 0, btn::bg_items::clouds);
    clouds_bg.set_priority(2);
    clouds_bg.set_blending_enabled(true);
    btn::blending::set_transparency_alpha(0.4);

    btn::regular_bg_move_by_action clouds_bg_move_action(btn::move(clouds_bg), -1.0 / 16, 0.5);
    clouds_bg_move_action.run();

    btn::sprite_text_generator text_generator(bf::variable_8x8_sprite_font);
    text_generator.set_bg_priority(1);

    bf::hero hero;
    bf::hero_bullets hero_bullets;
    bf::scoreboard scoreboard(text_generator);
    bf::stats stats(text_generator);
    btn::core::update();

    while(true)
    {
        hero.update();
        hero_bullets.update(hero);
        scoreboard.update(hero);
        btn::core::update();

        if(btn::keypad::pressed(btn::keypad::button_type::START))
        {
            if(btn::music::playing())
            {
                btn::music::stop();
            }
            else
            {
                btn::music::play(btn::music_items::battle_clean, 0.5);
            }
        }

        if(btn::keypad::pressed(btn::keypad::button_type::SELECT))
        {
            btn::core::sleep(btn::keypad::button_type::SELECT);
        }
    }
}
