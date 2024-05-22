/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_hero_bullets.h"

#include "bn_keypad.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_hero_bullets.h"
#include "bf_game_hero.h"
#include "bf_game_enemies.h"
#include "bf_game_hero_bullet_level.h"
#include "bf_game_check_hero_bullet_data.h"

namespace bf::game
{

hero_bullets::hero_bullets() :
    _palette(bn::sprite_items::hero_bullets.palette_item().create_palette()),
    _tiles({ bn::sprite_items::hero_bullets.tiles_item().create_tiles(0),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(1),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(2),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(3),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(4),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(5),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(6),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(7),
           bn::sprite_items::hero_bullets.tiles_item().create_tiles(8) })
{
}

void hero_bullets::update(const bn::camera_ptr& camera, hero& hero, enemies& enemies, objects& objects)
{
    if(hero.alive())
    {
        if(bn::keypad::b_held())
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

    _remove_bullets(camera, hero, enemies, objects);
    hero.set_shooting(_b_held_counter);

    if(_event_counter || _b_held_counter)
    {
        ++_event_counter;
    }

    if(_b_held_counter)
    {
        _add_bullets(camera, hero);
    }

    bn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();

    if(_event_counter >= levels_data[hero.level()].loop_frames)
    {
        _event_counter = 0;
    }
}

void hero_bullets::_remove_bullets(const bn::camera_ptr& camera, hero& hero, enemies& enemies, objects& objects)
{
    bn::iforward_list<bullet_type>* check_and_update_bullets;
    bn::iforward_list<bullet_type>* update_bullets;

    if(_check_odds)
    {
        check_and_update_bullets = &_odd_bullets;
        update_bullets = &_even_bullets;
        _check_odds = false;
    }
    else
    {
        check_and_update_bullets = &_even_bullets;
        update_bullets = &_odd_bullets;
        _check_odds = true;
    }

    auto before_it = check_and_update_bullets->before_begin();
    auto it = check_and_update_bullets->begin();
    auto end = check_and_update_bullets->end();

    if(hero.alive())
    {
        check_hero_bullet_data check_data{ bn::fixed_rect(), 0, camera, hero, objects };

        while(it != end)
        {
            bullet_type& bullet = *it;
            bn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
            const bn::fixed_point& position = sprite_move_action.sprite().position();
            const hero_bullet_level& level_data = *bullet.level_data;
            check_data.bullet_rect = bn::fixed_rect(position, level_data.dimensions);
            check_data.bullet_damage = level_data.damage;

            if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                    position.y() < -constants::view_height || position.y() > constants::view_height ||
                    enemies.check_hero_bullet(check_data))
            {
                it = check_and_update_bullets->erase_after(before_it);
            }
            else
            {
                sprite_move_action.update();
                before_it = it;
                ++it;
            }
        }
    }
    else
    {
        while(it != end)
        {
            bullet_type& bullet = *it;
            bn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
            const bn::fixed_point& position = sprite_move_action.sprite().position();

            if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                    position.y() < -constants::view_height || position.y() > constants::view_height)
            {
                it = check_and_update_bullets->erase_after(before_it);
            }
            else
            {
                sprite_move_action.update();
                before_it = it;
                ++it;
            }
        }
    }

    for(bullet_type& bullet : *update_bullets)
    {
        bullet.sprite_move_action.update();
    }
}

void hero_bullets::_add_bullets(const bn::camera_ptr& camera, hero& hero)
{
    bn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();
    bool looking_down = hero.looking_down();
    int counter = _event_counter;

    for(const hero_bullet_event& event : levels_data[hero.level()].events)
    {
        if(counter == event.frame)
        {
            bn::iforward_list<bullet_type>* bullets = _odd_bullets.size() < _even_bullets.size() ?
                        &_odd_bullets : &_even_bullets;
            BN_ASSERT(! bullets->full(), "No more space for hero bullets");

            const bn::fixed_point& bullet_position = hero.weapon_position();
            bn::fixed_point bullet_delta = looking_down ? -event.direction : event.direction;
            int event_level = event.level;
            const hero_bullet_level& level_data = levels_data[event_level];
            bn::sprite_builder builder(bn::sprite_items::hero_bullets.shape_size(), _tiles[event_level], _palette);
            builder.set_position(bullet_position);
            builder.set_horizontal_flip(looking_down);
            builder.set_vertical_flip(looking_down);
            builder.set_z_order(constants::hero_bullets_z_order);
            builder.set_camera(camera);
            bullets->push_front({ bn::sprite_move_by_action(builder.release_build(), bullet_delta), &level_data });
            hero.show_shoot(level_data.color);

            if(event.play_sound)
            {
                bn::fixed panning = bn::clamp(bullet_position.x() / constants::play_width, bn::fixed(0), bn::fixed(1));
                level_data.sound_item.play_with_priority(constants::hero_bullets_sound_priority, 0.35, 1, panning);
            }

            return;
        }
    }
}

}
