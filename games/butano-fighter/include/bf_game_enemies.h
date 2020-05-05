#ifndef BF_GAME_ENEMIES_H
#define BF_GAME_ENEMIES_H

#include "btn_forward_list.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_palette_ptr.h"
#include "bf_constants.h"
#include "bf_game_enemy.h"
#include "bf_game_enemies_grid.h"

namespace bf::game
{

class intro;
class hero_bomb;

class enemies
{

public:
    explicit enemies(const btn::sprite_palette_ptr& damage_palette);

    [[nodiscard]] bool check_hero(const btn::fixed_rect& hero_rect) const
    {
        return _grid.check_hero(hero_rect);
    }

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    void check_hero_bomb(const btn::point& bomb_center, int bomb_squared_radius);

    void update(const hero& hero, const hero_bomb& hero_bomb, const intro& intro, enemy_bullets& enemy_bullets);

private:
    btn::forward_list<enemy, constants::max_enemies> _list;
    enemies_grid _grid;
    btn::sprite_palette_ptr _damage_palette;
    int _event_index = -1;
    int _event_counter = 1;
    int _new_enemy_tag = 0;
    bool _hero_bomb_check_odds = false;

    [[nodiscard]] bool _remove_enemies(const hero& hero, enemy_bullets& enemy_bullets);

    [[nodiscard]] bool _add_enemies();

};

}

#endif
