#include "bf_game_enemy_bullets.h"

#include "btn_colors.h"
#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_affine_mats.h"
#include "btn_sprite_items_enemy_bullets.h"
#include "bf_game_bullet_util.h"
#include "bf_game_enemy_bullet_event.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed_size dimensions[3] = {
        btn::fixed_size(8, 8),
        btn::fixed_size(14, 14),
        btn::fixed_size(28, 28)
    };

    [[nodiscard]] btn::sprite_palette_fade_loop_action _create_palette_fade_action()
    {
        btn::sprite_palette_ptr palette = btn::sprite_items::enemy_bullets.palette_item().create_palette();
        palette.set_fade_color(btn::colors::red);
        return btn::sprite_palette_fade_loop_action(btn::move(palette), 15, 0.5);
    }
}

enemy_bullets::enemy_bullets() :
    _palette_fade_action(_create_palette_fade_action()),
    _tiles_list({ btn::sprite_items::enemy_bullets.tiles_item().create_tiles(0),
           btn::sprite_items::enemy_bullets.tiles_item().create_tiles(1) })
{
}

bool enemy_bullets::check_hero(const btn::fixed_rect& hero_rect)
{
    btn::iforward_list<bullet_type>* bullets = _check_odds ? &_odd_bullets : &_even_bullets;

    for(const bullet_type& bullet : *bullets)
    {
        const btn::fixed_point& bullet_position = bullet.sprite_move_action.sprite().position();
        btn::fixed_rect bullet_rect(bullet_position, dimensions[int(bullet.type)]);

        if(bullet_rect.intersects(hero_rect))
        {
            return true;
        }
    }

    return false;
}

void enemy_bullets::add_bullet(const btn::fixed_point& hero_position, const btn::fixed_point& enemy_position,
                               const enemy_bullet_event& event)
{
    enemy_bullet_type type = event.type;

    switch(type)
    {

    case enemy_bullet_type::SMALL:
    case enemy_bullet_type::BIG:
        break;

    case enemy_bullet_type::HUGE:
        if(btn::sprite_affine_mats::available_count() <= constants::reserved_sprite_affine_mats)
        {
            type = enemy_bullet_type::BIG;
        }
        break;

    case enemy_bullet_type::ROBOT_DOUBLE:
        {
            enemy_bullet_event single_event = event;
            single_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + btn::fixed_point(-18, 20), single_event);
            add_bullet(hero_position, enemy_position + btn::fixed_point(18, 20), single_event);
        }
        return;

    case enemy_bullet_type::CAVEMAN_SMALL:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::SMALL;
            add_bullet(hero_position, enemy_position + btn::fixed_point(24, 8), hand_event);
        }
        return;

    case enemy_bullet_type::CAVEMAN_BIG:
        {
            enemy_bullet_event hand_event = event;
            hand_event.type = enemy_bullet_type::BIG;
            add_bullet(hero_position, enemy_position + btn::fixed_point(24, 8), hand_event);
        }
        return;

    default:
        BTN_ERROR("Invalid bullet type: ", int(type));
        break;
    }

    btn::fixed scale = 1;
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

    btn::iforward_list<bullet_type>* bullets = _odd_bullets.size() < _even_bullets.size() ?
                &_odd_bullets : &_even_bullets;
    BTN_ASSERT(! bullets->full(), "No more space for enemy bullets");

    btn::sprite_builder builder(btn::sprite_items::enemy_bullets.shape_size(), _tiles_list[tile_index],
                                _palette_fade_action.palette());
    builder.set_position(enemy_position);
    builder.set_scale(scale);
    builder.set_z_order(constants::enemy_bullets_z_order);

    if(event.delta_speed > 0)
    {
        btn::fixed_point distance = hero_position - enemy_position;

        if(distance == btn::fixed_point())
        {
            distance.set_y(1);
        }

        btn::fixed_point delta_position = aprox_direction_vector(distance.x(), distance.y(), event.delta_speed);
        bullets->push_front({ btn::sprite_move_by_action(builder.release_build(), delta_position),
                             btn::nullopt, type });
    }
    else
    {
        bullets->push_front({ btn::sprite_move_by_action(builder.release_build(), event.delta_position),
                             btn::nullopt, type });
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
    btn::iforward_list<bullet_type>* check_and_update_bullets;
    btn::iforward_list<bullet_type>* update_bullets;

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
        btn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
        const btn::fixed_point& position = sprite_move_action.sprite().position();

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
