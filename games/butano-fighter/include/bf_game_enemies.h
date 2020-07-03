#ifndef BF_GAME_ENEMIES_H
#define BF_GAME_ENEMIES_H

#include "btn_forward_list.h"
#include "btn_sprite_palette_ptr.h"
#include "bf_game_boss.h"
#include "bf_game_enemy.h"
#include "bf_game_enemies_grid.h"

namespace bf::game
{

class intro;
class stage;
class hero_bomb;
class boss_intro;

class enemies
{

public:
    enemies(const stage& stage, const btn::sprite_palette_ptr& damage_palette);

    [[nodiscard]] bool check_hero(const btn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    void check_hero_bomb(const btn::point& bomb_center, int bomb_squared_radius);

    void update(const hero& hero, const hero_bomb& hero_bomb, const intro& intro, enemy_bullets& enemy_bullets,
                boss_intro& boss_intro);

private:
    const btn::span<const enemy_event>& _events;
    boss::type _boss_type;
    btn::forward_list<enemy, constants::max_enemies> _enemies;
    btn::unique_ptr<boss> _boss;
    enemies_grid _grid;
    btn::sprite_palette_ptr _damage_palette;
    int _event_index = -1;
    int _event_counter = 1;
    int _new_enemy_tag = 0;
    bool _hero_bomb_check_odds = false;

    bool _remove_enemies(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets);

    bool _add_enemies();

    void _enable_boss_intro(boss_intro& boss_intro);
};

}

#endif
