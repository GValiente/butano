#include "bf_game_gem.h"

#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_gem.h"
#include "bf_constants.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed_size dimensions(14, 14);
    constexpr const int flash_frames = 60;

    [[nodiscard]] btn::sprite_ptr _create_sprite(
        const btn::fixed_point& position, const btn::span<btn::sprite_tiles_ptr>& tiles,
        const btn::sprite_palette_ptr& palette)
    {
        btn::sprite_builder builder(btn::sprite_items::gem.shape_size(), tiles[0], palette);
        builder.set_position(position);
        builder.set_z_order(constants::objects_z_order);
        return builder.release_build();
    }
}

gem::gem(const btn::fixed_point& position, const btn::span<btn::sprite_tiles_ptr>& tiles,
         const btn::sprite_palette_ptr& palette) :
    _sprite(_create_sprite(position, tiles, palette)),
    _tiles(tiles),
    _counter(flash_frames)
{
}

bool gem::intersects_hero(const btn::fixed_rect& hero_rect) const
{
    return btn::fixed_rect(_sprite.position(), dimensions).intersects(hero_rect);
}

bool gem::done() const
{
    return _sprite.y() > constants::view_height;
}

void gem::update()
{
    _sprite.set_position(_sprite.x(), _sprite.y() + 0.5);
    --_counter;

    if(_counter == 12)
    {
        _sprite.set_tiles(_tiles[1]);
    }
    else if(_counter == 8)
    {
        _sprite.set_tiles(_tiles[2]);
    }
    else if(_counter == 4)
    {
        _sprite.set_tiles(_tiles[3]);
    }
    else if(_counter == 0)
    {
        _sprite.set_tiles(_tiles[0]);
        _counter = flash_frames;
    }
}

}
