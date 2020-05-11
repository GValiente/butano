#include "bf_game_enemies.h"

#include "bf_game_hero.h"
#include "bf_game_intro.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_check_hero_bullet_data.h"
#include "bf_game_stage_1.h"

namespace bf::game
{

enemies::enemies(const btn::sprite_palette_ptr& damage_palette) :
    _damage_palette(damage_palette)
{
}

bool enemies::check_hero_bullet(const check_hero_bullet_data& data)
{
    if(data.bullet_rect.y() < -(constants::play_height + 16))
    {
        return false;
    }

    return _grid.check_hero_bullet(data);
}

void enemies::check_hero_bomb(const btn::point& bomb_center, int bomb_squared_radius)
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
        enemy.check_hero_bomb(bomb_center, bomb_squared_radius);
        ++it;

        if(it != end)
        {
            ++it;
        }
    }
}

void enemies::update(const hero& hero, const hero_bomb& hero_bomb, const intro& intro, enemy_bullets& enemy_bullets)
{
    #if BF_CFG_ENEMIES_GRID_LOG_ENABLED
        bool grid_updated = _remove_enemies(hero, enemy_bullets);

        if(hero.alive() && ! hero_bomb.active() && ! intro.active())
        {
            grid_updated |= _add_enemies();
        }

        if(grid_updated)
        {
            _grid.log();
        }
    #else
        _remove_enemies(hero, enemy_bullets);

        if(hero.alive() && ! hero_bomb.active() && ! intro.active())
        {
            _add_enemies();
        }
    #endif
}

bool enemies::_remove_enemies(const hero& hero, enemy_bullets& enemy_bullets)
{
    const btn::fixed_point& hero_position = hero.body_position();
    auto before_it = _enemies.before_begin();
    auto it = _enemies.begin();
    auto end = _enemies.end();
    bool grid_updated = false;

    while(it != end)
    {
        enemy& enemy = *it;
        enemy.update(hero_position, enemy_bullets);

        if(enemy.done())
        {
            _grid.remove_enemy(enemy);
            grid_updated = true;
            it = _enemies.erase_after(before_it);
        }
        else
        {
            grid_updated |= _grid.update_enemy(enemy);
            before_it = it;
            ++it;
        }
    }

    return grid_updated;
}

bool enemies::_add_enemies()
{
    const btn::span<const enemy_event>& events = stage_1::get().enemy_events;
    int events_count = events.size();

    if(_event_index == events_count)
    {
        return false;
    }

    --_event_counter;

    if(_event_counter)
    {
        return false;
    }

    ++_event_index;

    if(_event_index == events_count)
    {
        return false;
    }

    BTN_ASSERT(! _enemies.full(), "Enemies list is full");

    auto new_enemy_tag = btn::max(int8_t(0), int8_t(_new_enemy_tag));
    ++_new_enemy_tag;

    const enemy_event& event = events[_event_index];
    _enemies.emplace_front(event, _damage_palette, new_enemy_tag);
    _grid.add_enemy(_enemies.front());
    _event_counter = event.wait_frames;
    return true;
}

}
