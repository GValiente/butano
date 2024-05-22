/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMIES_H
#define BF_GAME_ENEMIES_H

#include "bn_unique_ptr.h"
#include "bn_forward_list.h"
#include "bn_sprite_palette_ptr.h"
#include "bf_game_boss.h"
#include "bf_game_enemy.h"
#include "bf_game_enemies_grid.h"

namespace bf::game
{

class intro;
class stage;
class boss_intro;

class enemies
{

public:
    enemies(const stage& stage, const bn::sprite_palette_ptr& damage_palette);

    [[nodiscard]] bool check_hero(const bn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    void check_hero_bomb(const bn::point& bomb_center, int bomb_squared_radius, const bn::camera_ptr& camera);

    [[nodiscard]] bool boss_dying() const
    {
        return _boss && ! _boss->life();
    }

    [[nodiscard]] bool boss_dead() const
    {
        return _boss && _boss->dead();
    }

    [[nodiscard]] bool hero_should_look_down(const bn::fixed_point& hero_position, bool hero_is_looking_down) const;

    void update(const hero_bomb& hero_bomb, const intro& intro, const bn::camera_ptr& camera, hero& hero,
                enemy_bullets& enemy_bullets, objects& objects, boss_intro& boss_intro, scoreboard& scoreboard,
                background& background, rumble_manager& rumble_manager);

private:
    const bn::span<const enemy_event>& _events;
    boss::type _boss_type;
    bn::forward_list<enemy, constants::max_enemies> _enemies;
    bn::unique_ptr<boss> _boss;
    enemies_grid _grid;
    bn::sprite_palette_ptr _damage_palette;
    int _event_index = -1;
    int _event_counter = 1;
    int _new_enemy_tag = 0;
    bool _hero_bomb_check_odds = false;

    bool _remove_enemies(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                         enemy_bullets& enemy_bullets);

    bool _add_enemies(const bn::camera_ptr& camera);

    void _enable_boss_intro(boss_intro& boss_intro);
};

}

#endif
