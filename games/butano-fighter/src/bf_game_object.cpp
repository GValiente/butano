#include "bf_game_object.h"

#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_hero_weapons_sprite_item.h"
#include "btn_hero_bomb_icon_sprite_item.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed_size dimensions(14, 14);
    constexpr const int flash_frames = 16;
}

object object::create_hero_weapon(const btn::fixed_point& position, int hero_level,
                                  const btn::sprite_palette_ptr& flash_palette)
{
    btn::fixed_point sprite_position(position.x(), position.y() - 1);
    btn::sprite_builder builder(btn::sprite_items::hero_weapons, hero_level);
    builder.set_position(sprite_position);
    builder.set_z_order(constants::objects_z_order);
    return object(builder.release_build(), sprite_position, btn::fixed_point(1, -1), flash_palette);
}

object object::create_hero_bomb(const btn::fixed_point& position, const btn::sprite_palette_ptr& flash_palette)
{
    btn::sprite_builder builder(btn::sprite_items::hero_bomb_icon);
    builder.set_position(position);
    builder.set_z_order(constants::objects_z_order);
    return object(builder.release_build(), position, btn::fixed_point(-1, -1), flash_palette);
}

bool object::intersects_hero(const btn::fixed_rect& hero_rect) const
{
    return btn::fixed_rect(_position, dimensions).intersects(hero_rect);
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

    if(_flash_palette_counter == flash_frames / 2)
    {
        _sprite.set_palette(_flash_palette);
    }
    else if(! _flash_palette_counter)
    {
        _sprite.set_palette(_sprite_palette);
        _flash_palette_counter = flash_frames;
    }
}

object::object(btn::sprite_ptr&& sprite, const btn::fixed_point& position, const btn::fixed_point& delta_position,
               const btn::sprite_palette_ptr& flash_palette) :
    _position(position),
    _delta_position(delta_position),
    _sprite(btn::move(sprite)),
    _sprite_palette(_sprite.palette()),
    _flash_palette(flash_palette),
    _flash_palette_counter(flash_frames)
{
}

}
