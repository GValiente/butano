#include "bf_game_butano_boss.h"

#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_butano_fire.h"
#include "btn_sprite_items_mini_explosion.h"
#include "btn_sprite_items_enemy_explosion.h"
#include "btn_sprite_items_butano_big_sprite.h"
#include "btn_sprite_items_butano_flash_palette.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_background.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr const int state_0_1_2_3_life = 900;   // 15 seconds
    constexpr const int state_4_life = 1100;        // 30 seconds
    constexpr const int state_5_6_life = 600;       // 17 seconds
    constexpr const int total_life = state_0_1_2_3_life + state_4_life + state_5_6_life;

    constexpr const btn::fixed scale_margin = 0.1;
    constexpr const btn::fixed x_limit = (btn::display::width() + 256) / 2;

    [[nodiscard]] btn::sprite_animate_action<7> _create_mini_explosion(btn::fixed x, btn::fixed y,
                                                                       const btn::camera_ptr& camera)
    {
        btn::sprite_builder builder(btn::sprite_items::mini_explosion);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        builder.set_camera(camera);
        return btn::create_sprite_animate_action_once(
                    builder.release_build(), 4, btn::sprite_items::mini_explosion.tiles_item(), 0, 1, 2, 3, 4, 5, 6);
    }
}

butano_boss::butano_boss(const btn::sprite_palette_ptr& damage_palette, const btn::camera_ptr& camera) :
    boss(total_life, 1000, _butano_rects, damage_palette),
    _butano_position(0, -100)
{
    _sprites.push_back(btn::sprite_items::butano_big_sprite.create_sprite(0, 0));
    _sprites.push_back(btn::sprite_items::butano_big_sprite.create_sprite(0, 0, 1));
    _sprites.push_back(btn::sprite_items::butano_fire.create_sprite(0, 0));
    _sprites[2].set_z_order(constants::footprint_z_order);

    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_camera(camera);
    }

    btn::color fade_color(10, 1, 8);
    btn::sprite_palette_ptr body_palette = _sprites[0].palette();
    btn::sprite_palette_ptr fire_palette = _sprites[2].palette();
    body_palette.set_fade_color(fade_color);
    fire_palette.set_fade_color(fade_color);

    _update_sprites();
    _update_rects();
}

void butano_boss::play_music() const
{
    btn::music_items::basic_instinct.play(0.8);
}

