#include "bf_game.h"

namespace bf
{

game::game(btn::sprite_text_generator& text_generator) :
    _scoreboard(text_generator)
{
}

void game::update()
{
    _hero.update();
    _hero_bomb.update(_hero);
    _hero_bullets.update(_hero);
    _background.update();
    _scoreboard.update(_hero);
}

}
