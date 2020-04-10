#include "bf_game_hero.h"

#include "btn_sound.h"
#include "btn_camera.h"
#include "btn_keypad.h"
#include "btn_colors.h"
#include "btn_fixed_rect.h"
#include "btn_sound_items.h"
#include "btn_hero_body_sprite_item.h"
#include "btn_hero_death_sprite_item.h"
#include "btn_hero_weapons_sprite_item.h"
#include "bf_game_enemies.h"
#include "bf_game_objects.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_background.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

namespace
{
    constexpr const int body_delta_y = 40;
    constexpr const int weapon_delta_x = 2;
    constexpr const int weapon_delta_y = -13;
    constexpr const int scale_weapon_frames = 30;
    constexpr const int scale_weapon_half_frames = scale_weapon_frames / 2;
    constexpr const btn::fixed_size dimensions(14, 14);

    btn::sprite_cached_animate_action<2> _build_body_sprite_animate_action()
    {
        btn::sprite_ptr body_sprite = btn::sprite_ptr::create(0, body_delta_y, btn::sprite_items::hero_body);
        return btn::create_sprite_cached_animate_action_forever(btn::move(body_sprite), 16,
                                                                btn::sprite_items::hero_body, 0, 2);
    }

    btn::sprite_ptr _build_weapon_sprite(int level, const btn::fixed_point& position)
    {
        return btn::sprite_ptr::create(position, btn::sprite_items::hero_weapons, level);
    }
}

hero::hero() :
    _body_sprite_animate_action(_build_body_sprite_animate_action()),
    _weapon_position(weapon_delta_x, body_delta_y + weapon_delta_y),
    _weapon_sprite(_build_weapon_sprite(_level, _weapon_position))
{
}

btn::fixed hero::next_level_experience_ratio() const
{
    btn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();

    if(_level == levels_data.size() - 1)
    {
        return 1;
    }

    int experience_to_current_level = _level == 0 ? 0 : levels_data[_level - 1].experience_to_next_level;
    int next_level_experience = levels_data[_level].experience_to_next_level - experience_to_current_level;
    btn::fixed experience = _experience - experience_to_current_level;
    return btn::min(experience / next_level_experience, btn::fixed(1));
}

bool hero::add_experience(int experience)
{
    btn::span<const hero_bullet_level> levels_data = hero_bullet_level::all_levels();
    int level = _level;
    _experience += experience;

    if(level == levels_data.size() - 1)
    {
        return false;
    }

    return _experience >= levels_data[level].experience_to_next_level;
}

void hero::show_shoot(btn::color fade_color)
{
    _show_shoot_counter = 5;

    btn::sprite_palette_ptr body_palette = _body_sprite_animate_action.sprite().palette();
    body_palette.set_fade(fade_color, 0.5);
    _body_palette_fade_action.emplace(btn::move(body_palette), _show_shoot_counter, 0);
}

bool hero::add_bomb()
{
    if(_bombs_count < constants::max_hero_bombs)
    {
        return false;
    }

    ++_bombs_count;
    return true;
}

bool hero::throw_bomb()
{
    if(! _bombs_count)
    {
        return false;
    }

    --_bombs_count;
    return true;
}

void hero::update(const hero_bomb& hero_bomb, const enemies& enemies, const enemy_bullets& enemy_bullets,
                  objects& objects, background& background)
{
    if(alive())
    {
        btn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();
        btn::fixed_point old_body_position = body_sprite.position();
        btn::fixed_point new_body_position = _move(old_body_position, body_sprite);
        btn::fixed_rect new_body_rect(new_body_position, dimensions);
        _animate_alive(old_body_position, new_body_position);

        if(objects.check_hero_weapon(new_body_rect))
        {
            ++_level;
            _scale_weapon_counter = scale_weapon_frames;
            _weapon_sprite.set_item(btn::sprite_items::hero_weapons, _level);
            _weapon_sprite.set_scale(2);

            btn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
            weapon_palette.set_fade(btn::colors::yellow, 0.5);
        }

        bool max_bombs_count = _bombs_count == constants::max_hero_bombs;

        if(objects.check_hero_bomb(new_body_rect, max_bombs_count))
        {
            if(max_bombs_count)
            {
                //
            }
            else
            {
                ++_bombs_count;
            }
        }

        if(int experience = objects.check_gem(new_body_rect, _level))
        {
            if(add_experience(experience))
            {
                objects.spawn_hero_weapon(btn::fixed_point(0, -constants::view_height), _level + 1);
            }
        }

        if(! hero_bomb.active())
        {
            if(enemies.check_hero(new_body_rect) || enemy_bullets.check_hero(new_body_rect))
            {
                ++_death_counter;
            }
        }
    }
    else
    {
        _animate_dead(background);
    }
}

btn::fixed_point hero::_move(const btn::fixed_point& body_position, btn::sprite_ptr& body_sprite)
{
    btn::fixed_point new_body_position = body_position;
    btn::fixed speed = _shooting ? 1 : 2;

    if(btn::keypad::held(btn::keypad::button_type::LEFT))
    {
        btn::fixed sprite_x = btn::max(body_position.x() - speed, btn::fixed(-constants::play_width));
        body_sprite.set_x(sprite_x);
        new_body_position.set_x(sprite_x);

        if(sprite_x < constants::camera_width)
        {
            btn::camera::set_x(btn::max(btn::camera::x() - speed, btn::fixed(-constants::camera_width)));
        }
    }
    else if(btn::keypad::held(btn::keypad::button_type::RIGHT))
    {
        btn::fixed sprite_x = btn::min(body_position.x() + speed, btn::fixed(constants::play_width));
        body_sprite.set_x(sprite_x);
        new_body_position.set_x(sprite_x);

        if(sprite_x > -constants::camera_width)
        {
            btn::camera::set_x(btn::min(btn::camera::x() + speed, btn::fixed(constants::camera_width)));
        }
    }

    if(btn::keypad::held(btn::keypad::button_type::UP))
    {
        btn::fixed sprite_y = btn::max(body_position.y() - speed, btn::fixed(-constants::play_height));
        body_sprite.set_y(sprite_y);
        new_body_position.set_y(sprite_y);
    }
    else if(btn::keypad::held(btn::keypad::button_type::DOWN))
    {
        btn::fixed sprite_y = btn::min(body_position.y() + speed, btn::fixed(constants::play_height));
        body_sprite.set_y(sprite_y);
        new_body_position.set_y(sprite_y);
    }

    return new_body_position;
}

