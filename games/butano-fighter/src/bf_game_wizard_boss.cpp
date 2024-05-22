/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_wizard_boss.h"

#include "bn_colors.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_wizard.h"
#include "bn_sprite_items_hero_death.h"
#include "bn_sprite_items_wizard_aura.h"
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
    constexpr int state_0_1_life = 600;   // 21 seconds
    constexpr int state_2_3_life = 400;   // 20 seconds
    constexpr int state_4_5_life = 500;   // 18 seconds
    constexpr int total_life = state_0_1_life + state_2_3_life + state_4_5_life;

    constexpr int limit_x = 38;
    constexpr int limit_y = 46;
    constexpr bn::fixed rotate_speed = 0.5;

    void _add_wizard_sprite(const bn::camera_ptr& camera, bn::ivector<bn::sprite_ptr>& sprites)
    {
        bn::sprite_builder builder(bn::sprite_items::wizard);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_camera(camera);
        sprites.push_back(builder.release_build());
    }

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

wizard_boss::wizard_boss(const bn::fixed_point& hero_position, const bn::sprite_palette_ptr& damage_palette,
                         const bn::camera_ptr& camera) :
    boss(total_life, 600, _wizard_rects, damage_palette),
    _wizard_position(0, -160),
    _palette(bn::sprite_items::wizard.palette_item().create_palette())
{
    _add_wizard_sprite(camera, _sprites);

    bn::sprite_builder builder(bn::sprite_items::wizard_aura);
    builder.set_blending_enabled(true);
    builder.set_z_order(constants::enemy_explosions_z_order);
    builder.set_camera(camera);
    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                bn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, bn::sprite_items::wizard_aura.tiles_item(), 0, 0, 4, 8, 12));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                bn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, bn::sprite_items::wizard_aura.tiles_item(), 0, 1, 5, 9, 13));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                bn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, bn::sprite_items::wizard_aura.tiles_item(), 0, 2, 6, 10, 14));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                bn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, bn::sprite_items::wizard_aura.tiles_item(), 0, 3, 7, 11, 15));

    _wizard_rects.emplace_back(bn::fixed_point(), bn::fixed_size(46, 62));
    _update_sprites(hero_position);
    _update_rects();
}

