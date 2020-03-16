#include "btn_core.h"
#include "btn_music.h"
#include "btn_keypad.h"
#include "btn_sprite_text_generator.h"

#include "btn_music_items.h"

#include "bf_hero.h"
#include "bf_hero_bomb.h"
#include "bf_hero_bullets.h"
#include "bf_stats.h"
#include "bf_background.h"
#include "bf_scoreboard.h"
#include "bf_sprite_fonts.h"

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(bf::variable_8x8_sprite_font);
    text_generator.set_bg_priority(1);

    bf::hero hero;
    bf::hero_bomb hero_bomb;
    bf::hero_bullets hero_bullets;
    bf::background background;
    bf::scoreboard scoreboard(text_generator);
    bf::stats stats(text_generator);
    btn::core::update();

    while(true)
    {
        hero.update();
        hero_bomb.update(hero);
        hero_bullets.update(hero);
        background.update();
        scoreboard.update(hero);
        stats.update();
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
