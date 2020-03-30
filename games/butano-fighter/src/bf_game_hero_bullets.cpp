#include "bf_game_hero_bullets.h"

#include "btn_math.h"
#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_hero_bullets_sprite_item.h"
#include "bf_constants.h"
#include "bf_game_hero.h"
#include "bf_game_hero_bullet_event.h"

namespace bf::game
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

    struct level_data
    {
        btn::span<const hero_bullet_event> events;
        btn::sound_item sound_item;
        btn::color color;
        int8_t max_counter;
    };

    constexpr const hero_bullet_event _level0_events[] = {
        hero_bullet_event(_direction_vector(0, -1), 1,  0,  true),
    };

    constexpr const hero_bullet_event _level1_events[] = {
        hero_bullet_event(_direction_vector(0, -1),         1,  1,  true),
        hero_bullet_event(_direction_vector(-0.075, -1),    11, 0,  true),
        hero_bullet_event(_direction_vector(0.075, -1),     31, 0,  true),
    };

    constexpr const hero_bullet_event _level2_events[] = {
        hero_bullet_event(_direction_vector(0, -1, 0.7),    1,  2,  true),
        hero_bullet_event(_direction_vector(-0.085, -1),    11, 0,  false),
        hero_bullet_event(_direction_vector(0.05, -1),      21, 1,  true),
        hero_bullet_event(_direction_vector(0.085, -1),     31, 0,  false),

        hero_bullet_event(_direction_vector(0, -1, 0.7),    41, 2,  true),
        hero_bullet_event(_direction_vector(-0.085, -1),    51, 0,  false),
        hero_bullet_event(_direction_vector(-0.05, -1),     61, 1,  true),
        hero_bullet_event(_direction_vector(0.085, -1),     71, 0,  false),
    };

    constexpr const level_data _levels_data[] = {
        { _level0_events, btn::sound_items::gun_5,  btn::color(31, 28, 5),  20 },
        { _level1_events, btn::sound_items::gun_5,  btn::color(31, 28, 5),  40 },
        { _level2_events, btn::sound_items::gun_1,  btn::color(13, 2, 2),   80 },
    };
}

void hero_bullets::update(hero& hero)
{
    if(btn::keypad::held(btn::keypad::button_type::B))
    {
        _b_held_counter = 10;
    }
    else if(_b_held_counter)
    {
        --_b_held_counter;
    }

    _remove_bullets();
    hero.set_is_shooting(_b_held_counter);

    if(_event_counter || _b_held_counter)
    {
        ++_event_counter;
    }

    if(_b_held_counter)
    {
        _add_bullets(hero);
    }

    if(_event_counter == _levels_data[hero.level()].max_counter)
    {
        _event_counter = 0;
    }
}

void hero_bullets::_remove_bullets()
{
    int bullets_count = _sprite_move_actions.size();

    for(int index = 0; index < bullets_count; )
    {
        btn::sprite_move_by_action& sprite_move_action = _sprite_move_actions[index];
        const btn::fixed_point& position = sprite_move_action.sprite().position();

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height)
        {
            if(index < bullets_count - 1)
            {
                btn::swap(sprite_move_action, _sprite_move_actions[bullets_count - 1]);
            }

            --bullets_count;
        }
        else
        {
            sprite_move_action.update();
            ++index;
        }
    }

    _sprite_move_actions.shrink(bullets_count);
}

void hero_bullets::_add_bullets(hero& hero)
{
    int counter = _event_counter;

    for(const hero_bullet_event& event : _levels_data[hero.level()].events)
    {
        if(counter == event.frame)
        {
            BTN_ASSERT(! _sprite_move_actions.full(), "No more space for sprite bullets");

            int event_level = event.level;
            const level_data& shoot_level_data = _levels_data[event_level];
            btn::sprite_builder builder(btn::sprite_items::hero_bullets, event_level);
            builder.set_position(hero.weapon_position());
            builder.set_z_order(constants::hero_bullets_z_order);
            _sprite_move_actions.push_back(btn::sprite_move_by_action(builder.release_build(), event.direction));
            hero.show_shoot(shoot_level_data.color);

            if(event.play_sound)
            {
                btn::sound::play(shoot_level_data.sound_item, 0.5);
            }

            break;
        }
    }
}

}
