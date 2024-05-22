/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_tank_boss.h"

#include "bn_math.h"
#include "bn_colors.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_tank_base.h"
#include "bn_sprite_items_tank_jelly.h"
#include "bn_sprite_items_tank_cannon.h"
#include "bn_sprite_items_tank_extras.h"
#include "bn_sprite_items_mini_explosion.h"
#include "bn_sprite_items_enemy_explosion.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_rumble_manager.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr int jelly_x = 31;
    constexpr int jelly_damage_frames = 60;
    constexpr int vibration_frames = 10;

    constexpr int state_0_1_life = 190;   // 13 seconds
    constexpr int state_2_life = 100;     // 7 seconds
    constexpr int state_3_4_life = 190;   // 30 seconds
    constexpr int state_4_5_life = 190;   // 13 seconds
    constexpr int total_life = state_0_1_life + state_2_life + state_3_4_life + state_4_5_life;

    [[nodiscard]] bn::fixed _fix_rotation_angle(bn::fixed rotation_angle)
    {
        if(rotation_angle < 0)
        {
            rotation_angle += 360;
        }
        else if(rotation_angle >= 360)
        {
            rotation_angle -= 360;
        }

        return rotation_angle;
    }

    [[nodiscard]] bn::fixed_point _cannon_end_position(bn::fixed rotation_angle, bn::fixed y)
    {
        return bn::fixed_point(bn::degrees_sin(rotation_angle) * -32, (bn::degrees_cos(rotation_angle) * -32) + y);
    }

    [[nodiscard]] bn::fixed _squared_distance(const bn::fixed_point& a, const bn::fixed_point& b)
    {
        bn::fixed distance_x = a.x() - b.x();
        bn::fixed distance_y = a.y() - b.y();
        return (distance_x * distance_x) + (distance_y * distance_y);
    }

    void _shoot_bullet(bn::fixed bullet_rotation_angle, enemy_bullet_type bullet_type,
                       bn::fixed cannon_rotation_angle, const bn::fixed_point& hero_position, bn::fixed y,
                       const bn::camera_ptr& camera, enemy_bullets& enemy_bullets)
    {
        bn::fixed_point base_position(0, y);
        bn::fixed_point cannon_position = _cannon_end_position(cannon_rotation_angle, y);
        bn::fixed_point bullet_position = _cannon_end_position(bullet_rotation_angle, y);
        bn::fixed_point distance = bullet_position - base_position;
        bn::fixed bullet_speed = bullet_type == enemy_bullet_type::HUGE ? bn::fixed(0.9) : bn::fixed(1);
        bn::fixed_point delta_position = direction_vector(distance.x(), distance.y(), bullet_speed);
        enemy_bullets.add_bullet(hero_position, cannon_position, enemy_bullet_event(bullet_type, delta_position, 1),
                                 camera);
    }

    [[nodiscard]] bn::sprite_animate_action<7> _create_mini_explosion(bn::fixed x, bn::fixed y,
                                                                       const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::mini_explosion);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        builder.set_camera(camera);
        return bn::create_sprite_animate_action_once(
                    builder.release_build(), 6, bn::sprite_items::mini_explosion.tiles_item(), 0, 1, 2, 3, 4, 5, 6);
    }
}

