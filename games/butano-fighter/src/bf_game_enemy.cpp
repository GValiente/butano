#include "bf_game_enemy.h"

#include "btn_sound.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_affine_mats.h"
#include "bf_game_hero.h"
#include "bf_game_objects.h"
#include "bf_game_enemy_event.h"
#include "bf_game_check_hero_bullet_data.h"

namespace bf::game
{

namespace
{
    constexpr const int damage_frames = 12;

    btn::sprite_ptr _create_sprite(const enemy_event& event)
    {
        const enemy_data& enemy_data = event.enemy;
        btn::sprite_builder builder(enemy_data.sprite_item, enemy_data.graphics_index_1);
        builder.set_position(event.start_position);
        builder.set_z_order(constants::enemies_z_order);
        builder.set_horizontal_flip(event.move_events[0].horizontal_flip);
        return builder.release_build();
    }

    btn::sprite_cached_animate_action<2> _create_animate_action(const btn::sprite_ptr& sprite, const enemy_data& data)
    {
        return btn::create_sprite_cached_animate_action_forever(
                    sprite, 16, data.sprite_item, data.graphics_index_1, data.graphics_index_2);
    }
}

enemy::enemy(const enemy_event& event, const btn::sprite_palette_ptr& damage_palette, int8_t tag) :
    _event(&event),
    _sprite(_create_sprite(event)),
    _move_action(_sprite, event.move_events[0].delta_position),
    _animate_action(_create_animate_action(_sprite, event.enemy)),
    _sprite_palette(_sprite.palette()),
    _damage_palette(damage_palette),
    _life(event.enemy.life),
    _counter(event.move_events[0].duration_frames),
    _move_event_index(0),
    _grid_columns(int8_t(btn::max(event.enemy.dimensions.width().integer() / constants::enemies_grid_size, 1))),
    _grid_rows(int8_t(btn::max(event.enemy.dimensions.height().integer() / constants::enemies_grid_size, + 1))),
    _last_grid_column(0),
    _last_grid_row(0),
    _damage_palette_counter(0),
    _tag(tag)
{
}

bool enemy::check_hero_bullet(const check_hero_bullet_data& data)
{
    if(_life)
    {
        const enemy_data& enemy_data = _event->enemy;
        btn::fixed_rect enemy_rect(position(), enemy_data.dimensions);
        const btn::fixed_rect& bullet_rect = data.bullet_rect;

        if(enemy_rect.intersects(bullet_rect))
        {
            bool show_rotation = btn::sprite_affine_mats::available_count() > constants::reserved_sprite_affine_mats;
            _add_damage(enemy_rect.x(), bullet_rect.x(), data.bullet_damage, show_rotation);

            if(! _life)
            {
                if(data.hero_ref.add_experience(enemy_data.experience))
                {
                    data.objects_ref.spawn_hero_weapon(enemy_rect.position(), data.hero_ref.level() + 1);
                }

                switch(_event->drop)
                {

                case enemy_event::drop_type::NONE:
                    break;

                case enemy_event::drop_type::BOMB:
                    data.objects_ref.spawn_hero_bomb(enemy_rect.position());
                    break;
                }
            }

            return true;
        }
    }

    return false;
}

void enemy::check_hero_bomb(const btn::point& bomb_center, int bomb_squared_radius)
{
    if(_life)
    {
        btn::fixed_point enemy_position = position();
        int distance_x = enemy_position.x().integer() - bomb_center.x();
        int distance_y = enemy_position.y().integer() - bomb_center.y();
        int squared_distance = (distance_x * distance_x) + (distance_y * distance_y);

        if(squared_distance < bomb_squared_radius)
        {
            _add_damage(enemy_position.x(), bomb_center.x(), _life, false);
        }
    }
}

bool enemy::done() const
{
    return _move_event_index == _event->move_events.size();
}

void enemy::update()
{
    BTN_ASSERT(! done(), "Enemy is done");

    --_counter;

    if(_life)
    {
        if(! _counter)
        {
            ++_move_event_index;

            if(done())
            {
                return;
            }

            const enemy_move_event& move_event = _event->move_events[_move_event_index];
            _move_action = btn::sprite_move_by_action(_sprite, move_event.delta_position);
            _counter = move_event.duration_frames;

            if(move_event.horizontal_flip)
            {
                _sprite.set_horizontal_flip(! _sprite.horizontal_flip());
            }
        }

        _move_action.update();
        _animate_action.update();
    }
    else
    {
        if(_counter)
        {
            _rotate_action->update();
            _scale_action->update();
        }
        else
        {
            _move_event_index = int8_t(_event->move_events.size());
        }
    }

    if(_damage_palette_counter)
    {
        --_damage_palette_counter;

        if(_damage_palette_counter == damage_frames / 2)
        {
            _sprite.set_palette(_sprite_palette);
        }
    }
}

void enemy::_add_damage(btn::fixed enemy_x, btn::fixed attack_x, int damage, bool show_rotation)
{
    int life = btn::max(_life - damage, 0);
    _life = int16_t(life);

    if(! _damage_palette_counter)
    {
        _damage_palette_counter = damage_frames;
        _sprite.set_palette(_damage_palette);
    }

    if(! life)
    {
        btn::sound::play(_event->enemy.death_sound_item);

        if(show_rotation)
        {
            btn::fixed rotation_angle = attack_x < enemy_x ? -1 : 1;
            _counter = 30;
            _rotate_action.emplace(_sprite, rotation_angle);
            _rotate_action->update();
            _scale_action.emplace(_sprite, _counter + 1, 0.1);
            _scale_action->update();
        }
        else
        {
            _counter = 1;
        }
    }
}

}
