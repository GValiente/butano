#include "bf_hero_bullets.h"

#include "btn_math.h"
#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_hero_bullets_sprite_item.h"
#include "bf_hero.h"
#include "bf_constants.h"

namespace bf
{

namespace
{
    constexpr btn::fixed_point _unit_vector(btn::fixed x, btn::fixed y)
    {
        btn::fixed magnitude = btn::newton_raphson_sqrt((x * x) + (y * y));
        return btn::fixed_point(x, y) / magnitude;
    }

    constexpr btn::fixed_point _direction_vector(btn::fixed x, btn::fixed y, btn::fixed speed = 1)
    {
        return _unit_vector(x, y) * speed * btn::fixed(2.5);
    }
}

void hero_bullets::update(const hero& hero)
{
    bool b_held = btn::keypad::held(btn::keypad::button_type::B);
    _remove_bullets();

    if(_counter || b_held)
    {
        ++_counter;
    }

    if(b_held)
    {
        _add_bullets(hero);
    }

    _reset_counter(hero);
}

void hero_bullets::_remove_bullets()
{
    int remove_count = 0;

    for(size_t index = 0, size = _sprite_move_actions.size(); index < size; ++index)
    {
        btn::sprite_move_by_action& sprite_move_action = _sprite_move_actions[index];
        const btn::fixed_point& position = sprite_move_action.sprite().position();

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height)
        {
            if(index < size - 1)
            {
                swap(sprite_move_action, _sprite_move_actions[size - 1]);
                --index;
                --size;
            }

            ++remove_count;
        }
        else
        {
            sprite_move_action.update();
        }
    }

    while(remove_count)
    {
        _sprite_move_actions.pop_back();
        --remove_count;
    }
}

void hero_bullets::_add_bullets(const hero& hero)
{
    switch(hero.level())
    {

    case 0:
        if(_counter == 1)
        {
            constexpr btn::fixed_point direction = _direction_vector(0, -1);
            _add_bullet(0, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        break;

    case 1:
        if(_counter == 1)
        {
            constexpr btn::fixed_point direction = _direction_vector(0, -1);
            _add_bullet(1, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        else if(_counter == 11)
        {
            constexpr btn::fixed_point direction = _direction_vector(-0.075, -1);
            _add_bullet(0, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        else if(_counter == 31)
        {
            constexpr btn::fixed_point direction = _direction_vector(0.075, -1);
            _add_bullet(0, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        break;

    case 2:
        if(_counter == 1)
        {
            constexpr btn::fixed_point direction = _direction_vector(0.05, -1);
            _add_bullet(1, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        else if(_counter == 11 || _counter == 51)
        {
            constexpr btn::fixed_point direction = _direction_vector(-0.085, -1);
            _add_bullet(0, hero.weapon_position(), direction);
        }
        else if(_counter == 21 || _counter == 61)
        {
            constexpr btn::fixed_point direction = _direction_vector(0, -1, 0.7);
            _add_bullet(2, hero.weapon_position(), direction, btn::sound_items::gun_1);
        }
        else if(_counter == 31 || _counter == 71)
        {
            constexpr btn::fixed_point direction = _direction_vector(0.085, -1);
            _add_bullet(0, hero.weapon_position(), direction);
        }
        else if(_counter == 41)
        {
            constexpr btn::fixed_point direction = _direction_vector(-0.05, -1);
            _add_bullet(1, hero.weapon_position(), direction, btn::sound_items::gun_5);
        }
        break;
    }
}

void hero_bullets::_add_bullet(int level, const btn::fixed_point& position, const btn::fixed_point& direction,
                               btn::optional<btn::sound_item> sound_item)
{
    BTN_ASSERT(! _sprite_move_actions.full(), "No more space for sprite bullets");

    btn::sprite_builder builder(btn::sprite_items::hero_bullets, level);
    builder.set_position(position);
    builder.set_z_order(constants::hero_bullets_z_order);
    _sprite_move_actions.push_back(btn::sprite_move_by_action(builder.build_and_release(), direction));

    if(sound_item)
    {
        btn::sound::play(*sound_item, 0.5);
    }
}

void hero_bullets::_reset_counter(const hero& hero)
{
    int level = hero.level();

    if((level == 0 && _counter == 20) || (level == 1 && _counter == 40) || (level == 2 && _counter == 80))
    {
        _counter = 0;
    }
}

}
