#include "btn_core.h"
#include "btn_music.h"
#include "btn_sound.h"
#include "btn_blending.h"
#include "btn_regular_bg_actions.h"
#include "btn_sprite_text_generator.h"

#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_clouds_bg_item.h"
#include "btn_stage_1_bg_item.h"

#include "bf_hero.h"
#include "bf_stats.h"
#include "bf_hero_bullets.h"
#include "bf_sprite_fonts.h"

int main()
{
    btn::core::init();

    btn::regular_bg_ptr ground_bg = btn::regular_bg_ptr::create(0, 0, btn::bg_items::stage_1);
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
    bf::stats stats(text_generator);
    // btn::music::play(btn::music_items::battle_clean);
    btn::core::update();

    while(true)
    {
        hero.update();
        hero_bullets.update(hero);
        btn::core::update();
    }
}
