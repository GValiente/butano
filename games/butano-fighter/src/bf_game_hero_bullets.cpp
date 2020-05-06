#include "bf_game_hero_bullets.h"

#include "btn_keypad.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_hero_bullets.h"
#include "bf_game_hero.h"
#include "bf_game_enemies.h"
#include "bf_game_hero_bullet_level.h"
#include "bf_game_check_hero_bullet_data.h"

namespace bf::game
{

hero_bullets::hero_bullets() :
    _palette(btn::sprite_items::hero_bullets.palette_item().create_palette()),
    _tiles({ btn::sprite_items::hero_bullets.tiles_item().create_tiles(0),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(1),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(2),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(3),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(4),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(5),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(6),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(7),
           btn::sprite_items::hero_bullets.tiles_item().create_tiles(8) })
{
}

void hero_bullets::update(hero& hero, enemies& enemies, objects& objects)
{
    if(hero.alive())
    {
        if(btn::keypad::held(btn::keypad::button_type::B))
        {
            _b_held_counter = 10;
        }
        else if(_b_held_counter)
        {
            --_b_held_counter;
        }
    }
    else
    {
        _b_held_counter = 0;
    }

    _remove_bullets(hero, enemies, objects);
    hero.set_shooting(_b_held_counter);

    if(_event_counter || _b_held_counter)
    {
        ++_event_counter;
    }

    if(_b_held_counter)
    {
        _add_bullets(hero);
    }

    btn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();

    if(_event_counter >= levels_data[hero.level()].loop_frames)
    {
        _event_counter = 0;
    }
}

void hero_bullets::_remove_bullets(hero& hero, enemies& enemies, objects& objects)
{
    for(bullet& bullet : _even_bullets)
    {
        bullet.sprite_move_action.update();
    }

    for(bullet& bullet : _odd_bullets)
    {
        bullet.sprite_move_action.update();
    }

    btn::iforward_list<bullet>* bullets = _check_odds ? &_odd_bullets : &_even_bullets;
    _check_odds = ! _check_odds;

    auto before_it = bullets->before_begin();
    auto it = bullets->begin();
    auto end = bullets->end();

    while(it != end)
    {
        bullet& bullet = *it;
        const btn::fixed_point& position = bullet.sprite_move_action.sprite().position();
        const hero_bullet_level& level_data = *bullet.level_data;

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height ||
                enemies.check_hero_bullet({ btn::fixed_rect(position, level_data.dimensions),
                                          level_data.damage, hero, objects }))
        {
            it = bullets->erase_after(before_it);
        }
        else
        {
            before_it = it;
            ++it;
        }
    }
}

void hero_bullets::_add_bullets(hero& hero)
{
    btn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();
    int counter = _event_counter;

    for(const hero_bullet_event& event : levels_data[hero.level()].events)
    {
        if(counter == event.frame)
        {
            btn::iforward_list<bullet>* bullets = _odd_bullets.size() < _even_bullets.size() ?
                        &_odd_bullets : &_even_bullets;
            BTN_ASSERT(! bullets->full(), "No more space for sprite bullets");

            int event_level = event.level;
            const hero_bullet_level& level_data = levels_data[event_level];
            btn::sprite_builder builder(btn::sprite_items::hero_bullets.shape_size(), _tiles[event_level], _palette);
            builder.set_position(hero.weapon_position());
            builder.set_z_order(constants::hero_bullets_z_order);
            bullets->push_front({ btn::sprite_move_by_action(builder.release_build(), event.direction), &level_data });
            hero.show_shoot(level_data.color);

            if(event.play_sound)
            {
                level_data.sound_item.play_with_priority(constants::hero_bullets_sound_priority, 0.35);
            }

            return;
        }
    }
}

}
