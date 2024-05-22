/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_object.h"

#include "bn_fixed_rect.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_hero_weapon_big_2.h"
#include "bn_sprite_items_hero_weapon_big_3.h"
#include "bn_sprite_items_hero_weapon_big_4.h"
#include "bn_sprite_items_hero_weapon_big_5.h"
#include "bn_sprite_items_hero_weapon_big_6.h"
#include "bn_sprite_items_hero_weapon_big_7.h"
#include "bn_sprite_items_hero_weapon_big_8.h"
#include "bn_sprite_items_hero_weapon_big_9.h"
#include "bn_sprite_items_hero_bomb_icon.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

object object::create_hero_weapon(const bn::fixed_point& position, int hero_level,
                                  const bn::sprite_palette_ptr& flash_palette, const bn::camera_ptr& camera)
{
    const bn::sprite_item* sprite_item = &bn::sprite_items::hero_weapon_big_2;
	int width = 28;
	int height = 16;
	
    switch(hero_level)
    {

    case 1:
        break;

    case 2:
        sprite_item = &bn::sprite_items::hero_weapon_big_3;
		width = 33;
        break;

    case 3:
        sprite_item = &bn::sprite_items::hero_weapon_big_4;
		width = 23;
        break;

    case 4:
        sprite_item = &bn::sprite_items::hero_weapon_big_5;
		width = 22;
		height = 15;
        break;

    case 5:
        sprite_item = &bn::sprite_items::hero_weapon_big_6;
		width = 32;
        break;

    case 6:
        sprite_item = &bn::sprite_items::hero_weapon_big_7;
		width = 27;
		height = 17;
        break;

    case 7:
        sprite_item = &bn::sprite_items::hero_weapon_big_8;
		width = 38;
		height = 18;
        break;

    case 8:
        sprite_item = &bn::sprite_items::hero_weapon_big_9;
		width = 17;
		height = 15;
        break;

    default:
        BN_ERROR("Invalid hero level: ", hero_level);
        break;
    }

    bn::sprite_builder builder(*sprite_item);
    builder.set_position(position);
    builder.set_z_order(constants::objects_z_order);
    builder.set_camera(camera);
    return object(builder.release_build(), position, bn::fixed_size(width, height), bn::fixed_point(1, -1), flash_palette);
}

object object::create_hero_bomb(const bn::fixed_point& position, const bn::sprite_palette_ptr& flash_palette,
                                const bn::camera_ptr& camera)
{
    bn::sprite_builder builder(bn::sprite_items::hero_bomb_icon);
    builder.set_position(position);
    builder.set_z_order(constants::objects_z_order);
    builder.set_camera(camera);
    return object(builder.release_build(), position, bn::fixed_size(14, 16), bn::fixed_point(-1, -1), flash_palette);
}

bool object::intersects_hero(const bn::fixed_rect& hero_rect) const
{
    return bn::fixed_rect(_position, _dimensions).intersects(hero_rect);
}

int object::experience(int hero_level) const
{
    return hero_bullet_level::gem_experience(hero_level, -bf::constants::play_height);
}

void object::update()
{
    if(_position.x() <= -constants::play_width)
    {
        _delta_position.set_x(1);
    }
    else if(_position.x() >= constants::play_width)
    {
        _delta_position.set_x(-1);
    }

    if(_position.y() <= -constants::play_height)
    {
        _delta_position.set_y(1);
    }
    else if(_position.y() >= constants::play_height)
    {
        _delta_position.set_y(-1);
    }

    _position += _delta_position;
    _sprite.set_position(_position);
    --_flash_palette_counter;

    if(_flash_palette_counter == constants::object_flash_frames / 2)
    {
        _sprite.set_palette(_flash_palette);
    }
    else if(! _flash_palette_counter)
    {
        _sprite.set_palette(_sprite_palette);
        _flash_palette_counter = constants::object_flash_frames;
    }
}

object::object(bn::sprite_ptr&& sprite, const bn::fixed_point& position, const bn::fixed_size& dimensions,
               const bn::fixed_point& delta_position, const bn::sprite_palette_ptr& flash_palette) :
    _position(position),
    _dimensions(dimensions),
    _delta_position(delta_position),
    _sprite(bn::move(sprite)),
    _sprite_palette(_sprite.palette()),
    _flash_palette(flash_palette),
    _flash_palette_counter(constants::object_flash_frames)
{
}

}
