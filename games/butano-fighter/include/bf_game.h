#ifndef BF_GAME_H
#define BF_GAME_H

#include "bf_scene.h"
#include "bf_game_background.h"
#include "bf_game_hero.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_hero_bullets.h"
#include "bf_game_enemies.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_objects.h"
#include "bf_game_scoreboard.h"

namespace bf
{
    class background;
}

namespace bf::game
{

class game : public scene
{

public:
    explicit game(btn::sprite_text_generator& text_generator);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    background _background;
    hero _hero;
    hero_bullets _hero_bullets;
    hero_bomb _hero_bomb;
    enemies _enemies;
    enemy_bullets _enemy_bullets;
    objects _objects;
    scoreboard _scoreboard;
};

}

#endif
