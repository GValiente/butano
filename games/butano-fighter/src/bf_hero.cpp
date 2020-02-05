#include "bf_hero.h"

#include "btn_camera.h"
#include "btn_keypad.h"
#include "btn_algorithm.h"
#include "btn_hero_body_sprite_item.h"
#include "btn_hero_weapons_sprite_item.h"
#include "bf_constants.h"

namespace bf
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

    btn::sprite_ptr _build_weapon_sprite(int level)
    {
        return btn::sprite_ptr::create(weapon_delta_x, body_delta_y + weapon_delta_y,
                                       btn::sprite_items::hero_weapons, level);
    }
}

hero::hero() :
    _body_sprite_animate_action(_build_body_sprite_animate_action()),
    _weapon_sprite(_build_weapon_sprite(_level))
{
}

void hero::update()
{
    btn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();
    _body_sprite_animate_action.update();

    if(btn::keypad::held(btn::keypad::button_type::LEFT))
    {
        btn::fixed sprite_x = btn::max(body_sprite.x() - 1, btn::fixed(-constants::play_width));
        body_sprite.set_x(sprite_x);
        _weapon_sprite.set_x(sprite_x + weapon_delta_x);

        if(sprite_x < constants::camera_width)
        {
            btn::camera::set_x(btn::max(btn::camera::x() - 1, btn::fixed(-constants::camera_width)));
        }
    }
    else if(btn::keypad::held(btn::keypad::button_type::RIGHT))
    {
        btn::fixed sprite_x = btn::min(body_sprite.x() + 1, btn::fixed(constants::play_width));
        body_sprite.set_x(sprite_x);
        _weapon_sprite.set_x(sprite_x + weapon_delta_x);

        if(sprite_x > -constants::camera_width)
        {
            btn::camera::set_x(btn::min(btn::camera::x() + 1, btn::fixed(constants::camera_width)));
        }
    }

    if(btn::keypad::held(btn::keypad::button_type::UP))
    {
        btn::fixed sprite_y = btn::max(body_sprite.y() - 1, btn::fixed(-constants::play_height));
        body_sprite.set_y(sprite_y);
        _weapon_sprite.set_y(sprite_y + weapon_delta_y);
    }
    else if(btn::keypad::held(btn::keypad::button_type::DOWN))
    {
        btn::fixed sprite_y = btn::min(body_sprite.y() + 1, btn::fixed(constants::play_height));
        body_sprite.set_y(sprite_y);
        _weapon_sprite.set_y(sprite_y + weapon_delta_y);
    }
}

}
