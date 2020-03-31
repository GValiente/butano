#ifndef BF_GAME_ENEMIES_H
#define BF_GAME_ENEMIES_H

#include "btn_sprite_actions.h"
#include "bf_constants.h"
#include "bf_game_enemy.h"
#include "bf_game_enemies_grid.h"

namespace bf::game
{

class enemies
{

public:
    bool check_hero_bullet(const btn::fixed_rect& rect, int damage);

    void update();

private:
    btn::vector<enemy, constants::max_enemies> _enemies;
    enemies_grid _grid;
    int _event_index = -1;
    int _event_counter = 1;

    void _remove_enemies();

    void _add_enemies();

};

}

#endif
