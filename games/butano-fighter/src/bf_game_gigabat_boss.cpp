/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_gigabat_boss.h"

#include "bn_colors.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_gigabat.h"
#include "bn_sprite_items_hero_death.h"
#include "bn_sprite_items_gigabat_shadow.h"
#include "bn_sprite_items_mini_explosion_2.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_rumble_manager.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr int state_0_1_life = 350;   // 16 seconds
    constexpr int state_2_life = 440;     // 24 seconds
    constexpr int state_3_4_life = 350;   // 20 seconds
    constexpr int total_life = state_0_1_life + state_2_life + state_3_4_life;

    constexpr int mouth_y = 10;
    constexpr bn::fixed rotate_speed = 0.35;

    [[nodiscard]] bn::sprite_animate_action<5> _create_mini_explosion(bn::fixed x, bn::fixed y,
                                                                       const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::mini_explosion_2);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        builder.set_camera(camera);
        return bn::create_sprite_animate_action_once(
                    builder.release_build(), 6, bn::sprite_items::mini_explosion_2.tiles_item(), 0, 1, 2, 3, 4);
    }
}

gigabat_boss::gigabat_boss(const bn::fixed_point& hero_position, const bn::sprite_palette_ptr& damage_palette,
                           const bn::camera_ptr& camera) :
    boss(total_life, 400, _gigabat_rects, damage_palette),
    _gigabat_position(0, -125),
    _palette(bn::sprite_items::gigabat.palette_item().create_palette())
{
    bn::sprite_builder builder(bn::sprite_items::gigabat);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _sprites.push_back(builder.release_build());

    const bn::sprite_tiles_item& tiles_item = bn::sprite_items::gigabat.tiles_item();
    _animate_actions.push_back(
                bn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 0, 2, 0, 4, 6));

    builder = bn::sprite_builder(bn::sprite_items::gigabat, 1);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _sprites.push_back(builder.release_build());
    _animate_actions.push_back(
                bn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 1, 3, 1, 5, 7));

    builder = bn::sprite_builder(bn::sprite_items::gigabat_shadow);
    builder.set_blending_enabled(true);
    builder.set_z_order(constants::footprint_z_order);
    builder.set_camera(camera);
    _shadow_sprite = builder.release_build();

    _gigabat_rects.emplace_back(bn::fixed_point(), bn::fixed_size(44, 45));
    _update_sprites(hero_position, false);
    _update_rects();
}

