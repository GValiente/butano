#include "bf_game_gigabat_boss.h"

#include "btn_colors.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_gigabat.h"
#include "btn_sprite_items_gigabat_shadow.h"
#include "btn_sprite_items_enemy_explosion.h"
#include "btn_sprite_items_mini_explosion_2.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    // constexpr const int state_0_1_life = 350;   // 16 seconds
    constexpr const int state_0_1_life = 50;   // 16 seconds
    constexpr const int state_2_life = 400;     // ? seconds
    constexpr const int state_2_3_life = 190;   // 30 seconds
    constexpr const int state_4_5_life = 190;   // 13 seconds
    constexpr const int total_life = state_0_1_life + state_2_life + state_2_3_life + state_4_5_life;

    [[nodiscard]] btn::sprite_animate_action<5> _create_mini_explosion(btn::fixed x, btn::fixed y)
    {
        btn::sprite_builder builder(btn::sprite_items::mini_explosion_2);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        return btn::create_sprite_animate_action_once(builder.release_build(), 8, btn::sprite_items::mini_explosion_2,
                                                      0, 1, 2, 3, 4);
    }
}

gigabat_boss::gigabat_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette) :
    boss(total_life, 300, _gigabat_rects, damage_palette),
    _gigabat_position(0, -125),
    _palette(btn::sprite_items::gigabat.palette_item().create_palette())
{
    btn::sprite_builder builder(btn::sprite_items::gigabat);
    builder.set_z_order(constants::enemies_z_order);
    _sprites.push_back(builder.release_build());

    const btn::sprite_tiles_item& tiles_item = btn::sprite_items::gigabat.tiles_item();
    _animate_actions.push_back(
                btn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 0, 2, 0, 4, 6));

    builder = btn::sprite_builder(btn::sprite_items::gigabat, 1);
    builder.set_z_order(constants::enemies_z_order);
    _sprites.push_back(builder.release_build());
    _animate_actions.push_back(
                btn::create_sprite_animate_action_forever(_sprites.back(), 8, tiles_item, 1, 3, 1, 5, 7));

    builder = btn::sprite_builder(btn::sprite_items::gigabat_shadow);
    builder.set_blending_enabled(true);
    builder.set_z_order(constants::footprint_z_order);
    _shadow_sprite = builder.release_build();

    _gigabat_rects.emplace_back(btn::fixed_point(), btn::fixed_size(44, 45));
    _update_sprites(hero_position, false);
    _update_rects();
}

void gigabat_boss::_update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                 enemy_bullets& enemy_bullets)
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
                _movement_index = 1;
                _movement_counter = 100;
                _vibrate = false;
                break;

            case 1:
                _sprites[0].set_tiles(btn::sprite_items::gigabat.tiles_item(), 8);
                _sprites[1].set_tiles(btn::sprite_items::gigabat.tiles_item(), 9);
                _movement_index = 0;
                _movement_counter = 100;
                _vibrate = true;
                break;

            default:
                BTN_ERROR("Invalid movement index: ", _movement_index);
                break;
            }
        }

        switch(_movement_index)
        {

        case 0:
            if(y < -50)
            {
                _gigabat_position.set_y(btn::min(y + 0.25, btn::fixed(-50)));
            }
            else if(y > -50)
            {
                _gigabat_position.set_y(btn::max(y - 0.25, btn::fixed(-50)));
            }

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 1:
            break;

        default:
            BTN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
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
        _update_bullets(hero_position, enemy_bullets);
    }
}

bool gigabat_boss::_update_dead(const btn::fixed_point& hero_position)
{
    bool done = false;
    _gigabat_position.set_y(_gigabat_position.y() + constants::background_speed);

    if(_alive)
    {
        _alive = false;

        for(btn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _palette.set_fade(btn::colors::red, 0);
        _palette_action.emplace(_palette, 15, 0.5);
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

                btn::fixed x = int(_random.get() % 48) - 24 + _gigabat_position.x();
                btn::fixed y = int(_random.get() % 48) - 24 + _gigabat_position.y();
                _mini_explosions.push_back(_create_mini_explosion(x, y));
                btn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(btn::sprite_items::enemy_explosion, btn::fixed_point(0, btn::display::height() / 2),
                                   6, constants::enemy_explosions_z_order, true);
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
        _update_sprites(hero_position, false);
    }

    _update_explosions();
    return done;
}

void gigabat_boss::_show_damage_palette(const btn::sprite_palette_ptr& damage_palette)
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

            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 8));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 24));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16));
            _palette.set_fade(btn::colors::red, 0);
            _palette_action.emplace(_palette, 30, 0.2);
            btn::sound_items::explosion_3.play();
        }
        break;

    case 2:
        if(current_life < total_life - state_0_1_life - state_2_life)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;

            _explosion.emplace(btn::sprite_items::enemy_explosion, btn::fixed_point(x, y), 6,
                               constants::enemy_explosions_z_order, false);
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 8));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 24));
            _palette.set_fade(btn::colors::red, 0);
            _palette_action.emplace(_palette, 25, 0.3);
            btn::sound_items::explosion_1.play();
            btn::sound_items::explosion_3.play();
        }
        break;

    case 3:
        if(current_life < total_life - state_0_1_life - state_2_life - state_2_3_life)
        {
            ++_state_index;
            _bullets_index = 0;
            _bullets_counter = 80;

            _explosion.emplace(btn::sprite_items::enemy_explosion, btn::fixed_point(x, y), 6,
                               constants::enemy_explosions_z_order, false);
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 8));
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 24));
            _palette.set_fade(btn::colors::red, 0);
            _palette_action.emplace(_palette, 20, 0.4);
            btn::sound_items::explosion_1.play();
            btn::sound_items::explosion_3.play();
        }
        break;

    case 4:
        break;

    case 5:
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
                                 const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) const
{
    btn::fixed_point mouth_position = _gigabat_position + btn::fixed_point(0, 10);
    enemy_bullets.add_bullet(hero_position, mouth_position, enemy_bullet_event(bullet_type, delta_position, 1));
}

void gigabat_boss::_update_sprites(const btn::fixed_point& hero_position, bool hero_bomb_closing)
{
    bool flip = hero_position.x() < _gigabat_position.x();
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

    if(hero_bomb_closing)
    {
        _shadow_sprite->set_visible(false);
        _shadow_sprite->set_scale(0.01);
    }
    else
    {
        _shadow_sprite->set_visible(true);
        _shadow_sprite->set_position(_gigabat_position + btn::fixed_point(0, 40));

        btn::fixed scale = _shadow_sprite->scale_x();

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

void gigabat_boss::_update_bullets(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
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
        case 2:
            _bullets_counter = 50;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(enemy_bullet_type::BIG, direction_vector(0, 1, 0.9), hero_position, enemy_bullets);
                _bullets_index = 1;
                break;

            case 1:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position, enemy_bullets);
                _bullets_index = 2;
                break;

            case 2:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.25, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.25, 1, 1), hero_position, enemy_bullets);
                _bullets_index = 3;
                break;

            case 3:
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(-0.5, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, direction_vector(0.5, 1, 1), hero_position, enemy_bullets);
                _bullets_index = 0;
                break;

            default:
                BTN_ERROR("Invalid bullets index: ", _bullets_index);
                break;
            }
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
