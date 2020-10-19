#include "bf_game_gigabat_boss.h"

#include "btn_colors.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_gigabat.h"
#include "btn_sprite_items_hero_death.h"
#include "btn_sprite_items_gigabat_shadow.h"
#include "btn_sprite_items_mini_explosion_2.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr const int state_0_1_life = 350;   // 16 seconds
    constexpr const int state_2_life = 440;     // 24 seconds
    constexpr const int state_3_4_life = 350;   // 20 seconds
    constexpr const int total_life = state_0_1_life + state_2_life + state_3_4_life;

    constexpr const int mouth_y = 10;
    constexpr const btn::fixed rotate_speed = 0.35;

    [[nodiscard]] btn::sprite_animate_action<5> _create_mini_explosion(btn::fixed x, btn::fixed y,
                                                                       const btn::camera_ptr& camera)
    {
        btn::sprite_builder builder(btn::sprite_items::mini_explosion_2);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        builder.set_camera(camera);
        return btn::create_sprite_animate_action_once(
                    builder.release_build(), 6, btn::sprite_items::mini_explosion_2.tiles_item(), 0, 1, 2, 3, 4);
    }
}

gigabat_boss::gigabat_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette,
                           const btn::camera_ptr& camera) :
    boss(total_life, 400, _gigabat_rects, damage_palette),
    _gigabat_position(0, -125),
    _palette(btn::sprite_items::gigabat.palette_item().create_palette())
{
    btn::sprite_builder builder(btn::sprite_items::gigabat);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _sprites.push_back(builder.release_build());

    const btn::sprite_tiles_item& tiles_item = btn::sprite_items::gigabat.tiles_item();
    _animate_actions.push_back(
                btn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 0, 2, 0, 4, 6));

    builder = btn::sprite_builder(btn::sprite_items::gigabat, 1);
    builder.set_z_order(constants::enemies_z_order);
    builder.set_camera(camera);
    _sprites.push_back(builder.release_build());
    _animate_actions.push_back(
                btn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 1, 3, 1, 5, 7));

    builder = btn::sprite_builder(btn::sprite_items::gigabat_shadow);
    builder.set_blending_enabled(true);
    builder.set_z_order(constants::footprint_z_order);
    builder.set_camera(camera);
    _shadow_sprite = builder.release_build();

    _gigabat_rects.emplace_back(btn::fixed_point(), btn::fixed_size(44, 45));
    _update_sprites(hero_position, false);
    _update_rects();
}

void gigabat_boss::_update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                 const btn::camera_ptr& camera, enemy_bullets& enemy_bullets)
{
    btn::fixed x = _gigabat_position.x();
    btn::fixed y = _gigabat_position.y();

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
        _gigabat_position.set_y((btn::lut_sin(_movement_counter) * 5) - 50);
        break;

    case 2:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
                _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), 10);
                _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), 11);
                _target_x = hero_position.x();
                _movement_index = 1;
                _movement_counter = 80;
                _vibrate = false;
                _delta_position = (hero_position - _gigabat_position) / _movement_counter;
                break;

            case 1:
                _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), 8);
                _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), 9);
                _target_x.reset();
                _movement_index = 0;
                _movement_counter = 500;
                _vibrate = true;
                _bullets_counter = 100;
                break;

            default:
                BTN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            if(y < -56)
            {
                _gigabat_position.set_y(btn::min(y + 0.5, btn::fixed(-56)));
            }
            else if(y > -56)
            {
                _gigabat_position.set_y(btn::max(y - 0.5, btn::fixed(-56)));
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
            BTN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 3:
        if(y < -34)
        {
            _gigabat_position.set_y(btn::min(y + 0.25, btn::fixed(-34)));
        }
        else if(y > -34)
        {
            _gigabat_position.set_y(btn::max(y - 0.25, btn::fixed(-34)));
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
            _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), base_graphics_index);
            _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), base_graphics_index + 1);
            _sprites[2].set_tiles(btn::sprite_items::gigabat.tiles_item(), base_graphics_index + 2);
            _sprites[3].set_tiles(btn::sprite_items::gigabat.tiles_item(), base_graphics_index + 3);
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
        BTN_ERROR("Invalid state index: ", _state_index);
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

bool gigabat_boss::_update_dead(const btn::fixed_point& hero_position, const btn::camera_ptr& camera, background&)
{
    bool hide_shadow = false;
    bool done = false;

    if(_sprites.size() == 4)
    {
        _movement_counter = 1;
        _delta_position.set_x((0 - _gigabat_position.x()) / 240);
        _delta_position.set_y((0 - _gigabat_position.y()) / 240);

        _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), 28);
        _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), 29);
        _sprites.pop_back();
        _sprites.pop_back();

        for(btn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _palette.set_fade(btn::colors::red, 0);
        _palette_action.emplace(_palette, 15, 0.6);
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

                btn::fixed x = int(_random.get() % 48) - 24 + _gigabat_position.x();
                btn::fixed y = int(_random.get() % 48) - 24 + _gigabat_position.y();
                _mini_explosions.push_back(_create_mini_explosion(x, y, camera));
                btn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(btn::sprite_items::hero_death, btn::fixed_point(), 6,
                                   constants::enemy_explosions_z_order, true, camera);
                btn::sound_items::explosion_2.play();
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

void gigabat_boss::_show_damage_palette(const btn::sprite_palette_ptr& damage_palette, const btn::camera_ptr& camera)
{
    btn::fixed x = _gigabat_position.x();
    btn::fixed y = _gigabat_position.y();
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
            _palette.set_fade(btn::colors::red, 0);
            _palette_action.emplace(_palette, 25, 0.4);
            btn::sound_items::explosion_3.play();
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
            _random = btn::random();
            _delta_position = btn::fixed_point(rotate_speed, -rotate_speed);
            _target_x = constants::play_width;

            _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), 12);
            _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), 13);

            btn::sprite_builder builder(btn::sprite_items::gigabat, 14);
            builder.set_z_order(constants::enemies_z_order);
            builder.set_camera(camera);
            _sprites.push_back(builder.release_build());

            builder = btn::sprite_builder(btn::sprite_items::gigabat, 15);
            builder.set_z_order(constants::enemies_z_order);
            builder.set_camera(camera);
            _sprites.push_back(builder.release_build());

            _explosion.emplace(btn::sprite_items::hero_death, btn::fixed_point(x, y), 6,
                               constants::enemy_explosions_z_order, false, camera);
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 8, camera));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 24, camera));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16, camera));
            _palette.set_fade(btn::colors::red, 0);
            _palette_action.emplace(_palette, 20, 0.5);
            btn::sound_items::explosion_1.play();
            btn::sound_items::explosion_3.play();
        }
        break;

    case 3:
    case 4:
        break;

    default:
        BTN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(damage_palette);
    }
}

