#include "bf_game_object.h"

#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_hero_weapon_big_2.h"
#include "btn_sprite_items_hero_weapon_big_3.h"
#include "btn_sprite_items_hero_weapon_big_4.h"
#include "btn_sprite_items_hero_weapon_big_5.h"
#include "btn_sprite_items_hero_weapon_big_6.h"
#include "btn_sprite_items_hero_weapon_big_7.h"
#include "btn_sprite_items_hero_weapon_big_8.h"
#include "btn_sprite_items_hero_weapon_big_9.h"
#include "btn_sprite_items_hero_bomb_icon.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

object object::create_hero_weapon(const btn::fixed_point& position, int hero_level,
                                  const btn::sprite_palette_ptr& flash_palette)
{
    const btn::sprite_item* sprite_item = &btn::sprite_items::hero_weapon_big_2;
    btn::fixed_size dimensions(28, 16);

    switch(hero_level)
    {

    case 1:
        break;

    case 2:
        sprite_item = &btn::sprite_items::hero_weapon_big_3;
        dimensions = btn::fixed_size(33, 16);
        break;

    case 3:
        sprite_item = &btn::sprite_items::hero_weapon_big_4;
        dimensions = btn::fixed_size(23, 16);
        break;

    case 4:
        sprite_item = &btn::sprite_items::hero_weapon_big_5;
        dimensions = btn::fixed_size(22, 15);
        break;

    case 5:
        sprite_item = &btn::sprite_items::hero_weapon_big_6;
        dimensions = btn::fixed_size(32, 16);
        break;

    case 6:
        sprite_item = &btn::sprite_items::hero_weapon_big_7;
        dimensions = btn::fixed_size(27, 17);
        break;

    case 7:
        sprite_item = &btn::sprite_items::hero_weapon_big_8;
        dimensions = btn::fixed_size(38, 18);
        break;

    case 8:
        sprite_item = &btn::sprite_items::hero_weapon_big_9;
        dimensions = btn::fixed_size(17, 15);
        break;

    default:
        BTN_ERROR("Invalid hero level: ", hero_level);
        break;
    }

    btn::sprite_builder builder(*sprite_item);
    builder.set_position(position);
    builder.set_z_order(constants::objects_z_order);
    return object(builder.release_build(), position, dimensions, btn::fixed_point(1, -1), flash_palette);
}

object object::create_hero_bomb(const btn::fixed_point& position, const btn::sprite_palette_ptr& flash_palette)
{
    btn::sprite_builder builder(btn::sprite_items::hero_bomb_icon);
    builder.set_position(position);
    builder.set_z_order(constants::objects_z_order);
    return object(builder.release_build(), position, btn::fixed_size(14, 16), btn::fixed_point(-1, -1), flash_palette);
}

bool object::intersects_hero(const btn::fixed_rect& hero_rect) const
{
    return btn::fixed_rect(_position, _dimensions).intersects(hero_rect);
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

object::object(btn::sprite_ptr&& sprite, const btn::fixed_point& position, const btn::fixed_size& dimensions,
               const btn::fixed_point& delta_position, const btn::sprite_palette_ptr& flash_palette) :
    _position(position),
    _dimensions(dimensions),
    _delta_position(delta_position),
    _sprite(btn::move(sprite)),
    _sprite_palette(_sprite.palette()),
    _flash_palette(flash_palette),
    _flash_palette_counter(constants::object_flash_frames)
{
}

}