void wizard_boss::_update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                const bn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    bn::fixed x = _wizard_position.x();
    bn::fixed y = _wizard_position.y();

    switch(_state_index)
    {

    case 0:
        _wizard_position.set_y(y + constants::background_speed);

        if(y >= -limit_y)
        {
            _movement_index = 2;
            _movement_counter = 1;
            ++_state_index;
        }
        break;

    case 1:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
                ++_movement_index;
                _movement_counter = 6 * 8;

                _delta_position.set_x(-0.75);
                _delta_position.set_y(-(_wizard_position.y() + 16) / _movement_counter);
                _animate_actions.clear();
                _animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites[0], 8,
                                           bn::sprite_items::wizard.tiles_item(), 8, 9, 10, 11, 12, 13));
                break;

            case 1:
                ++_movement_index;
                _movement_counter = 120;

                _animate_actions.clear();
                _sprites[0].set_tiles(bn::sprite_items::wizard.tiles_item(), 14);
                break;

            case 2:
                _movement_index = 0;
                _movement_counter = 260;

                _target_x = limit_x;
                _animate_actions.clear();
                _animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites[0], 6,
                                           bn::sprite_items::wizard.tiles_item(), 1, 4, 5, 1, 6, 7));
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            if(*_target_x > 0)
            {
                if(x <= *_target_x)
                {
                    _wizard_position.set_x(x + 0.75);
                }
                else
                {
                    _target_x = -limit_x;
                }
            }
            else
            {
                if(x >= *_target_x)
                {
                    _wizard_position.set_x(x - 0.75);
                }
                else
                {
                    _target_x = limit_x;
                }
            }

            if(*_target_x > 0)
            {
                if(_movement_counter == 1 && _wizard_position.x() != -30)
                {
                    ++_movement_counter;
                }
            }
            else
            {
                if(_movement_counter == 1 && _wizard_position.x() != 30)
                {
                    ++_movement_counter;
                }
            }
            break;

        case 1:
            _wizard_position += _delta_position;
            break;

        case 2:
            if(y > -limit_y)
            {
                _wizard_position.set_y(bn::max(y - constants::background_speed, bn::fixed(-limit_y)));
            }
            else
            {
                _movement_counter = 1;
            }
            break;

        default:
            BN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 2:
        --_movement_counter;

        if(! _movement_counter)
        {
            ++_state_index;
            _movement_index = 2;
            _movement_counter = 1;

            if(_wizard_position.x() > 0)
            {
                _target_x = -limit_x;
            }
            else
            {
                _target_x = limit_x;
            }
        }
        break;

    case 3:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
                _movement_index = 1;
                _movement_counter = 6 * 3;

                if(_wizard_position.x() > 0)
                {
                    _target_x = -limit_x;
                }
                else
                {
                    _target_x = limit_x;
                }

                _delta_position = (bn::fixed_point(*_target_x, limit_y) - _wizard_position) / 240;
                _animate_actions.clear();
                _animate_actions.push_back(bn::create_sprite_animate_action_once(_sprites[0], 3,
                                           bn::sprite_items::wizard.tiles_item(), 18, 18, 18, 19, 19, 19));
                break;

            case 1:
                _movement_index = 2;
                _movement_counter = 120;

                _add_wizard_sprite(camera, _sprites);
                _animate_actions.clear();
                _animate_actions.push_back(bn::create_sprite_animate_action_once(_sprites[0], 4,
                                           bn::sprite_items::wizard.tiles_item(), 20, 20, 22, 22, 24, 24));
                _animate_actions.push_back(bn::create_sprite_animate_action_once(_sprites[1], 4,
                                           bn::sprite_items::wizard.tiles_item(), 21, 21, 23, 23, 25, 25));
                break;

            case 2:
                _movement_index = 0;
                _movement_counter = (bn::abs((*_target_x - _wizard_position.x())) / bn::fixed(0.75)).right_shift_integer();
                _movement_counter = bn::max(_movement_counter, 1);

                _delta_position = (bn::fixed_point(*_target_x, -limit_y) - _wizard_position) / _movement_counter;
                _sprites.shrink(1);
                _animate_actions.clear();
                _animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites[0], 6,
                                           bn::sprite_items::wizard.tiles_item(), 1, 4, 5, 1, 6, 7));
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            _wizard_position += _delta_position;
            break;

        case 1:
        case 2:
            _wizard_position.set_y(_wizard_position.y() + constants::background_speed);
            break;

        default:
            BN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 4:
        _wizard_position += _delta_position;
        --_movement_counter;

        if(! _movement_counter)
        {
            ++_state_index;
            _movement_counter = 1;
            _bullets_index = 0;
            _bullets_counter = 1;
            _delta_position = bn::fixed_point(rotate_speed, -rotate_speed);
            _vibrate = false;
        }
        else if(_movement_counter % 4 == 0)
        {
            _vibrate = ! _vibrate;
        }
        break;

    case 5:
        --_movement_counter;

        if(! _movement_counter)
        {
            _movement_counter = 4;
            _vibrate = ! _vibrate;
        }

        if(x <= -constants::play_width + 16)
        {
            _delta_position.set_x(rotate_speed);
            _target_x = constants::play_width;
        }
        else if(x >= constants::play_width - 16)
        {
            _delta_position.set_x(-rotate_speed);
            _target_x = -constants::play_width;
        }

        if(y <= -constants::play_height - 6)
        {
            _delta_position.set_y(rotate_speed);
        }
        else if(y >= constants::play_height - 12)
        {
            _delta_position.set_y(-rotate_speed);
        }

        _wizard_position += _delta_position;
        break;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    _update_sprites(hero_position);
    _update_explosions();
    _update_rects();

    if(! hero_bomb.active())
    {
        _update_bullets(hero_position, camera, enemy_bullets);
    }
}