void butano_boss::_update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                const btn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    btn::fixed x = _butano_position.x();
    btn::fixed y = _butano_position.y();

    switch(_state_index)
    {

    case 0:
        _butano_position.set_y(y + 0.1);

        if(_butano_position.y() >= -10)
        {
            _butano_position.set_y(-10);
            ++_state_index;
        }
        break;

    case 1:
        _butano_position.set_y(y - 0.5);
        _scale += 0.01;

        if(_scale >= 1 - (scale_margin * 2))
        {
            _scale = 1 - (scale_margin * 2);
            ++_state_index;
        }
        break;

    case 2:
        _butano_position.set_y(y - 0.95);
        _scale += 0.004;

        if(_scale >= 1)
        {
            _scale = 1;
            ++_state_index;
        }
        break;

    case 3:
        if(_movement_index == 0)
        {
            _butano_position.set_x(x + 0.35);

            if(_butano_position.x() >= 50)
            {
                _movement_index = 1;
            }
        }
        else
        {
            _butano_position.set_x(x - 0.35);

            if(_butano_position.x() <= -50)
            {
                _movement_index = 0;
            }
        }

        _movement_counter = (_movement_counter + 4) % 512;

        {
            btn::fixed movement_sin = btn::lut_sin(_movement_counter);
            _butano_position.set_y((movement_sin * 4) - 90);
            _scale = 1 - (btn::abs(movement_sin) * (scale_margin * 2));
        }
        break;

    case 4:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
            case 3:
                ++_movement_index;
                _movement_counter = 360;
                break;

            case 1:
            case 4:
                ++_movement_index;
                _movement_counter = 200;
                _rotation_angle = 0;
                _scale = 0.3;
                break;

            case 2:
                ++_movement_index;
                _movement_counter = 260;
                _rotation_angle = 180;
                break;

            case 5:
                _movement_index = 0;
                _movement_counter = 260;
                _rotation_angle = 180;
                break;

            default:
                BTN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            _scale = btn::min(_scale + 0.01, btn::fixed(1));

            if(x < -64)
            {
                _butano_position.set_x(x + 1);
            }
            else if(x > -64)
            {
                _butano_position.set_x(x - 1);
            }

            if(y < -100)
            {
                _butano_position.set_y(y + 0.5);
            }
            else if(y > -100)
            {
                _butano_position.set_y(y - 0.5);
            }

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 1:
        case 4:
            _butano_position.set_y(y + 0.75);

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 2:
            _butano_position.set_x(btn::min(x + 0.6, btn::fixed(64)));
            _butano_position.set_y(y - 1.6);
            _scale += 0.002;
            _vibrate = false;
            break;

        case 3:
            _scale = btn::min(_scale + 0.01, btn::fixed(1));

            if(x < 64)
            {
                _butano_position.set_x(x + 1);
            }
            else if(x > 64)
            {
                _butano_position.set_x(x - 1);
            }

            if(y < -100)
            {
                _butano_position.set_y(y + 0.5);
            }
            else if(y > -100)
            {
                _butano_position.set_y(y - 0.5);
            }

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 5:
            _butano_position.set_x(btn::max(x - 0.6, btn::fixed(-64)));
            _butano_position.set_y(y - 1.6);
            _scale += 0.002;
            _vibrate = false;
            break;

        default:
            BTN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 5:
        _butano_position += _delta_position;
        _scale += _delta_scale;
        --_movement_counter;

        if(! _movement_counter)
        {
            ++_state_index;
            _movement_index = 1;
            _movement_counter = 1;
            _butano_position.set_y(-48);
            _scale = 1;
        }
        break;

    case 6:
        --_movement_counter;

        if(! _movement_counter)
        {
            _movement_counter = 256;

            if(_movement_index == 0)
            {
                _movement_index = 1;
                _rotation_angle = 90;
                _butano_position.set_x(x_limit);
                _delta_position = (btn::fixed_point(-x_limit, -48) - _butano_position) / _movement_counter;
            }
            else
            {
                _movement_index = 0;
                _rotation_angle = 270;
                _butano_position.set_x(-x_limit);
                _delta_position = (btn::fixed_point(x_limit, -48) - _butano_position) / _movement_counter;
            }
        }

        _butano_position += _delta_position;

        if(_movement_counter % 4 == 0)
        {
            _vibrate = ! _vibrate;
        }
        break;

    default:
        BTN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    _update_sprites();
    _update_explosions();
    _update_rects();

    if(! hero_bomb.active())
    {
        _update_bullets(hero_position, camera, enemy_bullets);
    }
}

