#ifndef BF_GAME_ENEMIES_H
#define BF_GAME_ENEMIES_H

#include "btn_pool.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_palette_ptr.h"
#include "bf_constants.h"
#include "bf_game_enemy.h"
#include "bf_game_enemies_grid.h"

namespace bf::game
{

class hero_bomb;

class enemies
{

public:
    enemies();

    [[nodiscard]] bool check_hero_bullet(const btn::fixed_rect& rect, int damage, hero& hero)
    {
        return _grid.check_hero_bullet(rect, damage, hero);
    }

    void check_hero_bomb(const btn::point& center, int squared_radius);

    void update(const hero_bomb& hero_bomb);

private:
    btn::pool<enemy, constants::max_enemies> _pool;
    btn::vector<enemy*, constants::max_enemies> _list;
    enemies_grid _grid;
    btn::sprite_palette_ptr _damage_palette;
    int _event_index = -1;
    int _event_counter = 1;
    int _new_enemy_tag = 0;

    [[nodiscard]] bool _remove_enemies();

    [[nodiscard]] bool _add_enemies();

};

}

#endif
