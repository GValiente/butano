#include "bf_game_wizard_boss.h"

#include "btn_colors.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_wizard.h"
#include "btn_sprite_items_hero_death.h"
#include "btn_sprite_items_wizard_aura.h"
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
    constexpr const int state_3_4_life = 400;   // 23 seconds
    constexpr const int total_life = state_0_1_life + state_2_life + state_3_4_life;

    constexpr const int mouth_y = 10;
    constexpr const btn::fixed rotate_speed = 0.35;

    [[nodiscard]] btn::sprite_animate_action<5> _create_mini_explosion(btn::fixed x, btn::fixed y)
    {
        btn::sprite_builder builder(btn::sprite_items::mini_explosion_2);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        return btn::create_sprite_animate_action_once(
                    builder.release_build(), 6, btn::sprite_items::mini_explosion_2.tiles_item(), 0, 1, 2, 3, 4);
    }
}

wizard_boss::wizard_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette) :
    boss(total_life, 400, _wizard_rects, damage_palette),
    _wizard_position(0, -160),
    _palette(btn::sprite_items::wizard.palette_item().create_palette())
{
    btn::sprite_builder builder(btn::sprite_items::wizard);
    builder.set_z_order(constants::enemies_z_order);
    _sprites.push_back(builder.release_build());

    builder = btn::sprite_builder(btn::sprite_items::wizard_aura);
    builder.set_blending_enabled(true);
    builder.set_z_order(constants::enemy_explosions_z_order);
    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                btn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, btn::sprite_items::wizard_aura.tiles_item(), 0, 0, 4, 8, 12));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                btn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, btn::sprite_items::wizard_aura.tiles_item(), 0, 1, 5, 9, 13));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                btn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, btn::sprite_items::wizard_aura.tiles_item(), 0, 2, 6, 10, 14));

    _aura_sprites.push_back(builder.build());
    _aura_sprite_animate_actions.push_back(
                btn::create_sprite_animate_action_forever(
                    _aura_sprites.back(), 4, btn::sprite_items::wizard_aura.tiles_item(), 0, 3, 7, 11, 15));

    _wizard_rects.emplace_back(btn::fixed_point(), btn::fixed_size(46, 62));
    _update_sprites(hero_position);
    _update_rects();
}

void wizard_boss::_update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                enemy_bullets& enemy_bullets)
{
    btn::fixed x = _wizard_position.x();
    btn::fixed y = _wizard_position.y();

    switch(_state_index)
    {

    case 0:
        _wizard_position.set_y(y + constants::background_speed);

        if(y >= -46)
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

                _delta_position = (btn::fixed_point(x, -16) - _wizard_position) / _movement_counter;
                _animate_actions.clear();
                _animate_actions.push_back(btn::create_sprite_animate_action_forever(_sprites[0], 8,
                                           btn::sprite_items::wizard.tiles_item(), 8, 9, 10, 11, 12, 13));
                break;

            case 1:
                ++_movement_index;
                _movement_counter = 120;

                _animate_actions.clear();
                _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), 14);
                break;

            case 2:
                _movement_index = 0;
                _movement_counter = 240;

                _target_x = 46;
                _animate_actions.clear();
                _animate_actions.push_back(btn::create_sprite_animate_action_forever(_sprites[0], 6,
                                           btn::sprite_items::wizard.tiles_item(), 1, 4, 5, 1, 6, 7));
                break;

            default:
                BTN_ERROR("Invalid movement index: ", _movement_index);
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
                    _target_x = -46;
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
                    _target_x = 46;
                }
            }
            break;

        case 1:
            _wizard_position += _delta_position;
            break;

        case 2:
            if(y > -46)
            {
                _wizard_position.set_y(btn::max(y - constants::background_speed, btn::fixed(-46)));
            }
            else
            {
                _movement_counter = 1;
            }
            break;

        default:
            BTN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 2:
        --_movement_counter;

        if(! _movement_counter)
        {
            switch(_movement_index)
            {

            case 0:
                _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), 10);
                _sprites[1].set_tiles(btn::sprite_items::wizard.tiles_item(), 11);
                _target_x = hero_position.x();
                _movement_index = 1;
                _movement_counter = 80;
                _vibrate = false;
                _delta_position = (hero_position - _wizard_position) / _movement_counter;
                break;

            case 1:
                _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), 8);
                _sprites[1].set_tiles(btn::sprite_items::wizard.tiles_item(), 9);
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
                _wizard_position.set_y(btn::min(y + 0.5, btn::fixed(-56)));
            }
            else if(y > -56)
            {
                _wizard_position.set_y(btn::max(y - 0.5, btn::fixed(-56)));
            }

            if(_movement_counter % 4 == 0)
            {
                _vibrate = ! _vibrate;
            }
            break;

        case 1:
            _wizard_position += _delta_position;
            break;

        default:
            BTN_ERROR("Invalid movement index: ", _movement_index);
            break;
        }
        break;

    case 3:
        if(y < -34)
        {
            _wizard_position.set_y(btn::min(y + 0.25, btn::fixed(-34)));
        }
        else if(y > -34)
        {
            _wizard_position.set_y(btn::max(y - 0.25, btn::fixed(-34)));
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
            _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), base_graphics_index);
            _sprites[1].set_tiles(btn::sprite_items::wizard.tiles_item(), base_graphics_index + 1);
            _sprites[2].set_tiles(btn::sprite_items::wizard.tiles_item(), base_graphics_index + 2);
            _sprites[3].set_tiles(btn::sprite_items::wizard.tiles_item(), base_graphics_index + 3);
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

        if(y <= -constants::play_height - 16)
        {
            _delta_position.set_y(rotate_speed);
        }
        else if(y >= constants::play_height - 24)
        {
            _delta_position.set_y(-rotate_speed);
        }

        _wizard_position += _delta_position;
        break;

    default:
        BTN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    _update_sprites(hero_position);
    _update_explosions();
    _update_rects();

    if(! hero_bomb.active())
    {
        _update_bullets(hero_position, enemy_bullets);
    }
}

