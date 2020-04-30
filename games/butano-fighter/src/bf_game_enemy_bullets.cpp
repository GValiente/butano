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
    auto bullets_it = _bullets.begin();
    auto bullets_end = _bullets.end();
    _hero_check_odds = ! _hero_check_odds;

    if(_hero_check_odds)
    {
        ++bullets_it;
    }

    while(bullets_it < bullets_end)
    {
        const bullet& bullet = *bullets_it;
        const btn::fixed_point& bullet_position = bullet.sprite_move_action.sprite().position();
        btn::fixed_rect bullet_rect(bullet_position, dimensions[int(bullet.type)]);

        if(bullet_rect.intersects(hero_rect))
        {
            return true;
        }

        bullets_it += 2;
    }

    return false;
}

void enemy_bullets::add_bullet(const btn::fixed_point& hero_position, const btn::fixed_point& enemy_position,
                               const enemy_bullet_event& event)
{
    enemy_bullet_type type = event.type;

    if(type == enemy_bullet_type::HUGE &&
            btn::sprite_affine_mats::available_count() <= constants::reserved_sprite_affine_mats)
    {
        type = enemy_bullet_type::BIG;
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

    btn::sprite_builder builder(btn::sprite_items::enemy_bullets.shape_size(), _tiles_list[tile_index],
                                _palette_fade_action.palette());
    builder.set_position(enemy_position);
    builder.set_scale(scale);
    builder.set_z_order(constants::enemy_bullets_z_order);

    if(event.delta_speed > 0)
    {
        btn::fixed_point distance = hero_position - enemy_position;
        btn::fixed_point delta_position = aprox_unit_vector(distance.x(), distance.y());
        _bullets.push_back({ btn::sprite_move_by_action(builder.release_build(), delta_position),
                             btn::nullopt, type });
    }
    else
    {
        _bullets.push_back({ btn::sprite_move_by_action(builder.release_build(), event.delta_position),
                             btn::nullopt, type });
    }

    if(type == enemy_bullet_type::HUGE)
    {
        bullet& bullet = _bullets.back();
        bullet.sprite_rotate_action.emplace(bullet.sprite_move_action.sprite(), 4);
    }
}

void enemy_bullets::update()
{
    int bullets_count = _bullets.size();
    _palette_fade_action.update();

    for(int index = 0; index < bullets_count; )
    {
        bullet& bullet = _bullets[index];
        btn::sprite_move_by_action& sprite_move_action = bullet.sprite_move_action;
        const btn::fixed_point& position = sprite_move_action.sprite().position();

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height)
        {
            if(index < bullets_count - 1)
            {
                btn::swap(bullet, _bullets[bullets_count - 1]);
            }

            --bullets_count;
        }
        else
        {
            sprite_move_action.update();

            if(bullet.sprite_rotate_action)
            {
                bullet.sprite_rotate_action->update();
            }

            ++index;
        }
    }

    _bullets.shrink(bullets_count);
}

}