void gigabat_boss::_update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                 const bn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    bn::fixed x = _gigabat_position.x();
    bn::fixed y = _gigabat_position.y();

    switch(_state_index)
    {

    case 0:
        _gigabat_position.set_y(y + 0.35);

        if(y >= -50)
        {
            ++_state_index;
        }
        break;

    case 1:
        if(_movement_index == 0)
        {
            _gigabat_position.set_x(x + 0.35);

            if(_gigabat_position.x() >= 50)
            {
                _movement_index = 1;
            }
        }
        else
        {
            _gigabat_position.set_x(x - 0.35);

            if(_gigabat_position.x() <= -50)
            {
                _movement_index = 0;
            }
        }

        _movement_counter = (_movement_counter + 8) % 512;
        _gigabat_position.set_y((bn::lut_sin(_movement_counter * 4) * 5) - 50);
        break;

    case 2:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
                _sprites[0].set_tiles(bn::sprite_items::gigabat.tiles_item(), 10);
                _sprites[1].set_tiles(bn::sprite_items::gigabat.tiles_item(), 11);
                _target_x = hero_position.x();
                _movement_index = 1;
                _movement_counter = 80;
                _vibrate = false;
                _delta_position = (hero_position - _gigabat_position) / _movement_counter;
                break;

            case 1:
                _sprites[0].set_tiles(bn::sprite_items::gigabat.tiles_item(), 8);
                _sprites[1].set_tiles(bn::sprite_items::gigabat.tiles_item(), 9);
                _target_x.reset();
                _movement_index = 0;
                _movement_counter = 500;
                _vibrate = true;
                _bullets_counter = 100;
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            if(y < -56)
            {
                _gigabat_position.set_y(bn::min(y + 0.5, bn::fixed(-56)));
            }
            else if(y > -56)
            {
                _gigabat_position.set_y(bn::max(y - 0.5, bn::fixed(-56)));
            }

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 1:
            _gigabat_position += _delta_position;
            break;

        default:
            BN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 3:
        if(y < -34)
        {
            _gigabat_position.set_y(bn::min(y + 0.25, bn::fixed(-34)));
        }
        else if(y > -34)
        {
            _gigabat_position.set_y(bn::max(y - 0.25, bn::fixed(-34)));
        }

        --_movement_counter;

        if(! _movement_counter)
        {
            ++_state_index;
            _movement_index = 0;
            _movement_counter = 1;
            _bullets_index = 0;
            _bullets_counter = 1;
            _vibrate = false;
        }
        else if(_movement_counter % 4 == 0)
        {
            _vibrate = ! _vibrate;
        }
        break;

    case 4:
        --_movement_counter;

        if(! _movement_counter)
        {
            _movement_index = (_movement_index + 1) % 3;
            _movement_counter = 4;
            _vibrate = ! _vibrate;

            int base_graphics_index = 16 + (_movement_index * 4);
            _sprites[0].set_tiles(bn::sprite_items::gigabat.tiles_item(), base_graphics_index);
            _sprites[1].set_tiles(bn::sprite_items::gigabat.tiles_item(), base_graphics_index + 1);
            _sprites[2].set_tiles(bn::sprite_items::gigabat.tiles_item(), base_graphics_index + 2);
            _sprites[3].set_tiles(bn::sprite_items::gigabat.tiles_item(), base_graphics_index + 3);
        }

        if(x <= -constants::play_width + 30)
        {
            _delta_position.set_x(rotate_speed);
            _target_x = constants::play_width;
        }
        else if(x >= constants::play_width - 30)
        {
            _delta_position.set_x(-rotate_speed);
            _target_x = -constants::play_width;
        }

        if(y <= -constants::play_height + 16)
        {
            _delta_position.set_y(rotate_speed);
        }
        else if(y >= constants::play_height - 16)
        {
            _delta_position.set_y(-rotate_speed);
        }

        _gigabat_position += _delta_position;
        break;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    _update_sprites(hero_position, hero_bomb.closing());
    _update_explosions();
    _update_rects();

    if(! hero_bomb.active())
    {
        _update_bullets(hero_position, camera, enemy_bullets);
    }
}

bool gigabat_boss::_update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera, background&,
                                rumble_manager& rumble_manager)
{
    bool hide_shadow = false;
    bool done = false;

    if(_sprites.size() == 4)
    {
        _movement_counter = 1;
        _delta_position.set_x((0 - _gigabat_position.x()) / 240);
        _delta_position.set_y((0 - _gigabat_position.y()) / 240);

        _sprites[0].set_tiles(bn::sprite_items::gigabat.tiles_item(), 28);
        _sprites[1].set_tiles(bn::sprite_items::gigabat.tiles_item(), 29);
        _sprites.pop_back();
        _sprites.pop_back();

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _palette.set_fade(bn::colors::red, 0);
        _palette_action.emplace(_palette, 15, bn::fixed(0.6));
    }

    _gigabat_position += _delta_position;
    --_movement_counter;

    if(! _movement_counter)
    {
        _movement_counter = 4;
        _vibrate = ! _vibrate;
    }

    if(_death_flash())
    {
        _state_index = 0;
        _movement_index = 1;
        hide_shadow = true;
    }
    else
    {
        if(_state_index < 15)
        {
            --_movement_index;

            if(! _movement_index)
            {
                ++_state_index;
                _movement_index = 16;

                bn::fixed x = int(_random.get() % 48) - 24 + _gigabat_position.x();
                bn::fixed y = int(_random.get() % 48) - 24 + _gigabat_position.y();
                _mini_explosions.push_back(_create_mini_explosion(x, y, camera));
                bn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(bn::sprite_items::hero_death, bn::fixed_point(), 6,
                                   constants::enemy_explosions_z_order, true, camera);
                bn::sound_items::explosion_2.play();
                rumble_manager.set_enabled(true);
            }
        }
        else if(_state_index == 16)
        {
            if(_explosion)
            {
                if(! _explosion->show_target_sprite())
                {
                    _sprites.clear();
                    _shadow_sprite.reset();
                }
            }
            else
            {
                rumble_manager.set_enabled(false);
                done = true;
            }
        }
    }

    if(! _sprites.empty())
    {
        if(! hide_shadow)
        {
            _shadow_sprite->set_scale(1);
        }

        _update_sprites(hero_position, hide_shadow);
    }

    _update_explosions();
    return done;
}

