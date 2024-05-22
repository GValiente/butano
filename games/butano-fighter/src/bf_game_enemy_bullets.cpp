/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_enemy_bullets.h"

#include "bn_colors.h"
#include "bn_fixed_rect.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_affine_mats.h"
#include "bn_sprite_items_enemy_bullets.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr bn::fixed_size dimensions[3] = {
        bn::fixed_size(8, 8),
        bn::fixed_size(14, 14),
        bn::fixed_size(28, 28)
    };

    [[nodiscard]] bn::sprite_palette_fade_loop_action _create_palette_fade_action()
    {
        bn::sprite_palette_ptr palette = bn::sprite_items::enemy_bullets.palette_item().create_palette();
        palette.set_fade_color(bn::colors::red);
        return bn::sprite_palette_fade_loop_action(bn::move(palette), 15, 0.5);
    }
}

enemy_bullets::enemy_bullets() :
    _palette_fade_action(_create_palette_fade_action()),
    _tiles_list({ bn::sprite_items::enemy_bullets.tiles_item().create_tiles(0),
           bn::sprite_items::enemy_bullets.tiles_item().create_tiles(1) })
{
}

bool enemy_bullets::check_hero(const bn::fixed_rect& hero_rect)
{
    bn::iforward_list<bullet_type>* bullets = _check_odds ? &_odd_bullets : &_even_bullets;

    for(const bullet_type& bullet : *bullets)
    {
        const bn::fixed_point& bullet_position = bullet.sprite_move_action.sprite().position();
        bn::fixed_rect bullet_rect(bullet_position, dimensions[int(bullet.type)]);

        if(bullet_rect.intersects(hero_rect))
        {
            return true;
        }
    }

    return false;
}