bool butano_boss::_update_dead(const btn::fixed_point&, const btn::camera_ptr& camera, background& background)
{
    bool done = false;

    if(_sprites.size() > 2)
    {
        btn::fixed x = _butano_position.x().integer();
        btn::fixed y = _butano_position.y().integer();
        _movement_counter = 1;
        _sprites.pop_back();
        _butano_position.set_x(x);
        _butano_position.set_y(y);
        _delta_position.set_x((0 - x) / 300);
        _delta_position.set_y((0 - y) / 300);
        _delta_scale = btn::fixed(0 - 1) / 300;
    }

    _butano_position += _delta_position;
    _scale += _delta_scale;
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

                btn::fixed scale = _scale;
                btn::fixed x = (int(_random.get() % 96) - 48) * scale;
                btn::fixed y = (int(_random.get() % 48) - 24) * scale;
                _mini_explosions.push_back(
                            _create_mini_explosion(_butano_position.x() + x, _butano_position.y() + y, camera));

                btn::sprite_ptr mini_explosion_sprite = _mini_explosions.back().sprite();
                mini_explosion_sprite.set_scale(scale);
                btn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            ++_state_index;

            btn::rect_window external_window = btn::rect_window::external();
            external_window.set_boundaries(0, 0, 0, 0);
            external_window.set_camera(camera);
            _move_window_top_action.emplace(external_window, -4);
            _move_window_bottom_action.emplace(external_window, 4);

            _circle_generator.set_radius(0);
            _circle_generator.generate(_circle_hblank_effect_deltas);
            _circle_hblank_effect = btn::rect_window_boundaries_hblank_effect_ptr::create_horizontal(
                        external_window, _circle_hblank_effect_deltas);

            _explosion_counter = 50;
            _flame_sound_counter = 0;
            background.show_explosion_open(_explosion_counter);
            _sprites.clear();
            _mini_explosions.clear();
            btn::sound_items::explosion_2.play();
        }
        else if(_state_index == 16)
        {
            if(_explosion_counter)
            {
                --_explosion_counter;

                _move_window_top_action->update();
                _move_window_bottom_action->update();

                _circle_generator.set_radius(_circle_generator.radius() + 4);
                _circle_generator.generate(_circle_hblank_effect_deltas);
                _circle_hblank_effect->reload_deltas_ref();
            }
            else
            {
                btn::rect_window external_window = btn::rect_window::external();
                external_window.set_boundaries(0, 0, 0, 0);
                external_window.remove_camera();
                _move_window_top_action.reset();
                _move_window_bottom_action.reset();

                _circle_hblank_effect.reset();
                ++_state_index;

                _explosion_counter = 130;
                background.show_explosion_close(_explosion_counter);
            }
        }
        else if(_state_index == 17)
        {
            if(_explosion_counter)
            {
                --_explosion_counter;
            }
            else
            {
                _flame_sound_counter = -1;
                done = true;
            }
        }
    }

    if(! _sprites.empty())
    {
        _update_sprites();
    }

    _update_explosions();

    if(_flame_sound_counter >= 0)
    {
        ++_flame_sound_counter;

        if(_flame_sound_counter > 16 && _flame_sound_counter % 16 == 0)
        {
            btn::sound_items::flame_thrower.play();
        }
    }

    return done;
}

void butano_boss::_show_damage_palette(const btn::sprite_palette_ptr&, const btn::camera_ptr& camera)
{
    btn::fixed x = _butano_position.x();
    btn::fixed y = _butano_position.y();
    int current_life = life();

    switch(_state_index)
    {

    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        if(current_life < total_life - state_0_1_2_3_life)
        {
            ++_state_index;
            _movement_index = 5;
            _movement_counter = 1;
            _bullets_index = 0;
            _bullets_counter = 120;

            _mini_explosions.push_back(_create_mini_explosion(x - 16, y + 32 + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 16, y + 32 + 16, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y + 32 - 8, camera));
            btn::sound_items::explosion_3.play();
        }
        break;

    case 4:
        if(current_life < total_life - state_0_1_2_3_life - state_4_life)
        {
            ++_state_index;
            _movement_index = 0;
            _movement_counter = 160;
            _bullets_index = 0;
            _bullets_counter = 1;
            _delta_position = (btn::fixed_point(-x_limit, -48) - _butano_position) / _movement_counter;
            _delta_scale = (0.4 - _scale) / _movement_counter;
            _random = btn::random();

            _mini_explosions.push_back(_create_mini_explosion(x, y, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y - 16, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 16, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 16, y + 48, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 16, y - 48, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 8, y + 32, camera));
            _mini_explosions.push_back(_create_mini_explosion(x + 8, y - 32, camera));
            btn::sound_items::explosion_1.play();
            btn::sound_items::explosion_3.play();
        }
        break;

    case 5:
        break;

    case 6:
        break;

    default:
        BTN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    btn::sprite_palette_ptr body_palette = _sprites[0].palette();
    body_palette.set_colors(btn::sprite_items::butano_flash_palette.palette_item().colors());
}

void butano_boss::_hide_damage_palette()
{
    btn::sprite_palette_ptr body_palette = _sprites[0].palette();
    body_palette.set_colors(btn::sprite_items::butano_big_sprite.palette_item().colors());
}

bool butano_boss::_hero_should_look_down_impl(const btn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_sprites.empty())
    {
        return false;
    }

    btn::fixed y = _butano_position.y();

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

bool butano_boss::_throw_bomb() const
{
    return false;
}

btn::fixed_point butano_boss::_top_position() const
{
    btn::fixed_point top_position = _butano_position;
    btn::fixed rotation_angle = _rotation_angle;
    btn::fixed inc = 54 * _scale;

    if(rotation_angle == 90)
    {
        top_position.set_x(top_position.x() - inc);
    }
    else if(rotation_angle == 180)
    {
        top_position.set_y(top_position.y() + inc);
    }
    else if(rotation_angle == 270)
    {
        top_position.set_x(top_position.x() + inc);
    }
    else
    {
        top_position.set_y(top_position.y() - inc);
    }

    return top_position;
}

void butano_boss::_shoot_bullet(enemy_bullet_type bullet_type, btn::fixed delta_speed,
                                const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                enemy_bullets& enemy_bullets) const
{
    enemy_bullets.add_bullet(hero_position, _top_position(), enemy_bullet_event(bullet_type, delta_speed, 1), camera);
}

void butano_boss::_shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                                const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                enemy_bullets& enemy_bullets) const
{
    enemy_bullets.add_bullet(hero_position, _top_position(), enemy_bullet_event(bullet_type, delta_position, 1), camera);
}

