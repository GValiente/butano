#include "bf_game_butano_boss.h"

#include "btn_colors.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_mini_explosion.h"
#include "btn_sprite_items_enemy_explosion.h"
#include "btn_sprite_items_butano_big_sprite.h"
#include "btn_sprite_items_butano_flash_palette.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr const int state_0_1_life = 600;   // 21 seconds
    constexpr const int state_2_3_life = 400;   // 20 seconds
    constexpr const int state_4_5_life = 500;   // 18 seconds
    constexpr const int total_life = state_0_1_life + state_2_3_life + state_4_5_life;

    constexpr const btn::fixed scale_margin = 0.1;

    [[nodiscard]] btn::sprite_animate_action<7> _create_mini_explosion(btn::fixed x, btn::fixed y)
    {
        btn::sprite_builder builder(btn::sprite_items::mini_explosion);
        builder.set_z_order(constants::enemy_explosions_z_order);
        builder.set_x(x);
        builder.set_y(y);
        return btn::create_sprite_animate_action_once(
                    builder.release_build(), 6, btn::sprite_items::mini_explosion.tiles_item(), 0, 1, 2, 3, 4, 5, 6);
    }
}

butano_boss::butano_boss(const btn::sprite_palette_ptr& damage_palette) :
    boss(total_life, 1000, _butano_rects, damage_palette),
    _butano_position(0, -100),
    _scale(0.2)
{
    _sprites.push_back(btn::sprite_items::butano_big_sprite.create_sprite(0, 0));
    _sprites.push_back(btn::sprite_items::butano_big_sprite.create_sprite(0, 0, 1));

    btn::sprite_palette_ptr palette = _sprites[0].palette();
    palette.set_fade_color(btn::color(10, 1, 8));

    _update_sprites();
    _update_rects();
}

void butano_boss::play_music() const
{
    btn::music_items::basic_instinct.play(0.4);
}

void butano_boss::_update_alive(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                                enemy_bullets& enemy_bullets)
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
        _butano_position.set_y(y - 0.95);
        _scale += 0.02;

        if(_scale >= 1)
        {
            _scale = 1;
            ++_state_index;
        }
        break;

    case 2:
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
        _update_bullets(hero_position, enemy_bullets);
    }
}

bool butano_boss::_update_dead(const btn::fixed_point& hero_position)
{
    bool done = false;

    if(_alive)
    {
        _alive = false;
        _movement_counter = 1;
        _delta_position.set_x((0 - _butano_position.x()) / 240);
        _delta_position.set_y((0 - _butano_position.y()) / 240);

        for(btn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_z_order(constants::intro_sprites_z_order);
        }
    }

    _butano_position += _delta_position;
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

                btn::fixed x = int(_random.get() % 48) - 24 + _butano_position.x();
                btn::fixed y = int(_random.get() % 48) - 24 + _butano_position.y();
                _mini_explosions.push_back(_create_mini_explosion(x, y));
                btn::sound_items::explosion_1.play();
            }
        }
        else if(_state_index == 15)
        {
            if(_mini_explosions.empty())
            {
                ++_state_index;

                _explosion.emplace(btn::sprite_items::enemy_explosion, btn::fixed_point(), 6,
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
        _update_sprites();
    }

    _update_explosions();
    return done;
}

void butano_boss::_show_damage_palette(const btn::sprite_palette_ptr&)
{
    btn::fixed x = _butano_position.x();
    btn::fixed y = _butano_position.y();

    switch(_state_index)
    {

    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    default:
        BTN_ERROR("Invalid state index: ", _state_index);
        break;
    }

    btn::sprite_palette_ptr palette = _sprites[0].palette();
    palette.set_colors(btn::sprite_items::butano_flash_palette.palette_item().colors());
}

void butano_boss::_hide_damage_palette()
{
    btn::sprite_palette_ptr palette = _sprites[0].palette();
    palette.set_colors(btn::sprite_items::butano_big_sprite.palette_item().colors());
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

void butano_boss::_shoot_bullet(enemy_bullet_type bullet_type, const btn::fixed_point& delta_position,
                                const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) const
{
    enemy_bullets.add_bullet(hero_position, _butano_position, enemy_bullet_event(bullet_type, delta_position, 1));
}

void butano_boss::_shoot_random_bullet(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
{
    if(btn::abs(_butano_position.x() - hero_position.x()) < 48 &&
            btn::abs(_butano_position.y() - hero_position.y()) < 48)
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

    btn::fixed_point delta_position = aprox_direction_vector(bullet_x, bullet_y, bullet_speed);
    _shoot_bullet(bullet_type, delta_position, hero_position, enemy_bullets);
}

void butano_boss::_update_sprites()
{
    btn::fixed scale = _scale;
    bool visible = scale <= 2;
    _sprites[0].set_visible(visible);
    _sprites[1].set_visible(visible);

    if(visible)
    {
        btn::fixed x_inc = _vibrate ? 1 : 0;
        btn::fixed y_inc = 32 * scale;
        int z_order = scale < 1 - scale_margin ? constants::footprint_z_order :
                                                 scale < 1 + scale_margin ? constants::enemies_z_order :
                                                                            constants::minimum_z_order;
        bool flipped = _flipped;
        _sprites[0].set_scale(scale);
        _sprites[1].set_scale(scale);
        _sprites[0].set_z_order(z_order);
        _sprites[1].set_z_order(z_order);
        _sprites[0].set_vertical_flip(flipped);
        _sprites[1].set_vertical_flip(flipped);

        if(flipped)
        {
            _sprites[0].set_position(_butano_position + btn::fixed_point(x_inc, y_inc));
            _sprites[1].set_position(_butano_position + btn::fixed_point(x_inc, -y_inc));
        }
        else
        {
            _sprites[0].set_position(_butano_position + btn::fixed_point(x_inc, -y_inc));
            _sprites[1].set_position(_butano_position + btn::fixed_point(x_inc, y_inc));
        }

        btn::fixed fade_intensity = btn::min(1 - scale, btn::fixed(1));
        btn::sprite_palette_ptr palette = _sprites[0].palette();
        palette.set_fade_intensity(fade_intensity);
    }
}

void butano_boss::_update_rects()
{
    btn::fixed scale = _scale;
    _butano_rects.clear();

    if(btn::abs(scale - 1) < scale_margin)
    {
        btn::fixed_point position = _butano_position;
        btn::fixed_point center(position.x(), _flipped ? position.y() - 6 : position.y() + 6);
        _butano_rects.emplace_back(center, btn::fixed_size(58 * scale, 80 * scale));
        _butano_rects.emplace_back(center, btn::fixed_size(47 * scale, 86 * scale));
        _butano_rects.emplace_back(center, btn::fixed_size(36 * scale, 92 * scale));
        _butano_rects.emplace_back(center, btn::fixed_size(25 * scale, 98 * scale));
        _butano_rects.emplace_back(position, btn::fixed_size(10 * scale, 112 * scale));
    }
}

void butano_boss::_update_bullets(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
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
