#include "bf_game_enemies.h"

#include "bf_game_stage_1.h"

namespace bf::game
{

bool enemies::check_hero_bullet(const btn::fixed_rect& rect, int damage)
{
    return _grid.check_hero_bullet(rect, damage, _enemies);
}

void enemies::update()
{
    _remove_enemies();
    _add_enemies();
}

void enemies::_remove_enemies()
{
    int enemies_count = _enemies.size();

    for(int index = 0; index < enemies_count; )
    {
        enemy& enemy = _enemies[index];
        btn::fixed_point old_position = enemy.position();
        enemy.update();

        if(enemy.done())
        {
            _grid.remove_enemy(index, old_position);

            if(index < enemies_count - 1)
            {
                btn::swap(enemy, _enemies[enemies_count - 1]);
            }

            --enemies_count;
        }
        else
        {
            _grid.update_enemy(index, old_position, enemy.position());
            ++index;
        }
    }

    _enemies.shrink(enemies_count);
}

void enemies::_add_enemies()
{
    const btn::span<const enemy_event>& events = stage_1::get().enemy_events;
    int events_count = events.size();

    if(_event_index == events_count)
    {
        return;
    }

    --_event_counter;

    if(! _event_counter)
    {
        ++_event_index;

        if(_event_index == events_count)
        {
            return;
        }

        const enemy_event& event = events[_event_index];
        _grid.add_enemy(_enemies.size(), event.start_position);
        _enemies.emplace_back(event);
        _event_counter = event.wait_frames;
    }
}

}