bool wizard_boss::_update_dead(const btn::fixed_point& hero_position)
{
    bool done = false;

    if(_sprites.size() == 4)
    {
        _movement_counter = 1;
        _delta_position.set_x((0 - _wizard_position.x()) / 240);
        _delta_position.set_y((0 - _wizard_position.y()) / 240);

        _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), 28);
        _sprites[1].set_tiles(btn::sprite_items::wizard.tiles_item(), 29);
        _sprites.pop_back();
        _sprites.pop_back();

        for(btn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }

        _palette.set_fade(btn::colors::red, 0);
        _palette_action.emplace(_palette, 15, 0.6);
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

                btn::fixed x = int(_random.get() % 48) - 24 + _wizard_position.x();
                btn::fixed y = int(_random.get() % 48) - 24 + _wizard_position.y();
                _mini_explosions.push_back(_create_mini_explosion(x, y));
                btn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(btn::sprite_items::hero_death, btn::fixed_point(), 6,
                                   constants::enemy_explosions_z_order, true);
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
                    _aura_sprites.clear();
                    _aura_sprite_animate_actions.clear();
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
        _update_sprites(hero_position);
    }

    _update_explosions();
    return done;
}

void wizard_boss::_show_damage_palette(const btn::sprite_palette_ptr& damage_palette)
{
    btn::fixed x = _wizard_position.x();
    btn::fixed y = _wizard_position.y();
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

            _sprites[0].set_tiles(btn::sprite_items::wizard.tiles_item(), 12);
            _sprites[1].set_tiles(btn::sprite_items::wizard.tiles_item(), 13);

            btn::sprite_builder builder(btn::sprite_items::wizard, 14);
            builder.set_z_order(constants::enemies_z_order);
            _sprites.push_back(builder.release_build());

            builder = btn::sprite_builder(btn::sprite_items::wizard, 15);
            builder.set_z_order(constants::enemies_z_order);
            _sprites.push_back(builder.release_build());

            _explosion.emplace(btn::sprite_items::hero_death, btn::fixed_point(x, y), 6,
                               constants::enemy_explosions_z_order, false);
            _mini_explosions.push_back(_create_mini_explosion(x + 24, y + 8));
            _mini_explosions.push_back(_create_mini_explosion(x - 24, y + 24));
            _mini_explosions.push_back(_create_mini_explosion(x, y - 16));
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

void wizard_boss::_hide_damage_palette()
{
    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_palette(_palette);
    }
}

bool wizard_boss::_hero_should_look_down_impl(const btn::fixed_point& hero_position, bool hero_is_looking_down) const
{
    if(_sprites.empty())
    {
        return false;
    }

    btn::fixed y = _wizard_position.y();

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

void wizard_boss::_shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                                 const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) const
{
    btn::fixed_point mouth_position = _wizard_position + btn::fixed_point(0, mouth_y);
    enemy_bullets.add_bullet(hero_position, mouth_position, enemy_bullet_event(bullet_type, delta_position, 1));
}

