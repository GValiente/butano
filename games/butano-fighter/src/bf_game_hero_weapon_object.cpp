#include "bf_game_hero_weapon_object.h"

#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_hero_weapons_sprite_item.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed_size dimensions(14, 14);
    constexpr const int flash_frames = 16;

    btn::sprite_ptr _create_sprite(const btn::fixed_point& position, int hero_level)
    {
        btn::sprite_builder builder(btn::sprite_items::hero_weapons, hero_level + 1);
        builder.set_position(position);
        builder.set_z_order(constants::objects_z_order);
        return builder.release_build();
    }
}

hero_weapon_object::hero_weapon_object(const btn::fixed_point& position, int hero_level,
                                       const btn::sprite_palette_ptr& flash_palette) :
    _position(position.x(), position.y() - 1),
    _delta_position(1, -1),
    _sprite(_create_sprite(_position, hero_level)),
    _sprite_palette(_sprite.palette()),
    _flash_palette(flash_palette),
    _flash_palette_counter(flash_frames)
{
}

bool hero_weapon_object::intersects_hero(const btn::fixed_rect& hero_rect) const
{
    return btn::fixed_rect(_position, dimensions).intersects(hero_rect);
}

void hero_weapon_object::update()
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

}