bool wizard_boss::_update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera, background&,
                               rumble_manager& rumble_manager)
{
    bool done = false;

    if(! _aura_sprites.empty())
    {
        _movement_counter = 1;
        _delta_position.set_x((0 - _wizard_position.x()) / 240);
        _delta_position.set_y((0 - _wizard_position.y()) / 240);

        _aura_sprites.clear();
        _animate_actions.clear();
        _aura_sprite_animate_actions.clear();
        _add_wizard_sprite(camera, _sprites);
        _sprites[0].set_tiles(bn::sprite_items::wizard.tiles_item(), 26);
        _sprites[1].set_tiles(bn::sprite_items::wizard.tiles_item(), 27);

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _palette.set_fade(bn::colors::red, 0);
        _palette_action.emplace(_palette, 15, bn::fixed(0.6));
    }

    _wizard_position += _delta_position;
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

                bn::fixed x = int(_random.get() % 48) - 24 + _wizard_position.x();
                bn::fixed y = int(_random.get() % 48) - 24 + _wizard_position.y();
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
        _update_sprites(hero_position);
    }

    _update_explosions();
    return done;
}

void wizard_boss::_show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera)
{
    bn::fixed x = _wizard_position.x();
    bn::fixed y = _wizard_position.y();
    int current_life = life();

    switch(_state_index)
    {

    case 0:
        break;

    case 1:
        if(current_life < total_life - state_0_1_life)
        {
            ++_state_index;
            _movement_index = 0;
            _movement_counter = 120;
            _bullets_index = 0;
            _bullets_counter = 1;
            _target_x.reset();

            _animate_actions.clear();
            _animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites[0], 6,
                                       bn::sprite_items::wizard.tiles_item(), 1, 4, 5, 1, 6, 7));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 24, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16, camera));
            _palette.set_fade(bn::colors::red, 0);
            _palette_action.emplace(_palette, 25, bn::fixed(0.4));
            bn::sound_items::explosion_3.play();
        }
        break;

    case 2:
        break;

    case 3:
        if(current_life < total_life - state_0_1_life - state_2_3_life)
        {
            ++_state_index;
            _movement_index = 0;
            _movement_counter = 128;
            _bullets_index = 0;
            _bullets_counter = 80;
            _delta_position = (bn::fixed_point(0, -32) - _wizard_position) / _movement_counter;
            _target_x = constants::play_width;
            _wizard_rects.clear();
            _wizard_rects.emplace_back(bn::fixed_point(), bn::fixed_size(50, 38));

            _sprites.shrink(1);
            _animate_actions.clear();
            _animate_actions.push_back(bn::create_sprite_animate_action_forever(_sprites[0], 2,
                                       bn::sprite_items::wizard.tiles_item(), 15, 16, 17, 15, 16, 17));

            _mini_explosions.push_back(_create_mini_explosion(x, y, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 24, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16, camera));
            _palette.set_fade(bn::colors::red, 0);
            _palette_action.emplace(_palette, 20, bn::fixed(0.5));
            bn::sound_items::explosion_1.play();
            bn::sound_items::explosion_3.play();
        }
        break;

    case 4:
    case 5:
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

void wizard_boss::_hide_damage_palette()
{
    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(_palette);
    }
}

bool wizard_boss::_hero_should_look_down_impl(const bn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_sprites.empty())
    {
        return false;
    }

    bn::fixed y = _wizard_position.y();

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

void wizard_boss::_shoot_bullet(enemy_bullet_type bullet_type, const bn::fixed_point& delta_position,
                                const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                enemy_bullets& enemy_bullets) const
{
    enemy_bullets.add_bullet(hero_position, _wizard_position, enemy_bullet_event(bullet_type, delta_position, 1),
                             camera);
}