void wizard_boss::_shoot_target_random_bullet(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
{
    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    btn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? 0.9 : 1.0;

    if(_random.get() % 8 == 0)
    {
        btn::fixed_point mouth_position = _wizard_position + btn::fixed_point(0, mouth_y);
        btn::fixed_point distance = hero_position - mouth_position;

        if(distance == btn::fixed_point())
        {
            distance.set_y(1);
        }

        btn::fixed_point delta_position = aprox_direction_vector(distance.x(), distance.y(), bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, enemy_bullets);
    }
    else
    {
        btn::fixed bullet_x = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;
        btn::fixed bullet_y = btn::fixed::from_data(int(_random.get() % btn::fixed(1).data()));

        if(bullet_x == 0 && bullet_y == 0)
        {
            bullet_y = 1;
        }

        btn::fixed_point delta_position = aprox_direction_vector(bullet_x, bullet_y, bullet_speed);
        _shoot_bullet(bullet_type, delta_position, hero_position, enemy_bullets);
    }
}

void wizard_boss::_shoot_free_random_bullet(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
{
    enemy_bullet_type bullet_type = _random.get() % 8 == 0 ? enemy_bullet_type::BIG : enemy_bullet_type::SMALL;
    btn::fixed bullet_speed = bullet_type == enemy_bullet_type::BIG ? 0.9 : 1.0;
    btn::fixed bullet_x = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;
    btn::fixed bullet_y = btn::fixed::from_data(int(_random.get() % btn::fixed(2).data())) - 1;

    if(bullet_x == 0 && bullet_y == 0)
    {
        bullet_y = 1;
    }

    btn::fixed_point delta_position = aprox_direction_vector(bullet_x, bullet_y, bullet_speed);
    _shoot_bullet(bullet_type, delta_position, hero_position, enemy_bullets);
}

void wizard_boss::_update_sprites(const btn::fixed_point& hero_position)
{
    btn::fixed target_x = _target_x.value_or(hero_position.x());
    bool flip = target_x < _wizard_position.x();
    btn::fixed y_inc = _vibrate ? 1 : 0;

    if(_sprites.size() == 2)
    {
        if(life())
        {
            btn::fixed x1_inc = flip ? 32 : -32;
            btn::fixed x2_inc = flip ? -32 : 32;
            _sprites[0].set_position(_wizard_position + btn::fixed_point(x1_inc, y_inc));
            _sprites[1].set_position(_wizard_position + btn::fixed_point(x2_inc, y_inc));
        }
        else
        {
            _sprites[0].set_position(_wizard_position + btn::fixed_point(0, y_inc));
            _sprites[1].set_position(_wizard_position + btn::fixed_point(0, y_inc));
        }
    }
    else
    {
        _sprites[0].set_position(_wizard_position + btn::fixed_point(0, y_inc));
    }

    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_horizontal_flip(flip);
    }

    for(btn::sprite_animate_action<6>& animate_action : _animate_actions)
    {
        animate_action.update();
    }

    bool aura_visible = ! _aura_sprites[0].visible() && ! _death_flash();
    _aura_sprites[0].set_visible(aura_visible);
    _aura_sprites[0].set_position(_wizard_position + btn::fixed_point(-32, -32 - 4));
    _aura_sprites[1].set_visible(aura_visible);
    _aura_sprites[1].set_position(_wizard_position + btn::fixed_point(32, -32 - 4));
    _aura_sprites[2].set_visible(aura_visible);
    _aura_sprites[2].set_position(_wizard_position + btn::fixed_point(-32, 32 - 4));
    _aura_sprites[3].set_visible(aura_visible);
    _aura_sprites[3].set_position(_wizard_position + btn::fixed_point(32, 32 - 4));

    for(btn::sprite_animate_action<5>& aura_sprite_animate_action : _aura_sprite_animate_actions)
    {
        aura_sprite_animate_action.update();
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

void wizard_boss::_update_bullets(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
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
            _bullets_counter = 40;

            switch(_bullets_index)
            {

            case 0:
                _shoot_bullet(enemy_bullet_type::BIG, aprox_direction_vector(0, 1, 0.9), hero_position, enemy_bullets);
                _bullets_index = 1;
                break;

            case 1:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.5, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.5, 1, 1), hero_position, enemy_bullets);
                _bullets_index = 2;
                break;

            case 2:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.25, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.25, 1, 1), hero_position, enemy_bullets);
                _bullets_index = 3;
                break;

            case 3:
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(-0.5, 1, 1), hero_position, enemy_bullets);
                _shoot_bullet(enemy_bullet_type::SMALL, aprox_direction_vector(0.5, 1, 1), hero_position, enemy_bullets);
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
                    _shoot_target_random_bullet(hero_position, enemy_bullets);
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
            _shoot_free_random_bullet(hero_position, enemy_bullets);
            break;

        default:
            BTN_ERROR("Invalid state index: ", _state_index);
            break;
        }
    }
}

void wizard_boss::_update_explosions()
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