void gigabat_boss::_show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera)
{
    bn::fixed x = _gigabat_position.x();
    bn::fixed y = _gigabat_position.y();
    int current_life = life();

    switch(_state_index)
    {

    case 0:
        break;

    case 1:
        if(current_life < total_life - state_0_1_life)
        {
            ++_state_index;
            _movement_index = 1;
            _movement_counter = 1;
            _bullets_index = 0;
            _bullets_counter = 80;
            _animate_actions.clear();

            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 24, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16, camera));
            _palette.set_fade(bn::colors::red, 0);
            _palette_action.emplace(_palette, 25, bn::fixed(0.4));
            bn::sound_items::explosion_3.play();
        }
        break;

    case 2:
        if(current_life < total_life - state_0_1_life - state_2_life)
        {
            ++_state_index;
            _movement_index = 0;
            _movement_counter = 180;
            _bullets_index = 0;
            _bullets_counter = 80;
            _random = bn::random();
            _delta_position = bn::fixed_point(rotate_speed, -rotate_speed);
            _target_x = constants::play_width;

            _sprites[0].set_tiles(bn::sprite_items::gigabat.tiles_item(), 12);
            _sprites[1].set_tiles(bn::sprite_items::gigabat.tiles_item(), 13);

            bn::sprite_builder builder(bn::sprite_items::gigabat, 14);
            builder.set_z_order(constants::enemies_z_order);
            builder.set_camera(camera);
            _sprites.push_back(builder.release_build());

            builder = bn::sprite_builder(bn::sprite_items::gigabat, 15);
            builder.set_z_order(constants::enemies_z_order);
            builder.set_camera(camera);
            _sprites.push_back(builder.release_build());

            _explosion.emplace(bn::sprite_items::hero_death, bn::fixed_point(x, y), 6,
                               constants::enemy_explosions_z_order, false, camera);
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 24, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16, camera));
            _palette.set_fade(bn::colors::red, 0);
            _palette_action.emplace(_palette, 20, bn::fixed(0.5));
            bn::sound_items::explosion_1.play();
            bn::sound_items::explosion_3.play();
        }
        break;

    case 3:
    case 4:
        break;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(damage_palette);
    }
}

void gigabat_boss::_hide_damage_palette()
{
    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(_palette);
    }
}

bool gigabat_boss::_hero_should_look_down_impl(const bn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_sprites.empty())
    {
        return false;
    }

    bn::fixed y = _gigabat_position.y();

    if(hero_position.y() < y - 48)
    {
        return true;
    }

    if(hero_position.y() > y + 48)
    {
        return false;
    }

    return hero_is_looking_down;
}

void gigabat_boss::_shoot_bullet(enemy_bullet_type bullet_type, const bn::fixed_point& delta_position,
                                 const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                 enemy_bullets& enemy_bullets) const
{
    bn::fixed_point mouth_position = _gigabat_position + bn::fixed_point(0, mouth_y);
    enemy_bullets.add_bullet(hero_position, mouth_position, enemy_bullet_event(bullet_type, delta_position, 1), camera);
}

