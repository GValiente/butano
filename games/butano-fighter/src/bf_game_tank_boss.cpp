#include "bf_game_tank_boss.h"

#include "btn_sprite_builder.h"
#include "btn_sprite_items_tank_base.h"
#include "btn_sprite_items_tank_jelly.h"
#include "btn_sprite_items_tank_cannon.h"
#include "btn_sprite_items_tank_footprint.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const int jelly_x = 31;
    constexpr const int jelly_damage_frames = 60;
}

tank_boss::tank_boss(const btn::fixed_point& hero_position, const btn::sprite_palette_ptr& damage_palette) :
    boss(500, 500, _tank_rects, damage_palette),
    _base_palette(btn::sprite_items::tank_base.palette_item().create_palette()),
    _jelly_palette(btn::sprite_items::tank_jelly.palette_item().create_palette()),
    _cannon_palette(btn::sprite_items::tank_cannon.palette_item().create_palette()),
    _y(-70)
{
    btn::fixed footprint_y = 16 - (btn::display::height() / 2);

    for(int index = 0; index < 5; ++index)
    {
        btn::sprite_builder builder(btn::sprite_items::tank_footprint);
        builder.set_position(-32, footprint_y + (index * 32));
        builder.set_z_order(constants::intro_sprites_z_order);
        _footprint_sprites.push_back(builder.release_build());

        builder = btn::sprite_builder(btn::sprite_items::tank_footprint);
        builder.set_position(32, footprint_y + (index * 32));
        builder.set_horizontal_flip(true);
        builder.set_z_order(constants::enemies_z_order);
        _footprint_sprites.push_back(builder.release_build());
    }

    for(int index = 0; index < 9; index += 3)
    {
        btn::sprite_builder builder(btn::sprite_items::tank_base, index);
        builder.set_x(-32);
        builder.set_z_order(constants::enemies_z_order);
        _base_sprites.push_back(builder.release_build());

        builder = btn::sprite_builder(btn::sprite_items::tank_base, index + 1);
        builder.set_z_order(constants::enemies_z_order);
        _base_sprites.push_back(builder.release_build());

        builder = btn::sprite_builder(btn::sprite_items::tank_base, index + 2);
        builder.set_x(32);
        builder.set_z_order(constants::enemies_z_order);
        _base_sprites.push_back(builder.release_build());
    }

    btn::sprite_builder builder(btn::sprite_items::tank_jelly);
    builder.set_x(jelly_x);
    builder.set_z_order(constants::enemies_z_order);
    _jelly_sprite = builder.release_build();

    builder = btn::sprite_builder(btn::sprite_items::tank_cannon);
    builder.set_rotation_angle(45);
    builder.set_z_order(constants::enemies_z_order);
    _cannon_sprite = builder.release_build();

    _tank_rects.emplace_back(btn::fixed_point(), btn::fixed_size(84, 58));
    _tank_rects.emplace_back(btn::fixed_point(), btn::fixed_size(76, 74));

    btn::fixed y = _calculate_y();
    _update_footprint_sprites(y);
    _update_base_sprites(y);
    _update_jelly_sprite(y, hero_position);
    _update_cannon_sprite(y);
    _update_rects(y);
}

void tank_boss::_update_alive(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets)
{
    btn::fixed y = _calculate_y();
    _update_footprint_sprites(y);
    _update_base_sprites(y);
    _update_jelly_sprite(y, hero_position);
    _update_cannon_sprite(y);
    _update_rects(y);
}

bool tank_boss::_update_dead(const btn::fixed_point& hero_position)
{
    btn::fixed y = _calculate_y();
    _update_footprint_sprites(y);
    _update_base_sprites(y);
    _update_jelly_sprite(y, hero_position);
    _update_cannon_sprite(y);
    return false;
}

