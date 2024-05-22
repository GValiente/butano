/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_enemies.h"

#include "bf_game_hero.h"
#include "bf_game_intro.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_boss_intro.h"

namespace bf::game
{

enemies::enemies(const stage& stage, const bn::sprite_palette_ptr& damage_palette) :
    _events(stage.enemy_events),
    _boss_type(stage.boss_type),
    _damage_palette(damage_palette)
{
}

bool enemies::check_hero(const bn::fixed_rect& hero_rect) const
{
    if(_boss)
    {
        return _boss->check_hero(hero_rect);
    }

    return _grid.check_hero(hero_rect);
}

bool enemies::check_hero_bullet(const check_hero_bullet_data& data)
{
    if(_boss)
    {
        return _boss->check_hero_bullet(data);
    }

    return _grid.check_hero_bullet(data);
}

void enemies::check_hero_bomb(const bn::point& bomb_center, int bomb_squared_radius, const bn::camera_ptr& camera)
{
    if(! _boss)
    {
        auto it = _enemies.begin();
        auto end = _enemies.end();
        _hero_bomb_check_odds = ! _hero_bomb_check_odds;

        if(_hero_bomb_check_odds && it != end)
        {
            ++it;
        }

        while(it != end)
        {
            enemy& enemy = *it;
            enemy.check_hero_bomb(bomb_center, bomb_squared_radius, camera);
            ++it;

            if(it != end)
            {
                ++it;
            }
        }
    }
}

bool enemies::hero_should_look_down(const bn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_boss)
    {
        return _boss->hero_should_look_down(hero_position, hero_is_looking_down);
    }

    return false;
}

void enemies::update(const hero_bomb& hero_bomb, const intro& intro, const bn::camera_ptr& camera, hero& hero,
                     enemy_bullets& enemy_bullets, objects& objects, boss_intro& boss_intro, scoreboard& scoreboard,
                     background& background, rumble_manager& rumble_manager)
{
    if(_boss)
    {
        _boss->update(hero_bomb, camera, hero, enemy_bullets, objects, scoreboard, background, rumble_manager);
    }
    else
    {
        const bn::fixed_point& hero_position = hero.body_position();

        #if BF_CFG_ENEMIES_GRID_LOG_ENABLED
            bool grid_updated = _remove_enemies(hero_position, camera, enemy_bullets);

            if(hero.alive() && ! hero_bomb.active() && ! intro.active())
            {
                grid_updated |= _add_enemies(camera);
                _enable_boss_intro(boss_intro);
            }

            if(grid_updated)
            {
                _grid.log();
            }
        #else
            _remove_enemies(hero_position, camera, enemy_bullets);

            if(hero.alive() && ! hero_bomb.active() && ! intro.active())
            {
                _add_enemies(camera);
                _enable_boss_intro(boss_intro);
            }
        #endif

        if(boss_intro.done())
        {
            _boss = boss::create(_boss_type, hero_position, _damage_palette, camera);
            _boss->play_music();
        }
    }
}

bool enemies::_remove_enemies(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                              enemy_bullets& enemy_bullets)
{
    auto before_it = _enemies.before_begin();
    auto it = _enemies.begin();
    auto end = _enemies.end();
    bool grid_updated = false;

    while(it != end)
    {
        enemy& enemy = *it;
        enemy.update(hero_position, camera, enemy_bullets);
        grid_updated |= _grid.update_enemy(enemy);

        if(enemy.done())
        {
            _grid.remove_enemy(enemy);
            grid_updated = true;
            it = _enemies.erase_after(before_it);
        }
        else
        {
            before_it = it;
            ++it;
        }
    }

    return grid_updated;
}

bool enemies::_add_enemies(const bn::camera_ptr& camera)
{
    int events_count = _events.size();

    if(_event_index == events_count)
    {
        return false;
    }

    if(_event_counter)
    {
        --_event_counter;

        if(_event_counter)
        {
            return false;
        }
    }
    else
    {
        if(! _enemies.empty())
        {
            return false;
        }
    }

    ++_event_index;

    if(_event_index == events_count)
    {
        return false;
    }

    BN_ASSERT(! _enemies.full(), "Enemies list is full");

    auto new_enemy_tag = bn::max(int8_t(0), int8_t(_new_enemy_tag));
    ++_new_enemy_tag;

    const enemy_event& event = _events[_event_index];
    _enemies.emplace_front(event, _damage_palette, new_enemy_tag, camera);
    _grid.add_enemy(_enemies.front());
    _event_counter = event.wait_frames;
    return true;
}

void enemies::_enable_boss_intro(boss_intro& boss_intro)
{
    if(_event_index == _events.size() && ! boss_intro.active() && ! boss_intro.done() && _enemies.empty())
    {
        boss_intro.enable();
    }
}

}