void wizard_boss::_shoot_random_bullet(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                       enemy_bullets& enemy_bullets)
{
    if(bn::abs(_wizard_position.x() - hero_position.x()) < 48 &&
            bn::abs(_wizard_position.y() - hero_position.y()) < 48)
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

void wizard_boss::_update_sprites(const bn::fixed_point& hero_position)
{
    bn::fixed target_x = _target_x.value_or(hero_position.x());
    bool flip = target_x < _wizard_position.x();
    bn::fixed y_inc = _vibrate ? 1 : 0;

    if(_sprites.size() == 2)
    {
        if(life())
        {
            bn::fixed x1_inc = flip ? 32 : -32;
            bn::fixed x2_inc = flip ? -32 : 32;
            _sprites[0].set_position(_wizard_position + bn::fixed_point(x1_inc, y_inc));
            _sprites[1].set_position(_wizard_position + bn::fixed_point(x2_inc, y_inc));
        }
        else
        {
            _sprites[0].set_position(_wizard_position + bn::fixed_point(0, y_inc - 32));
            _sprites[1].set_position(_wizard_position + bn::fixed_point(0, y_inc + 32));
        }
    }
    else
    {
        _sprites[0].set_position(_wizard_position + bn::fixed_point(0, y_inc));
    }

    for(bn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_horizontal_flip(flip);
    }

    for(bn::sprite_animate_action<6>& animate_action : _animate_actions)
    {
        if(! animate_action.done())
        {
            animate_action.update();
        }
    }

    if(! _aura_sprites.empty())
    {
        bool aura_visible = ! _aura_sprites[0].visible() && ! _death_flash();
        _aura_sprites[0].set_visible(aura_visible);
        _aura_sprites[0].set_position(_wizard_position + bn::fixed_point(-32, -32 - 4));
        _aura_sprites[1].set_visible(aura_visible);
        _aura_sprites[1].set_position(_wizard_position + bn::fixed_point(32, -32 - 4));
        _aura_sprites[2].set_visible(aura_visible);
        _aura_sprites[2].set_position(_wizard_position + bn::fixed_point(-32, 32 - 4));
        _aura_sprites[3].set_visible(aura_visible);
        _aura_sprites[3].set_position(_wizard_position + bn::fixed_point(32, 32 - 4));

        for(bn::sprite_animate_action<5>& aura_sprite_animate_action : _aura_sprite_animate_actions)
        {
            aura_sprite_animate_action.update();
        }
    }

    if(_palette_action)
    {
        _palette_action->update();
    }
}

void wizard_boss::_update_rects()
{
    _wizard_rects[0].set_position(_wizard_position);
}

void wizard_boss::_update_bullets(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                  enemy_bullets& enemy_bullets)
{
    --_bullets_counter;

    if(! _bullets_counter)
    {
        _bullets_counter = 1;

        switch(_state_index)
        {

        case 0:
            break;

        case 1:
            switch(_movement_index)
            {

            case 0:
                if(_movement_counter > 20)
                {
                    _bullets_counter = 50;

                    switch(_bullets_index)
                    {

                    case 0:
                        _shoot_bullet(enemy_bullet_type::BIG, direction_vector(0, 1, 0.9), hero_position,
                                      camera, enemy_bullets);
                        _bullets_index = 1;
                        break;

                    case 1:
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _bullets_index = 2;
                        break;

                    case 2:
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.25, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.25, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _bullets_index = 3;
                        break;

                    case 3:
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position,
                                      camera, enemy_bullets);
                        _bullets_index = 0;
                        break;

                    default:
                        BN_ERROR("Invalid bullets index: ", _bullets_index);
                        break;
                    }
                }
                break;

            case 1:
                if(_movement_counter == 1)
                {
                    _shoot_bullet(enemy_bullet_type::BIG, direction_vector(0, 1, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(1, 1, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::BIG, direction_vector(1, 0, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(1, -1, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::BIG, direction_vector(0, -1, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-1, -1, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::BIG, direction_vector(-1, 0, 0.9), hero_position, camera,
                                  enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-1, 1, 0.9), hero_position, camera,
                                  enemy_bullets);
                }
                break;

            case 2:
                _bullets_counter = 45;
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
            break;

        case 2:
            break;

        case 3:
            switch(_movement_index)
            {

            case 0:
                if(_movement_counter > 30 && _wizard_position.y() <= 20 && _wizard_position.y() >= 1 - limit_y)
                {
                    _bullets_counter = 20;
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0, 1, 1), hero_position,
                                  camera, enemy_bullets);
                }
                break;

            case 1:
                if(_movement_counter == 1)
                {
                    _bullets_counter = 20;
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.225, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.225, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.45, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.45, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.675, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                    _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.675, 1, 0.85), hero_position,
                                  camera, enemy_bullets);
                }
                break;

            case 2:
                break;

            default:
                BN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
            break;

        case 4:
            break;

        case 5:
            _bullets_counter = 20;
            _shoot_random_bullet(hero_position, camera, enemy_bullets);
            break;

        default:
            BN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void wizard_boss::_update_explosions()
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
