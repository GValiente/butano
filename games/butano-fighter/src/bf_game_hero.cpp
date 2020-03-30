#include "bf_game_hero.h"

#include "btn_camera.h"
#include "btn_keypad.h"
#include "btn_algorithm.h"
#include "btn_hero_body_sprite_item.h"
#include "btn_hero_weapons_sprite_item.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const int body_delta_y = 40;
    constexpr const int weapon_delta_x = 2;
    constexpr const int weapon_delta_y = -13;

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

void hero::show_shoot(btn::color fade_color)
{
    _show_shoot_counter = 5;

    btn::sprite_palette_ptr body_palette = _body_sprite_animate_action.sprite().palette();
    body_palette.set_fade(fade_color, 0.5);
    _body_palette_fade_action.emplace(btn::move(body_palette), _show_shoot_counter, 0);

    btn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
    weapon_palette.set_fade(fade_color, 0.5);
    _weapon_palette_fade_action.emplace(btn::move(weapon_palette), _show_shoot_counter, 0);
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

void hero::update()
{
    btn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();
    btn::fixed_point body_position = body_sprite.position();
    btn::fixed speed = _is_shooting ? 1 : 2;

    if(btn::keypad::held(btn::keypad::button_type::LEFT))
    {
        btn::fixed sprite_x = btn::max(body_position.x() - speed, btn::fixed(-constants::play_width));
        body_sprite.set_x(sprite_x);
        body_position.set_x(sprite_x);

        if(sprite_x < constants::camera_width)
        {
            btn::camera::set_x(btn::max(btn::camera::x() - speed, btn::fixed(-constants::camera_width)));
        }
    }
    else if(btn::keypad::held(btn::keypad::button_type::RIGHT))
    {
        btn::fixed sprite_x = btn::min(body_position.x() + speed, btn::fixed(constants::play_width));
        body_sprite.set_x(sprite_x);
        body_position.set_x(sprite_x);

        if(sprite_x > -constants::camera_width)
        {
            btn::camera::set_x(btn::min(btn::camera::x() + speed, btn::fixed(constants::camera_width)));
        }
    }

    if(btn::keypad::held(btn::keypad::button_type::UP))
    {
        btn::fixed sprite_y = btn::max(body_position.y() - speed, btn::fixed(-constants::play_height));
        body_sprite.set_y(sprite_y);
        body_position.set_y(sprite_y);
    }
    else if(btn::keypad::held(btn::keypad::button_type::DOWN))
    {
        btn::fixed sprite_y = btn::min(body_position.y() + speed, btn::fixed(constants::play_height));
        body_sprite.set_y(sprite_y);
        body_position.set_y(sprite_y);
    }

    int shoot_shift_y;

    if(_show_shoot_counter)
    {
        shoot_shift_y = -1;
        --_show_shoot_counter;
        _body_palette_fade_action->update();
        _weapon_palette_fade_action->update();
    }
    else
    {
        shoot_shift_y = 0;
    }

    _weapon_position = body_position + btn::fixed_point(weapon_delta_x, weapon_delta_y);
    _weapon_sprite.set_position(_weapon_position + btn::fixed_point(0, shoot_shift_y));

    if(! _is_shooting && body_sprite.position() != body_position)
    {
        _body_sprite_animate_action.update();
    }

    _body_sprite_animate_action.update();
}

}