void hero::_animate_alive(const btn::fixed_point& old_body_position, const btn::fixed_point& new_body_position)
{
    int shoot_shift_y;

    if(_show_shoot_counter)
    {
        shoot_shift_y = -1;
        --_show_shoot_counter;
        _body_palette_fade_action->update();
    }
    else
    {
        shoot_shift_y = 0;
    }

    _weapon_position = new_body_position + btn::fixed_point(weapon_delta_x, weapon_delta_y);
    _weapon_sprite.set_position(_weapon_position + btn::fixed_point(0, shoot_shift_y));

    if(! _shooting && old_body_position != new_body_position)
    {
        _body_sprite_animate_action.update();
    }

    _body_sprite_animate_action.update();

    if(_scale_weapon_counter)
    {
        btn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
        --_scale_weapon_counter;

        if(_scale_weapon_counter)
        {
            if(_scale_weapon_counter <= scale_weapon_half_frames)
            {
                _weapon_sprite.set_scale(1 + (btn::fixed(_scale_weapon_counter) / scale_weapon_half_frames));

                if(_scale_weapon_counter == scale_weapon_half_frames)
                {
                    weapon_palette.set_fade_intensity(0);
                }
            }
        }
        else
        {
            _weapon_sprite.set_scale(1);
        }
    }
}

void hero::_animate_dead(background& background)
{
    btn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();

    if(_death_counter == 1)
    {
        btn::sprite_palette_ptr body_palette = body_sprite.palette();
        body_palette.set_fade(btn::colors::yellow, 0.75);
        _body_palette_fade_action.emplace(btn::move(body_palette), 30, 0);
        _body_rotate_action.emplace(body_sprite, 0.5);

        btn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
        weapon_palette.set_fade(btn::colors::yellow, 0.75);
        _weapon_palette_fade_action.emplace(btn::move(weapon_palette), 30, 0);
        _weapon_sprite.set_scale(1);
        _weapon_move_action.emplace(_weapon_sprite, 70, _weapon_sprite.position() + btn::fixed_point(10, -10));
        _weapon_rotate_action.emplace(_weapon_sprite, -10);

        _music_volume_action.emplace(50, 0);
        background.show_hero_dying();
        btn::sound::play(btn::sound_items::boss_shoot);
    }
    else if(_death_counter == 70)
    {
        const btn::fixed_point& body_position = body_sprite.position();
        btn::fixed body_x = body_position.x() + 2;
        btn::fixed body_y = body_position.y() + 4;
        _death_sprites.push_back(btn::sprite_ptr::create(body_x - 32, body_y - 32, btn::sprite_items::hero_death, 0));
        _death_sprites.push_back(btn::sprite_ptr::create(body_x + 32, body_y - 32, btn::sprite_items::hero_death, 1));
        _death_sprites.push_back(btn::sprite_ptr::create(body_x - 32, body_y + 32, btn::sprite_items::hero_death, 2));
        _death_sprites.push_back(btn::sprite_ptr::create(body_x + 32, body_y + 32, btn::sprite_items::hero_death, 3));

        _weapon_move_action.emplace(_weapon_sprite, 70, _weapon_sprite.position() + btn::fixed_point(5, -5));
        _weapon_rotate_action.emplace(_weapon_sprite, -5);

        background.show_hero_dead();
        btn::sound::play(btn::sound_items::death);
    }
    else if(_death_counter > 70 && (_death_counter - 70) % 4 == 0)
    {
        int animation_index = (_death_counter - 70) / 4;

        if(animation_index < 6)
        {
            if(animation_index == 3)
            {
                body_sprite.set_visible(false);
            }

            for(int index = 0; index < 4; ++index)
            {
                _death_sprites[index].set_tiles(btn::sprite_items::hero_death, (animation_index * 4) + index);
            }
        }
        else
        {
            _death_sprites.clear();
        }
    }

    ++_death_counter;

    if(body_sprite.visible())
    {
        _body_rotate_action->update();
        body_sprite.set_y(body_sprite.y() + 0.5);

        if(_death_counter % 8 == 0)
        {
            body_sprite.set_x(body_sprite.x() + 2);
        }
        else if(_death_counter % 4 == 0)
        {
            body_sprite.set_x(body_sprite.x() - 2);
        }
    }

    if(_weapon_move_action)
    {
        _weapon_move_action->update();
        _weapon_rotate_action->update();

        if(_weapon_move_action->done())
        {
            _weapon_move_action.reset();
            _weapon_rotate_action.reset();
        }
    }
    else
    {
        btn::fixed weapon_y = _weapon_sprite.y();

        if(weapon_y < constants::view_height)
        {
            _weapon_sprite.set_y(weapon_y + 0.5);
        }
    }

    if(_body_palette_fade_action)
    {
        _body_palette_fade_action->update();
        _weapon_palette_fade_action->update();

        if(_body_palette_fade_action->done())
        {
            _body_palette_fade_action.reset();
            _weapon_palette_fade_action.reset();
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

}

}