void butano_boss::_shoot_random_bullet(bool down_only, const btn::fixed_point& hero_position,
                                       const btn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    btn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? 0.9 : 1.0;

    if(_random.get() % 8 == 0)
    {
        _shoot_bullet(bullet_type, bullet_speed, hero_position, camera, enemy_bullets);
    }
    else
    {
        btn::fixed bullet_x = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;
        btn::fixed bullet_y = down_only ?
                    btn::fixed::from_data(int(_random.get() % btn::fixed(1).data())) :
                    btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;

        if(bullet_x == 0 && bullet_y == 0)
        {
            bullet_y = 1;
        }

        btn::fixed_point delta_position = aprox_direction_vector(bullet_x, bullet_y, bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
    }
}

void butano_boss::_update_sprites()
{
    btn::fixed scale = _scale;
    btn::fixed fire_scale = scale * _fire_scale_mult;

    if(_increase_fire_scale_mult)
    {
        _fire_scale_mult += 0.05;

        if(_fire_scale_mult >= 1)
        {
            _fire_scale_mult = 1;
            _increase_fire_scale_mult = false;
        }
    }
    else
    {
        _fire_scale_mult -= 0.05;

        if(_fire_scale_mult <= 0.5)
        {
            _fire_scale_mult = 0.5;
            _increase_fire_scale_mult = true;
        }
    }

    btn::fixed vibrate_inc = _vibrate ? 1 : 0;
    btn::fixed scale_inc = 32 * scale;
    btn::fixed fire_scale_inc = 54 * scale;
    bool fire_active = _sprites.size() > 2;
    int z_order = fire_active && scale >= 1 - scale_margin ?
                constants::enemies_z_order : constants::footprint_z_order;
    btn::fixed rotation_angle = _rotation_angle;
    _sprites[0].set_scale(scale);
    _sprites[0].set_z_order(z_order);
    _sprites[0].set_rotation_angle(rotation_angle);
    _sprites[1].set_scale(scale);
    _sprites[1].set_z_order(z_order);
    _sprites[1].set_rotation_angle(rotation_angle);

    if(fire_active)
    {
        _sprites[2].set_scale(fire_scale);
        _sprites[2].set_rotation_angle(rotation_angle);
    }

    if(rotation_angle == 90)
    {
        _sprites[0].set_position(_butano_position + btn::fixed_point(-scale_inc + 1, vibrate_inc));
        _sprites[1].set_position(_butano_position + btn::fixed_point(scale_inc - 1, vibrate_inc));

        if(fire_active)
        {
            _sprites[2].set_position(_butano_position + btn::fixed_point(fire_scale_inc, vibrate_inc));
        }
    }
    else if(rotation_angle == 180)
    {
        _sprites[0].set_position(_butano_position + btn::fixed_point(vibrate_inc, scale_inc - 1));
        _sprites[1].set_position(_butano_position + btn::fixed_point(vibrate_inc, -scale_inc + 1));

        if(fire_active)
        {
            _sprites[2].set_position(_butano_position + btn::fixed_point(vibrate_inc, -fire_scale_inc));
        }
    }
    else if(rotation_angle == 270)
    {
        _sprites[0].set_position(_butano_position + btn::fixed_point(scale_inc - 1, vibrate_inc));
        _sprites[1].set_position(_butano_position + btn::fixed_point(-scale_inc + 1, vibrate_inc));

        if(fire_active)
        {
            _sprites[2].set_position(_butano_position + btn::fixed_point(-fire_scale_inc, vibrate_inc));
        }
    }
    else
    {
        _sprites[0].set_position(_butano_position + btn::fixed_point(vibrate_inc, -scale_inc + 1));
        _sprites[1].set_position(_butano_position + btn::fixed_point(vibrate_inc, scale_inc - 1));

        if(fire_active)
        {
            _sprites[2].set_position(_butano_position + btn::fixed_point(vibrate_inc, fire_scale_inc));
        }
    }

    btn::fixed fade_intensity = 1 - btn::min(scale, btn::fixed(1));
    btn::sprite_palette_ptr body_palette = _sprites[0].palette();
    body_palette.set_fade_intensity(fade_intensity);

    if(fire_active)
    {
        btn::sprite_palette_ptr fire_palette = _sprites[2].palette();
        fire_palette.set_fade_intensity(fade_intensity);
    }
}

void butano_boss::_update_rects()
{
    btn::fixed scale = _scale;
    _butano_rects.clear();

    if(btn::abs(scale - 1) < scale_margin)
    {
        btn::fixed_point position = _butano_position;
        btn::fixed rotation_angle = _rotation_angle;

        if(rotation_angle == 0 || rotation_angle == 180)
        {
            btn::fixed_point center(position.x(), rotation_angle == 0 ? position.y() - 6 : position.y() + 6);
            _butano_rects.emplace_back(center, btn::fixed_size(58 * scale, 80 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(47 * scale, 86 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(36 * scale, 92 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(25 * scale, 98 * scale));
            _butano_rects.emplace_back(position, btn::fixed_size(10 * scale, 112 * scale));
        }
        else
        {
            btn::fixed_point center(rotation_angle == 270 ? position.x() - 6 : position.x() + 6, position.y());
            _butano_rects.emplace_back(center, btn::fixed_size(80 * scale, 58 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(86 * scale, 47 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(92 * scale, 36 * scale));
            _butano_rects.emplace_back(center, btn::fixed_size(98 * scale, 25 * scale));
            _butano_rects.emplace_back(position, btn::fixed_size(112 * scale, 10 * scale));
        }
    }
}

void butano_boss::_update_bullets(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
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
            break;

        case 2:
            break;

        case 3:
            _bullets_counter = 60;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(enemy_bullet_type::BIG, aprox_direction_vector(0, 1, 0.9), hero_position, camera,
                              enemy_bullets);
                ++_bullets_index;
                break;

            case 1:
                _shoot_bullet(enemy_bullet_type::SMALL, 1, hero_position, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 2:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                ++_bullets_index;
                break;

            case 3:
                _shoot_bullet(enemy_bullet_type::BIG, 0.9, hero_position, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 4:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.25, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.25, 1, 1), hero_position, camera,
                              enemy_bullets);
                ++_bullets_index;
                break;

            case 5:
                _shoot_bullet(enemy_bullet_type::HUGE, 0.8, hero_position, camera, enemy_bullets);
                ++_bullets_index;
                break;

            case 6:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.5, 1, 1), hero_position, camera,
                              enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0, 1, 1), hero_position, camera,
                              enemy_bullets);
                ++_bullets_index;
                break;

            case 7:
                _shoot_bullet(enemy_bullet_type::HUGE, 0.8, hero_position, camera, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::BIG, 0.8, hero_position, camera, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, 0.8, hero_position, camera, enemy_bullets);
                _bullets_index = 0;
                break;

            default:
                BTN_ERROR("Invalid bullets index: ", _bullets_index);
                break;
            }
            break;

        case 4:
            switch(_movement_index)
            {

            case 0:
            case 1:
            case 3:
            case 4:
                _bullets_counter = 12;
                _shoot_random_bullet(false, hero_position, camera, enemy_bullets);
                break;

            case 2:
            case 5:
                _bullets_counter = 1;
                break;

            default:
                BTN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
            break;

        case 5:
            break;

        case 6:
            _bullets_counter = 16;
            _shoot_random_bullet(true, hero_position, camera, enemy_bullets);
            break;

        default:
            BTN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void butano_boss::_update_explosions()
{
    for(auto it = _mini_explosions.begin(), end = _mini_explosions.end(); it != end; )
    {
        btn::sprite_animate_action<7>& mini_explosions = *it;
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
}

}