tank_boss::tank_boss(const bn::fixed_point& hero_position, const bn::sprite_palette_ptr& damage_palette,
                     const bn::camera_ptr& camera) :
    boss(total_life, 300, _tank_rects, damage_palette),
    _base_palette(bn::sprite_items::tank_base.palette_item().create_palette()),
    _jelly_palette(bn::sprite_items::tank_jelly.palette_item().create_palette()),
    _cannon_palette(bn::sprite_items::tank_cannon.palette_item().create_palette()),
    _y(-125),
    _y_inc(bn::fixed(1.1))
{
    bn::fixed footprint_y = 16 - (bn::display::height() / 2);

    for(int index = 0; index < 5; ++index)
    {
        bn::sprite_builder builder(bn::sprite_items::tank_extras);
        builder.set_position(-32, footprint_y + (index * 32));
        builder.set_z_order(constants::footprint_z_order);
        builder.set_camera(camera);
        _footprint_sprites.push_back(builder.release_build());

        builder = bn::sprite_builder(bn::sprite_items::tank_extras);
        builder.set_position(32, footprint_y + (index * 32));
        builder.set_horizontal_flip(true);
        builder.set_z_order(constants::footprint_z_order);
        builder.set_camera(camera);
        _footprint_sprites.push_back(builder.release_build());
    }

    for(int index = 0; index < 9; index += 3)
    {
        bn::sprite_builder builder(bn::sprite_items::tank_base, index);
        builder.set_x(-32);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_camera(camera);
        _base_sprites.push_back(builder.release_build());

        builder = bn::sprite_builder(bn::sprite_items::tank_base, index + 1);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_camera(camera);
        _base_sprites.push_back(builder.release_build());

        builder = bn::sprite_builder(bn::sprite_items::tank_base, index + 2);
        builder.set_x(32);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_camera(camera);
        _base_sprites.push_back(builder.release_build());
    }

    bn::sprite_builder builder(bn::sprite_items::tank_jelly);
    builder.set_x(jelly_x);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _jelly_sprite = builder.release_build();

    builder = bn::sprite_builder(bn::sprite_items::tank_cannon);
    builder.set_rotation_angle(180);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _cannon_sprite = builder.release_build();

    builder = bn::sprite_builder(bn::sprite_items::tank_extras, 4);
    builder.set_z_order(constants::footprint_z_order);
    builder.set_visible(false);
    builder.set_camera(camera);
    _arrow_sprite = builder.release_build();

    _tank_rects.emplace_back(bn::fixed_point(), bn::fixed_size(84, 58));
    _tank_rects.emplace_back(bn::fixed_point(), bn::fixed_size(76, 74));

    bn::fixed y = _calculate_y_alive();
    _update_footprint_sprites(y);
    _update_base_sprites(y);
    _update_jelly_sprite(y, hero_position);
    _update_cannon_sprite(y, hero_position);
    _update_rects(y);
}

void tank_boss::_update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                              const bn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    switch(_state_index)
    {

    case 0:
    case 1:
        if(_y >= -70)
        {
            _y_inc = -0.25;
        }
        else if(_y <= -90)
        {
            _y_inc = 0.25;
        }
        break;

    case 2:
        if(_y >= -70)
        {
            _y_inc = -0.25;
        }
        else if(_y <= -90)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;
        }
        break;

    case 3:
        if(_y >= 70)
        {
            _y_inc = -0.25;
        }
        else if(_y <= -90)
        {
            _y_inc = 0.25;
        }

        if(_y_inc < 0)
        {
            if(_y >= 20)
            {
                _arrow_sprite->set_visible(true);
                _arrow_sprite->set_vertical_flip(true);
            }
            else
            {
                _arrow_sprite->set_visible(false);
            }
        }
        else
        {
            if(_y <= -40)
            {
                _arrow_sprite->set_visible(true);
                _arrow_sprite->set_vertical_flip(false);
            }
            else
            {
                _arrow_sprite->set_visible(false);
            }
        }
        break;

    case 4:
        if(_y >= -80)
        {
            _y_inc = -0.25;
        }
        else if(_y <= -90)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;
        }
        break;

    case 5:
        if(_y >= -80)
        {
            _y_inc = -0.25;
        }
        else if(_y <= -90)
        {
            _y_inc = 0.25;
        }
        break;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    _y += _y_inc;

    bn::fixed y = _calculate_y_alive();
    _update_footprint_sprites(y);
    _update_base_sprites(y);
    _update_jelly_sprite(y, hero_position);
    _update_cannon_sprite(y, hero_position);
    _update_explosions();
    _update_rects(y);

    if(! hero_bomb.active())
    {
        _update_bullets(hero_position, y, camera, enemy_bullets);
    }
}

