/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_boss.h"

#include "bn_colors.h"
#include "bn_green_swap.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"
#include "bf_game_hero.h"
#include "bf_game_objects.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_tank_boss.h"
#include "bf_game_scoreboard.h"
#include "bf_game_background.h"
#include "bf_game_butano_boss.h"
#include "bf_game_wizard_boss.h"
#include "bf_game_gigabat_boss.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_check_hero_bullet_data.h"


namespace bf::game
{

namespace
{
    constexpr int damage_frames = 12;
    constexpr int death_flash_frames = 50;
}

bn::unique_ptr<boss> boss::create(type type, const bn::fixed_point& hero_position,
                                   const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera)
{
    bn::unique_ptr<boss> result;

    switch(type)
    {

    case type::TANK:
        result.reset(new tank_boss(hero_position, damage_palette, camera));
        break;

    case type::GIGABAT:
        result.reset(new gigabat_boss(hero_position, damage_palette, camera));
        break;

    case type::WIZARD:
        result.reset(new wizard_boss(hero_position, damage_palette, camera));
        break;

    case type::BUTANO:
        result.reset(new butano_boss(damage_palette, camera));
        break;

    default:
        BN_ERROR("Invalid type: ", int(type));
        break;
    }

    return result;
}

void boss::play_music() const
{
    bn::music_items::minor_boss_r.play(0.4);
}

bool boss::check_hero(const bn::fixed_rect& hero_rect) const
{
    if(_life)
    {
        for(const bn::fixed_rect& rect : _rects)
        {
            if(rect.intersects(hero_rect))
            {
                return true;
            }
        }
    }

    return false;
}

bool boss::check_hero_bullet(const check_hero_bullet_data& data)
{
    if(_life && ! _ignore_hero_bullet_counter)
    {
        const bn::fixed_rect& bullet_rect = data.bullet_rect;

        for(const bn::fixed_rect& rect : _rects)
        {
            if(rect.intersects(bullet_rect))
            {
                if(! _damage_palette_counter)
                {
                    _damage_palette_counter = damage_frames;
                    _show_damage_palette(_damage_palette, data.camera_ref);
                }

                if(! _hero_bomb_active)
                {
                    _life -= data.bullet_damage;

                    if(_life <= 0)
                    {
                        _life = 0;
                        _death_flash_counter = death_flash_frames;
                    }
                }

                return true;
            }
        }
    }

    return false;
}

void boss::update(const hero_bomb& hero_bomb, const bn::camera_ptr& camera, hero& hero, enemy_bullets& enemy_bullets,
                  objects& objects, scoreboard& scoreboard, background& background, rumble_manager& rumble_manager)
{
    const bn::fixed_point& hero_position = hero.body_position();

    if(_life)
    {
        _hero_bomb_active = hero_bomb.active();

        if(_ignore_hero_bullet_counter)
        {
            --_ignore_hero_bullet_counter;
        }

        _update_alive(hero_position, hero_bomb, camera, enemy_bullets);
    }
    else
    {
        if(_death_flash_counter)
        {
            if(_death_flash_counter == death_flash_frames)
            {
                _transparent_color = bn::bg_palettes::transparent_color();
                bn::bg_palettes::set_transparent_color(bn::colors::white);
                bn::sprite_palettes::set_fade(bn::colors::black, 1);
                bn::green_swap::set_enabled(true);
                bn::sound_items::glass_breaking_2.play();
                scoreboard.set_visible(false);
                background.set_visible(false);
                enemy_bullets.clear();
                bn::music::stop();
            }

            --_death_flash_counter;

            if(! _death_flash_counter)
            {
                bn::fixed_point enemy_position = _position();

                if(_throw_bomb())
                {
                    objects.spawn_hero_bomb_without_sound(enemy_position, camera);
                }

                if(hero.add_experience(_experience))
                {
                    objects.spawn_hero_weapon_without_sound(enemy_position, hero.level() + 1, camera);
                }

                if(_transparent_color)
                {
                    bn::bg_palettes::set_transparent_color(*_transparent_color);
                }
                else
                {
                    bn::bg_palettes::remove_transparent_color();
                }

                bn::sprite_palettes::set_fade_intensity(0);
                bn::green_swap::set_enabled(false);
                scoreboard.set_visible(true);
                background.set_visible(true);
            }
        }

        if(_update_dead(hero_position, camera, background, rumble_manager))
        {
            if(_dead_counter)
            {
               --_dead_counter;
            }
        }
    }

    if(_damage_palette_counter)
    {
        --_damage_palette_counter;

        if(_damage_palette_counter == damage_frames / 2)
        {
            _hide_damage_palette();
        }
    }
}

boss::boss(int life, int experience, const bn::ivector<bn::fixed_rect>& rects,
           const bn::sprite_palette_ptr& damage_palette) :
    _rects(rects),
    _damage_palette(damage_palette),
    _life(life),
    _experience(experience),
    _damage_palette_counter(0),
    _ignore_hero_bullet_counter(constants::enemies_invencible_frames),
    _death_flash_counter(0),
    _dead_counter(60),
    _hero_bomb_active(false)
{
}

bool boss::_throw_bomb() const
{
    return true;
}

}