void enemy_bullets::add_bullet(const bn::fixed_point& hero_position, const bn::fixed_point& enemy_position,
                               const enemy_bullet_event& event, const bn::camera_ptr& camera)
{
    enemy_bullet_type type = event.type;

    switch(type)
    {

    case enemy_bullet_type::SMALL:
    case enemy_bullet_type::BIG:
        break;

    case enemy_bullet_type::HUGE:
        if(bn::sprite_affine_mats::available_count() <= constants::reserved_sprite_affine_mats)
        {
            type = enemy_bullet_type::BIG;
        }
        break;

    case enemy_bullet_type::ROBOT_SMALL:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-18, 20), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(18, 20), single_event, camera);
        }
        return;

    case enemy_bullet_type::CAVEMAN_SMALL:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(24, 8), hand_event, camera);
        }
        return;

    case enemy_bullet_type::CAVEMAN_BIG:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + bn::fixed_point(24, 8), hand_event, camera);
        }
        return;

    case enemy_bullet_type::CAVEMAN_FLIPPED_SMALL:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-24, 8), hand_event, camera);
        }
        return;

    case enemy_bullet_type::CAVEMAN_FLIPPED_BIG:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-24, 8), hand_event, camera);
        }
        return;

    case enemy_bullet_type::ASTRONAUT_SMALL:
        {
            enemy_bullet_event shotgun_event = event;
            shotgun_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-3, 12), shotgun_event, camera);
        }
        return;

    case enemy_bullet_type::ASTRONAUT_BIG:
        {
            enemy_bullet_event shotgun_event = event;
            shotgun_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-3, 12), shotgun_event, camera);
        }
        return;

    case enemy_bullet_type::YELLOW_SPACESHIP_SMALL:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-14, 14), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(14, 14), single_event, camera);
        }
        return;

    case enemy_bullet_type::GREEN_SPACESHIP_SMALL:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-15, 22), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(15, 22), single_event, camera);
        }
        return;

    case enemy_bullet_type::BRAIN_SMALL:
        {
            enemy_bullet_event mouth_event = event;
            mouth_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(0, 23), mouth_event, camera);
        }
        return;

    case enemy_bullet_type::BRAIN_BIG:
        {
            enemy_bullet_event mouth_event = event;
            mouth_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + bn::fixed_point(0, 23), mouth_event, camera);
        }
        return;

    case enemy_bullet_type::BRAIN_HUGE:
        {
            enemy_bullet_event mouth_event = event;
            mouth_event.type = enemy_bullet_type::HUGE;
            add_bullet(hero_position, enemy_position + bn::fixed_point(0, 23), mouth_event, camera);
        }
        return;

    case enemy_bullet_type::RED_SPACESHIP_BIG:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-11, 17), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(11, 17), single_event, camera);
        }
        return;

    case enemy_bullet_type::BLUE_SPACESHIP_SMALL:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(25, -12), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(25, 12), single_event, camera);
        }
        return;

    case enemy_bullet_type::BLUE_SPACESHIP_FLIPPED_SMALL:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + bn::fixed_point(-25, -12), single_event, camera);
            add_bullet(hero_position, enemy_position + bn::fixed_point(-25, 12), single_event, camera);
        }
        return;

    default:
        BN_ERROR("Invalid bullet type: ", int(type));
        break;
    }

    bn::fixed scale = 1;
    int tile_index;

    if(type == enemy_bullet_type::HUGE)
    {
        scale = 2;
        tile_index = 1;
    }
    else
    {
        tile_index = int(type);
    }

    bn::iforward_list<bullet_type>* bullets = _odd_bullets.size() < _even_bullets.size() ?
                &_odd_bullets : &_even_bullets;
    BN_ASSERT(! bullets->full(), "No more space for enemy bullets");

    bn::sprite_builder builder(bn::sprite_items::enemy_bullets.shape_size(), _tiles_list[tile_index],
                                _palette_fade_action.palette());
    builder.set_position(enemy_position);
    builder.set_scale(scale);
    builder.set_z_order(constants::enemy_bullets_z_order);
    builder.set_camera(camera);

    if(event.delta_speed > 0)
    {
        bn::fixed_point distance = hero_position - enemy_position;

        if(distance == bn::fixed_point())
        {
            distance.set_y(1);
        }

        bn::fixed_point delta_position = direction_vector(distance.x(), distance.y(), event.delta_speed);
        bullets->push_front({ bn::sprite_move_by_action(builder.release_build(), delta_position),
                             bn::nullopt, type });
    }
    else
    {
        bullets->push_front({ bn::sprite_move_by_action(builder.release_build(), event.delta_position),
                             bn::nullopt, type });
    }

    if(type == enemy_bullet_type::HUGE)
    {
        bullet_type& bullet = bullets->front();
        bullet.sprite_rotate_action.emplace(bullet.sprite_move_action.sprite(), 4);
    }
}

void enemy_bullets::clear()
{
    _even_bullets.clear();
    _odd_bullets.clear();
}

void enemy_bullets::update()
{
    bn::iforward_list<bullet_type>* check_and_update_bullets;
    bn::iforward_list<bullet_type>* update_bullets;

    if(_check_odds)
    {
        check_and_update_bullets = &_odd_bullets;
        update_bullets = &_even_bullets;
        _check_odds = false;
    }
    else
    {
        check_and_update_bullets = &_even_bullets;
        update_bullets = &_odd_bullets;
        _check_odds = true;
    }

    auto before_it = check_and_update_bullets->before_begin();
    auto it = check_and_update_bullets->begin();
    auto end = check_and_update_bullets->end();
    _palette_fade_action.update();

    while(it != end)
    {
        bullet_type& bullet = *it;
        bn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
        const bn::fixed_point& position = sprite_move_action.sprite().position();

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height)
        {
            it = check_and_update_bullets->erase_after(before_it);
        }
        else
        {
            sprite_move_action.update();

            if(bullet.sprite_rotate_action)
            {
                bullet.sprite_rotate_action->update();
            }

            before_it = it;
            ++it;
        }
    }

    for(bullet_type& bullet : *update_bullets)
    {
        bullet.sprite_move_action.update();

        if(bullet.sprite_rotate_action)
        {
            bullet.sprite_rotate_action->update();
        }
    }
}

}
