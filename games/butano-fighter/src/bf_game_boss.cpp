#include "bf_game_boss.h"

#include "btn_colors.h"
#include "btn_sound_items.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_palettes.h"
#include "bf_game_hero.h"
#include "bf_game_objects.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_tank_boss.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_check_hero_bullet_data.h"

namespace bf::game
{

namespace
{
    constexpr const int damage_frames = 12;
}

btn::unique_ptr<boss> boss::create(type type, const btn::fixed_point& hero_position,
                                   const btn::sprite_palette_ptr& damage_palette)
{
    btn::unique_ptr<boss> result;

    switch(type)
    {

    case type::TANK:
        result.reset(new tank_boss(hero_position, damage_palette));
        break;

    default:
        BTN_ERROR("Invalid type: ", int(type));
        break;
    }

    return result;
}

bool boss::check_hero(const btn::fixed_rect& hero_rect) const
{
    if(_life)
    {
        for(const btn::fixed_rect& rect : _rects)
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
        const btn::fixed_rect& bullet_rect = data.bullet_rect;

        for(const btn::fixed_rect& rect : _rects)
        {
            if(rect.intersects(bullet_rect))
            {
                if(! _damage_palette_counter)
                {
                    _damage_palette_counter = damage_frames;
                    _show_damage_palette(_damage_palette);
                }

                if(! _hero_bomb_active)
                {
                    _life -= data.bullet_damage;

                    if(_life <= 0)
                    {
                        _life = 0;

                        [[maybe_unused]] bool level_up = data.hero_ref.add_experience(_experience);
                        btn::bg_palettes::set_fade(btn::colors::white, 1);
                        btn::sprite_palettes::set_fade(btn::colors::black, 1);
                        btn::sound_items::glass_breaking_2.play();
                        btn::music::stop();
                        _death_flash_counter = 50;
                    }
                }

                return true;
            }
        }
    }

    return false;
}

void boss::update(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb, enemy_bullets& enemy_bullets,
                  objects& objects)
{
    if(_life)
    {
        _hero_bomb_active = hero_bomb.active();

        if(_ignore_hero_bullet_counter)
        {
            --_ignore_hero_bullet_counter;
        }

        _update_alive(hero_position, enemy_bullets);
    }
    else
    {
        if(_death_flash_counter)
        {
            enemy_bullets.clear();
            --_death_flash_counter;

            if(! _death_flash_counter)
            {
                btn::bg_palettes::set_fade_intensity(0);
                btn::sprite_palettes::set_fade_intensity(0);
                objects.spawn_hero_bomb_without_sound(_position());
            }
        }

        if(_update_dead(hero_position))
        {
            _done = true;
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

boss::boss(int life, int experience, const btn::ivector<btn::fixed_rect>& rects,
           const btn::sprite_palette_ptr& damage_palette) :
    _rects(rects),
    _damage_palette(damage_palette),
    _life(life),
    _experience(experience),
    _damage_palette_counter(0),
    _ignore_hero_bullet_counter(constants::enemies_invencible_frames),
    _death_flash_counter(0),
    _hero_bomb_active(false),
    _done(false)
{
}

}