bool tank_boss::_update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera, background&,
                             rumble_manager& rumble_manager)
{
    bool done = false;
    _y += constants::background_speed;

    if(_cannon_sprite)
    {
        _cannon_sprite.reset();

        for(bn::sprite_ptr& sprite : _base_sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _jelly_sprite->set_z_order(constants::intro_sprites_z_order);
        _base_palette.set_fade(bn::colors::red, 0);
        _base_palette_action.emplace(_base_palette, 15, bn::fixed(0.5));
        _cannon_palette.set_fade(bn::colors::red, 0);
        _cannon_palette_action.emplace(_cannon_palette, 15, bn::fixed(0.5));
    }

    if(_death_flash())
    {
        _state_index = 0;
        _bullets_index = 1;
    }
    else
    {
        if(_state_index < 15)
        {
            --_bullets_index;

            if(! _bullets_index)
            {
                ++_state_index;
                _bullets_index = 16;

                bn::fixed x = int(_random.get() % 48) - 24;
                bn::fixed y = int(_random.get() % 48) - 24 + _y;
                _mini_explosions.push_back(_create_mini_explosion(x, y, camera));
                bn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(bn::sprite_items::enemy_explosion, bn::fixed_point(0, bn::display::height() / 2),
                                   6, constants::enemy_explosions_z_order, true, camera);
                bn::sound_items::explosion_2.play();
                rumble_manager.set_enabled(true);
            }
        }
        else if(_state_index == 16)
        {
            if(_explosion)
            {
                if(_jelly_sprite && ! _explosion->show_target_sprite())
                {
                    _base_sprites.clear();
                    _jelly_sprite.reset();
                    _jelly_animate_action.reset();
                    _arrow_sprite.reset();
                }
            }
            else
            {
                rumble_manager.set_enabled(false);
                done = true;
            }
        }
    }

    bn::fixed y = _calculate_y_dead();
    _update_footprint_sprites(y);

    if(_jelly_sprite)
    {
        _update_base_sprites(y);
        _update_jelly_sprite(y, hero_position);
    }

    _update_explosions();
    return done;
}

void tank_boss::_show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera)
{
    int current_life = life();
    _jelly_damage_counter = jelly_damage_frames;

    switch(_state_index)
    {

    case 0:
        if(current_life < total_life - state_0_1_life)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;

            _mini_explosions.push_back(_create_mini_explosion(-24, _y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(24, _y + 24, camera));
            _base_palette.set_fade(bn::colors::red, 0);
            _base_palette_action.emplace(_base_palette, 30, bn::fixed(0.2));
            _cannon_palette.set_fade(bn::colors::red, 0);
            _cannon_palette_action.emplace(_cannon_palette, 30, bn::fixed(0.2));
            bn::sound_items::explosion_3.play();
        }
        break;

    case 1:
        if(current_life < total_life - state_0_1_life - state_2_life)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;

            _explosion.emplace(bn::sprite_items::enemy_explosion, bn::fixed_point(0, _y), 6,
                               constants::enemy_explosions_z_order, false, camera);
            _mini_explosions.push_back(_create_mini_explosion(24, _y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(-24, _y + 24, camera));
            _base_palette.set_fade(bn::colors::red, 0);
            _base_palette_action.emplace(_base_palette, 25, bn::fixed(0.3));
            _cannon_palette.set_fade(bn::colors::red, 0);
            _cannon_palette_action.emplace(_cannon_palette, 25, bn::fixed(0.3));
            bn::sound_items::explosion_1.play();
            bn::sound_items::explosion_3.play();
        }
        break;

    case 2:
        break;

    case 3:
        if(current_life < total_life - state_0_1_life - state_2_life - state_3_4_life)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;

            _explosion.emplace(bn::sprite_items::enemy_explosion, bn::fixed_point(0, _y), 6,
                               constants::enemy_explosions_z_order, false, camera);
            _mini_explosions.push_back(_create_mini_explosion(-24, _y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(24, _y + 24, camera));
            _base_palette.set_fade(bn::colors::red, 0);
            _base_palette_action.emplace(_base_palette, 20, bn::fixed(0.4));
            _cannon_palette.set_fade(bn::colors::red, 0);
            _cannon_palette_action.emplace(_cannon_palette, 20, bn::fixed(0.4));
            _arrow_sprite->set_visible(false);
            bn::sound_items::explosion_1.play();
            bn::sound_items::explosion_3.play();
        }
        break;

    case 4:
        break;

    case 5:
        break;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    for(bn::sprite_ptr& sprite : _base_sprites)
    {
        sprite.set_palette(damage_palette);
    }

    _jelly_sprite->set_palette(damage_palette);
    _cannon_sprite->set_palette(damage_palette);
}

void tank_boss::_hide_damage_palette()
{
    for(bn::sprite_ptr& sprite : _base_sprites)
    {
        sprite.set_palette(_base_palette);
    }

    _jelly_sprite->set_palette(_jelly_palette);

    if(_cannon_sprite)
    {
        _cannon_sprite->set_palette(_cannon_palette);
    }
}

bool tank_boss::_hero_should_look_down_impl(const bn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(! _jelly_sprite)
    {
        return false;
    }

    if(hero_position.y() < _y - 48)
    {
        return true;
    }

    if(hero_position.y() > _y + 48)
    {
        return false;
    }

    return hero_is_looking_down;
}

bn::fixed tank_boss::_calculate_y_alive()
{
    --_vibration_counter;

    if(! _vibration_counter)
    {
        _vibration_counter = vibration_frames * 2;
    }

    bn::fixed result = _y;

    if(_vibration_counter > vibration_frames)
    {
        result += 1;
    }

    return result;
}

bn::fixed tank_boss::_calculate_y_dead()
{
    --_vibration_counter;

    if(! _vibration_counter)
    {
        _vibration_counter = 6;
    }

    bn::fixed result = _y;

    if(_vibration_counter > 3)
    {
        result += 1;
    }

    return result;
}

bn::fixed tank_boss::_cannon_rotation_angle_inc() const
{
    switch(_state_index)
    {

    case 0:
    case 1:
    case 2:
        return 0.25;

    case 3:
    case 4:
    case 5:
        return 0.5;

    default:
        BN_ERROR("Invalid state index: ", _state_index);
        return 0;
    }
}

void tank_boss::_update_footprint_sprites(bn::fixed y)
{
    --_footprint_counter;

    if(! _footprint_counter)
    {
        _footprint_graphics_index = (_footprint_graphics_index + 1) % 4;
        _footprint_counter = 2;

        bn::sprite_tiles_ptr tiles = _footprint_sprites[0].tiles();
        tiles.set_tiles_ref(bn::sprite_items::tank_extras.tiles_item(), _footprint_graphics_index);
    }

    bn::fixed hidden_y = -44;

    for(int index = 0; index < 5; ++index)
    {
        bn::sprite_ptr& left_sprite = _footprint_sprites[index * 2];
        bn::sprite_ptr& right_sprite = _footprint_sprites[(index * 2) + 1];
        bool visible = y >= hidden_y;
        left_sprite.set_visible(visible);
        right_sprite.set_visible(visible);
        hidden_y += 32;
    }
}

void tank_boss::_update_base_sprites(bn::fixed y)
{
    _base_sprites[0].set_y(y - 32);
    _base_sprites[1].set_y(y - 32);
    _base_sprites[2].set_y(y - 32);
    _base_sprites[3].set_y(y);
    _base_sprites[4].set_y(y);
    _base_sprites[5].set_y(y);
    _base_sprites[6].set_y(y + 32);
    _base_sprites[7].set_y(y + 32);
    _base_sprites[8].set_y(y + 32);

    for(int index = 0; index < 3; ++index)
    {
        int sprite_index = index * 3;
        bn::fixed base_y = y - 32 + (index * 32);
        _base_sprites[sprite_index + 0].set_y(base_y);
        _base_sprites[sprite_index + 1].set_y(base_y);
        _base_sprites[sprite_index + 2].set_y(base_y);
    }

    if(_arrow_sprite->visible())
    {
        if(_vibration_counter > vibration_frames)
        {
            if(_arrow_sprite->vertical_flip())
            {
                _arrow_sprite->set_y(_y - 64);
            }
            else
            {
                _arrow_sprite->set_y(_y + 64);
            }
        }
        else
        {
            _arrow_sprite->set_y(bn::display::height());
        }
    }

    if(_base_palette_action)
    {
        _base_palette_action->update();
    }
}

void tank_boss::_update_jelly_sprite(bn::fixed y, const bn::fixed_point& hero_position)
{
    bn::sprite_ptr& jelly_sprite = *_jelly_sprite;

    if(_jelly_damage_counter)
    {
        --_jelly_damage_counter;

        if(_jelly_damage_counter)
        {
            if(! _jelly_crying)
            {
                _jelly_animate_action = bn::create_sprite_cached_animate_action_forever(
                            jelly_sprite, 16, bn::sprite_items::tank_jelly.tiles_item(), 2, 3);
                _jelly_crying = true;
            }
        }
        else
        {
            if(life())
            {
                _jelly_animate_action = bn::create_sprite_cached_animate_action_forever(
                            jelly_sprite, 16, bn::sprite_items::tank_jelly.tiles_item(), 0, 1);
                _jelly_crying = false;
            }
        }
    }

    jelly_sprite.set_y(y + 20);
    jelly_sprite.set_horizontal_flip(hero_position.x() < jelly_x);
    _jelly_animate_action->update();
}

void tank_boss::_update_cannon_sprite(bn::fixed y, const bn::fixed_point& hero_position)
{
    _cannon_sprite->set_y(y);

    if(_cannon_palette_action)
    {
        _cannon_palette_action->update();
    }

    bn::fixed current_rotation_angle = _cannon_sprite->rotation_angle();
    bn::fixed previous_rotation_angle = _fix_rotation_angle(current_rotation_angle - 1);
    bn::fixed next_rotation_angle = _fix_rotation_angle(current_rotation_angle + 1);
    bn::fixed_point current_end_position = _cannon_end_position(current_rotation_angle, y);
    bn::fixed_point previous_end_position = _cannon_end_position(previous_rotation_angle, y);
    bn::fixed_point next_end_position = _cannon_end_position(next_rotation_angle, y);
    bn::fixed current_hero_distance = _squared_distance(current_end_position, hero_position);
    bn::fixed previous_hero_distance = _squared_distance(previous_end_position, hero_position);
    bn::fixed next_hero_distance = _squared_distance(next_end_position, hero_position);

    if(previous_hero_distance < current_hero_distance && previous_hero_distance < next_hero_distance)
    {
        _cannon_sprite->set_rotation_angle(
                    _fix_rotation_angle(current_rotation_angle - _cannon_rotation_angle_inc()));
    }
    else if(next_hero_distance < current_hero_distance)
    {
        _cannon_sprite->set_rotation_angle(
                    _fix_rotation_angle(current_rotation_angle + _cannon_rotation_angle_inc()));
    }
}

void tank_boss::_update_rects(bn::fixed y)
{
    _tank_rects[0].set_y(y - 2);
    _tank_rects[1].set_y(y);
}

void tank_boss::_update_bullets(const bn::fixed_point& hero_position, bn::fixed y, const bn::camera_ptr& camera,
                                enemy_bullets& enemy_bullets)
{
    --_bullets_counter;

    if(! _bullets_counter && _cannon_sprite)
    {
        bn::fixed cannon_rotation_angle = _cannon_sprite->rotation_angle();

        switch(_state_index)
        {

        case 0:
            _bullets_counter = 60;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::SMALL, cannon_rotation_angle, hero_position,
                              y, camera, enemy_bullets);
                _bullets_index = 1;
                break;

            case 1:
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 15), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 15), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _bullets_index = 0;
                break;

            default:
                BN_ERROR("Invalid bullets index: ", _bullets_index);
                break;
            }
            break;

        case 1:
            _bullets_counter = 80;
            _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::BIG, cannon_rotation_angle, hero_position,
                          y, camera, enemy_bullets);
            _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 30), enemy_bullet_type::SMALL,
                          cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
            _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 30), enemy_bullet_type::SMALL,
                          cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
            break;

        case 2:
            _bullets_counter = 60;
            break;

        case 3:
            _bullets_counter = 60;
            _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::SMALL, cannon_rotation_angle, hero_position,
                          y, camera, enemy_bullets);
            break;

        case 4:
            _bullets_counter = 60;
            break;

        case 5:
            _bullets_counter = 70;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::SMALL, cannon_rotation_angle, hero_position,
                              y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 25), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 25), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 50), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 50), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 75), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 75), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 1:
                _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::BIG, cannon_rotation_angle, hero_position,
                              y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 30), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 30), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 2:
                _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::SMALL, cannon_rotation_angle, hero_position,
                              y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 25), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 25), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 50), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 50), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle - 75), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                _shoot_bullet(_fix_rotation_angle(cannon_rotation_angle + 75), enemy_bullet_type::SMALL,
                              cannon_rotation_angle, hero_position, y, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 3:
                _shoot_bullet(cannon_rotation_angle, enemy_bullet_type::HUGE, cannon_rotation_angle, hero_position,
                              y, camera, enemy_bullets);
                _bullets_index = 0;
                break;

            default:
                BN_ERROR("Invalid bullets index: ", _bullets_index);
                break;
            }
            break;

        default:
            BN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void tank_boss::_update_explosions()
{
    for(auto it = _mini_explosions.begin(), end = _mini_explosions.end(); it != end; )
    {
        bn::sprite_animate_action<7>& mini_explosions = *it;
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