void tank_boss::_show_damage_palette(const btn::sprite_palette_ptr& damage_palette)
{
    _jelly_damage_counter = jelly_damage_frames;

    for(btn::sprite_ptr& sprite : _base_sprites)
    {
        sprite.set_palette(damage_palette);
    }

    _jelly_sprite->set_palette(damage_palette);
    _cannon_sprite->set_palette(damage_palette);
}

void tank_boss::_hide_damage_palette()
{
    for(btn::sprite_ptr& sprite : _base_sprites)
    {
        sprite.set_palette(_base_palette);
    }

    _jelly_sprite->set_palette(_jelly_palette);
    _cannon_sprite->set_palette(_cannon_palette);
}

[[nodiscard]] btn::fixed tank_boss::_calculate_y()
{
    --_vibration_counter;

    if(! _vibration_counter)
    {
        _vibration_counter = 20;
    }

    btn::fixed result = _y;

    if(_vibration_counter > 10)
    {
        result += 1;
    }

    return result;
}

void tank_boss::_update_footprint_sprites(btn::fixed y)
{
    --_footprint_counter;

    if(! _footprint_counter)
    {
        _footprint_graphics_index = (_footprint_graphics_index + 1) % 4;
        _footprint_counter = 2;

        btn::sprite_tiles_ptr tiles = _footprint_sprites[0].tiles();
        tiles.set_tiles_ref(btn::sprite_items::tank_footprint.tiles_item().graphics_tiles_ref(_footprint_graphics_index));
    }

    btn::fixed hidden_y = -44;

    for(int index = 0; index < 5; ++index)
    {
        btn::sprite_ptr& left_sprite = _footprint_sprites[index * 2];
        btn::sprite_ptr& right_sprite = _footprint_sprites[(index * 2) + 1];
        bool visible = y >= hidden_y;
        left_sprite.set_visible(visible);
        right_sprite.set_visible(visible);
        hidden_y += 32;
    }
}

void tank_boss::_update_base_sprites(btn::fixed y)
{
    _base_sprites[0].set_y(y - 32);
    _base_sprites[1].set_y(y - 32);
    _base_sprites[2].set_y(y - 32);
    _base_sprites[3].set_y(y);
    _base_sprites[4].set_y(y);
    _base_sprites[5].set_y(y);
    _base_sprites[6].set_y(y + 32);
    _base_sprites[7].set_y(y + 32);
    _base_sprites[8].set_y(y + 32);

    for(int index = 0; index < 3; ++index)
    {
        int sprite_index = index * 3;
        btn::fixed base_y = y - 32 + (index * 32);
        _base_sprites[sprite_index + 0].set_y(base_y);
        _base_sprites[sprite_index + 1].set_y(base_y);
        _base_sprites[sprite_index + 2].set_y(base_y);
    }
}

void tank_boss::_update_jelly_sprite(btn::fixed y, const btn::fixed_point& hero_position)
{
    btn::sprite_ptr& jelly_sprite = *_jelly_sprite;

    if(_jelly_damage_counter)
    {
        --_jelly_damage_counter;

        if(_jelly_damage_counter)
        {
            if(! _jelly_crying)
            {
                _jelly_animate_action = btn::create_sprite_cached_animate_action_forever(
                            jelly_sprite, 16, btn::sprite_items::tank_jelly, 2, 3);
                _jelly_crying = true;
            }
        }
        else
        {
            _jelly_animate_action = btn::create_sprite_cached_animate_action_forever(
                        jelly_sprite, 16, btn::sprite_items::tank_jelly, 0, 1);
            _jelly_crying = false;
        }
    }

    jelly_sprite.set_y(y + 20);
    jelly_sprite.set_horizontal_flip(hero_position.x() < jelly_x);
    _jelly_animate_action->update();
}

void tank_boss::_update_cannon_sprite(btn::fixed y)
{
    _cannon_sprite->set_y(y);
}

void tank_boss::_update_rects(btn::fixed y)
{
    _tank_rects[0].set_y(y - 2);
    _tank_rects[1].set_y(y);
}

}
