#include "bf_game.h"

#include "btn_music.h"
#include "btn_keypad.h"
#include "btn_music_items.h"

namespace bf::game
{

game::game(btn::sprite_text_generator& text_generator) :
    _scoreboard(text_generator)
{
}

void game::update()
{
    _hero.update();
    _hero_bomb.update(_hero, _background);
    _hero_bullets.update(_hero, _enemies);
    _background.update();
    _enemies.update();
    _scoreboard.update(_hero);

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
}

}