void gigabat_boss::_hide_damage_palette()
{
    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(_palette);
    }
}

bool gigabat_boss::_hero_should_look_down_impl(const btn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_sprites.empty())
    {
        return false;
    }

    btn::fixed y = _gigabat_position.y();

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

void gigabat_boss::_shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                                 const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                 enemy_bullets& enemy_bullets) const
{
    btn::fixed_point mouth_position = _gigabat_position + btn::fixed_point(0, mouth_y);
    enemy_bullets.add_bullet(hero_position, mouth_position, enemy_bullet_event(bullet_type, delta_position, 1), camera);
}

void gigabat_boss::_shoot_target_random_bullet(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                               enemy_bullets& enemy_bullets)
{
    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    btn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? 0.9 : 1.0;

    if(_random.get() % 8 == 0)
    {
        btn::fixed_point mouth_position = _gigabat_position + btn::fixed_point(0, mouth_y);
        btn::fixed_point distance = hero_position - mouth_position;

        if(distance == btn::fixed_point())
        {
            distance.set_y(1);
        }

        btn::fixed_point delta_position = direction_vector(distance.x(), distance.y(), bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
    }
    else
    {
        btn::fixed bullet_x = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;
        btn::fixed bullet_y = btn::fixed::from_data(int(_random.get() % btn::fixed(1).data()));

        if(bullet_x == 0 && bullet_y == 0)
        {
            bullet_y = 1;
        }

        btn::fixed_point delta_position = direction_vector(bullet_x, bullet_y, bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
    }
}

void gigabat_boss::_shoot_free_random_bullet(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
                                             enemy_bullets& enemy_bullets)
{
    if(btn::abs(_gigabat_position.x() - hero_position.x()) < 48 &&
            btn::abs(_gigabat_position.y() - hero_position.y()) < 48)
    {
        return;
    }

    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    btn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? 0.9 : 1.0;
    btn::fixed bullet_x = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;
    btn::fixed bullet_y = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;

    if(bullet_x == 0 && bullet_y == 0)
    {
        bullet_y = 1;
    }

    btn::fixed_point delta_position = direction_vector(bullet_x, bullet_y, bullet_speed);
    _shoot_bullet(bullet_type, delta_position, hero_position, camera, enemy_bullets);
}

void gigabat_boss::_update_sprites(const btn::fixed_point& hero_position, bool hide_shadow)
{
    btn::fixed target_x = _target_x.value_or(hero_position.x());
    bool flip = target_x < _gigabat_position.x();
    btn::fixed x1_inc = flip ? 32 : -32;
    btn::fixed x2_inc = flip ? -32 : 32;
    btn::fixed y_inc = _vibrate ? 1 : 0;

    if(_sprites.size() == 2)
    {
        _sprites[0].set_position(_gigabat_position + btn::fixed_point(x1_inc, y_inc));
        _sprites[1].set_position(_gigabat_position + btn::fixed_point(x2_inc, y_inc));
    }
    else
    {
        _sprites[0].set_position(_gigabat_position + btn::fixed_point(x1_inc, y_inc - 32));
        _sprites[1].set_position(_gigabat_position + btn::fixed_point(x2_inc, y_inc - 32));
        _sprites[2].set_position(_gigabat_position + btn::fixed_point(x1_inc, y_inc + 32));
        _sprites[3].set_position(_gigabat_position + btn::fixed_point(x2_inc, y_inc + 32));
    }

    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_horizontal_flip(flip);
    }

    for(btn::sprite_animate_action<5>& animate_action : _animate_actions)
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
        _shadow_sprite->set_position(_gigabat_position + btn::fixed_point(0, 40));

        btn::fixed scale = _shadow_sprite->horizontal_scale();

        if(scale < 1)
        {
            _shadow_sprite->set_scale(btn::min(scale + 0.05, btn::fixed(1)));
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

void gigabat_boss::_update_bullets(const btn::fixed_point& hero_position, const btn::camera_ptr& camera,
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
                BTN_ERROR("Invalid bullets index: ", _bullets_index);
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
                BTN_ERROR("Invalid movement index: ", _movement_index);
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
            BTN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void gigabat_boss::_update_explosions()
{
    for(auto it = _mini_explosions.begin(), end = _mini_explosions.end(); it != end; )
    {
        btn::sprite_animate_action<5>& mini_explosions = *it;
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
