#include "bf_game_enemies.h"

#include "bf_game_hero.h"
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
    for(enemy* enemy : _list)
    {
        enemy->check_hero_bomb(bomb_center, bomb_squared_radius);
    }
}

void enemies::update(const hero& hero, const hero_bomb& hero_bomb, enemy_bullets& enemy_bullets)
{
    bool grid_updated = _remove_enemies(hero, enemy_bullets);

    if(! hero_bomb.active())
    {
        grid_updated |= _add_enemies();
    }

    #if BF_CFG_ENEMIES_GRID_LOG_ENABLED
        if(grid_updated)
        {
            _grid.log();
        }
    #endif
}

bool enemies::_remove_enemies(const hero& hero, enemy_bullets& enemy_bullets)
{
    const btn::fixed_point& hero_position = hero.body_position();
    int enemies_count = _list.size();
    bool grid_updated = false;

    for(int index = 0; index < enemies_count; )
    {
        enemy*& enemy = _list[index];
        enemy->update(hero_position, enemy_bullets);

        if(enemy->done())
        {
            _grid.remove_enemy(*enemy);
            _pool.destroy(*enemy);
            grid_updated = true;

            if(index < enemies_count - 1)
            {
                btn::swap(enemy, _list[enemies_count - 1]);
            }

            --enemies_count;
        }
        else
        {
            grid_updated |= _grid.update_enemy(*enemy);
            ++index;
        }
    }

    _list.shrink(enemies_count);
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

    BTN_ASSERT(! _list.full(), "Enemies list is full");

    auto new_enemy_tag = btn::max(int8_t(0), int8_t(_new_enemy_tag));
    ++_new_enemy_tag;

    const enemy_event& event = events[_event_index];
    enemy& enemy = _pool.create(event, _damage_palette, new_enemy_tag);
    _list.emplace_back(&enemy);
    _grid.add_enemy(enemy);
    _event_counter = event.wait_frames;
    return true;
}

}
