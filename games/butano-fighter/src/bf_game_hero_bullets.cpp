#include "bf_game_hero_bullets.h"

#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_sprite_builder.h"
#include "btn_hero_bullets_sprite_item.h"
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
    if(btn::keypad::held(btn::keypad::button_type::B))
    {
        _b_held_counter = 10;
    }
    else if(_b_held_counter)
    {
        --_b_held_counter;
    }

    _remove_bullets(hero, enemies, objects);
    hero.set_is_shooting(_b_held_counter);

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
    int bullets_count = _bullets.size();

    for(int index = 0; index < bullets_count; )
    {
        bullet& bullet = _bullets[index];
        btn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
        const btn::fixed_point& position = sprite_move_action.sprite().position();
        const hero_bullet_level& level_data = *bullet.level_data;

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height ||
                enemies.check_hero_bullet({ btn::fixed_rect(position, level_data.dimensions),
                                          level_data.damage, hero, objects }))
        {
            if(index < bullets_count - 1)
            {
                btn::swap(bullet, _bullets[bullets_count - 1]);
            }

            --bullets_count;
        }
        else
        {
            sprite_move_action.update();
            ++index;
        }
    }

    _bullets.shrink(bullets_count);
}

void hero_bullets::_add_bullets(hero& hero)
{
    btn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();
    int counter = _event_counter;

    for(const hero_bullet_event& event : levels_data[hero.level()].events)
    {
        if(counter == event.frame)
        {
            BTN_ASSERT(! _bullets.full(), "No more space for sprite bullets");

            int event_level = event.level;
            const hero_bullet_level& level_data = levels_data[event_level];
            btn::sprite_builder builder(btn::sprite_items::hero_bullets.shape_size(), _tiles[event_level], _palette);
            builder.set_position(hero.weapon_position());
            builder.set_z_order(constants::hero_bullets_z_order);
            _bullets.push_back({ btn::sprite_move_by_action(builder.release_build(), event.direction), &level_data });
            hero.show_shoot(level_data.color);

            if(event.play_sound)
            {
                btn::sound::play(level_data.sound_item, 0.35);
            }

            break;
        }
    }
}

}