void gigabat_boss::_shoot_target_random_bullet(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                               enemy_bullets& enemy_bullets)
{
    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    bn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? bn::fixed(0.9) : bn::fixed(1.0);

    if(_random.get() % 8 == 0)
    {
        bn::fixed_point mouth_position = _gigabat_position + bn::fixed_point(0, mouth_y);
        bn::fixed_point distance = hero_position - mouth_position;

        if(distance == bn::fixed_point())
        {
            distance.set_y(1);
        }

        bn::fixed_point delta_position = direction_vector(distance.x(), distance.y(), bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
    }
    else
    {
        bn::fixed bullet_x = bn::fixed::from_data(int(_random.get() % bn::fixed(2).data())) - 1;
        bn::fixed bullet_y = bn::fixed::from_data(int(_random.get() % bn::fixed(1).data()));

        if(bullet_x == 0 && bullet_y == 0)
        {
            bullet_y = 1;
        }

        bn::fixed_point delta_position = direction_vector(bullet_x, bullet_y, bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
    }
}

void gigabat_boss::_shoot_free_random_bullet(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                             enemy_bullets& enemy_bullets)
{
    if(bn::abs(_gigabat_position.x() - hero_position.x()) < 48 &&
            bn::abs(_gigabat_position.y() - hero_position.y()) < 48)
    {
        return;
    }

    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    bn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? bn::fixed(0.9) : bn::fixed(1.0);
    bn::fixed bullet_x = bn::fixed::from_data(int(_random.get() % bn::fixed(2).data())) - 1;
    bn::fixed bullet_y = bn::fixed::from_data(int(_random.get() % bn::fixed(2).data())) - 1;

    if(bullet_x == 0 && bullet_y == 0)
    {
        bullet_y = 1;
    }

    bn::fixed_point delta_position = direction_vector(bullet_x, bullet_y, bullet_speed);
    _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
}

void gigabat_boss::_update_sprites(const bn::fixed_point& hero_position, bool hide_shadow)
{
    bn::fixed target_x = _target_x.value_or(hero_position.x());
    bool flip = target_x < _gigabat_position.x();
    bn::fixed x1_inc = flip ? 32 : -32;
    bn::fixed x2_inc = flip ? -32 : 32;
    bn::fixed y_inc = _vibrate ? 1 : 0;

    if(_sprites.size() == 2)
    {
        _sprites[0].set_position(_gigabat_position + bn::fixed_point(x1_inc, y_inc));
        _sprites[1].set_position(_gigabat_position + bn::fixed_point(x2_inc, y_inc));
    }
    else
    {
        _sprites[0].set_position(_gigabat_position + bn::fixed_point(x1_inc, y_inc - 32));
        _sprites[1].set_position(_gigabat_position + bn::fixed_point(x2_inc, y_inc - 32));
        _sprites[2].set_position(_gigabat_position + bn::fixed_point(x1_inc, y_inc + 32));
        _sprites[3].set_position(_gigabat_position + bn::fixed_point(x2_inc, y_inc + 32));
    }

    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_horizontal_flip(flip);
    }

    for(bn::sprite_animate_action<5>& animate_action : _animate_actions)
    {
        animate_action.update();
    }

    if(hide_shadow)
    {
        _shadow_sprite->set_visible(false);
        _shadow_sprite->set_scale(0.01);
    }
    else
    {
        _shadow_sprite->set_visible(true);
        _shadow_sprite->set_position(_gigabat_position + bn::fixed_point(0, 40));

        bn::fixed scale = _shadow_sprite->horizontal_scale();

        if(scale < 1)
        {
            _shadow_sprite->set_scale(bn::min(scale + 0.05, bn::fixed(1)));
        }
    }

    if(_palette_action)
    {
        _palette_action->update();
    }
}

void gigabat_boss::_update_rects()
{
    _gigabat_rects[0].set_position(_gigabat_position);
}

void gigabat_boss::_update_bullets(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                   enemy_bullets& enemy_bullets)
{
    --_bullets_counter;

    if(! _bullets_counter)
    {
        switch(_state_index)
        {

        case 0:
            _bullets_counter = 1;
            break;

        case 1:
            _bullets_counter = 50;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(enemy_bullet_type::BIG, direction_vector(0, 1, 0.9), hero_position, camera,
                              enemy_bullets);
                _bullets_index = 1;
                break;

            case 1:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _bullets_index = 2;
                break;

            case 2:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.25, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.25, 1, 1), hero_position, camera,
                              enemy_bullets);
                _bullets_index = 3;
                break;

            case 3:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _bullets_index = 0;
                break;

            default:
                BN_ERROR("Invalid bullets index: ", _bullets_index);
                break;
            }
            break;

        case 2:
            _bullets_counter = 12;

            switch(_movement_index)
            {

            case 0:
                if(_movement_counter >= 80)
                {
                    _shoot_target_random_bullet(hero_position, camera, enemy_bullets);
                }
                break;

            case 1:
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
            break;

        case 3:
            _bullets_counter = 1;
            break;

        case 4:
            _bullets_counter = 20;
            _shoot_free_random_bullet(hero_position, camera, enemy_bullets);
            break;

        default:
            BN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void gigabat_boss::_update_explosions()
{
    for(auto it = _mini_explosions.begin(), end = _mini_explosions.end(); it != end; )
    {
        bn::sprite_animate_action<5>& mini_explosions = *it;
        mini_explosions.update();

        if(mini_explosions.done())
        {
            it = _mini_explosions.erase(it);
            end = _mini_explosions.end();
        }
        else
        {
            ++it;
        }
    }

    if(_explosion)
    {
        _explosion->update();

        if(_explosion->done())
        {
            _explosion.reset();
        }
    }
}

}
