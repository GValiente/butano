#ifndef BF_GAME_H
#define BF_GAME_H

#include "bf_game_background.h"
#include "bf_game_hero.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_hero_bullets.h"
#include "bf_game_scoreboard.h"

namespace bf
{

class game
{

public:
    explicit game(btn::sprite_text_generator& text_generator);

    void update();

private:
    game_background _background;
    game_hero _hero;
    game_hero_bomb _hero_bomb;
    game_hero_bullets _hero_bullets;
    game_scoreboard _scoreboard;
};

}

#endif
