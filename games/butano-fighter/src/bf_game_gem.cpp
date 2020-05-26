#include "bf_game_gem.h"

#include "btn_fixed_rect.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_gem.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

namespace
{
    constexpr const btn::fixed_size dimensions(15, 15);
    constexpr const int flash_frames = 60;

    [[nodiscard]] btn::sprite_ptr _create_sprite(
        const btn::fixed_point& position, const btn::span<btn::sprite_tiles_ptr>& tiles,
        const btn::sprite_palette_ptr& palette)
    {
        btn::sprite_builder builder(btn::sprite_items::gem.shape_size(), tiles[0], palette);
        builder.set_position(position);
        builder.set_z_order(constants::gems_z_order);
        return builder.release_build();
    }
}

gem::gem(const btn::fixed_point& position, const btn::span<btn::sprite_tiles_ptr>& tiles,
         const btn::sprite_palette_ptr& palette) :
    _position(position),
    _sprite(_create_sprite(position, tiles, palette)),
    _tiles(tiles),
    _counter(flash_frames)
{
}

bool gem::intersects_hero(const btn::fixed_rect& hero_rect) const
{
    return btn::fixed_rect(_position, dimensions).intersects(hero_rect);
}

bool gem::done() const
{
    return _position.y() > constants::view_height;
}

int gem::experience(int hero_level) const
{
    return hero_bullet_level::gem_experience(hero_level, _position.y());
}

void gem::update()
{
    _position.set_y(_position.y() + constants::background_speed);
    _sprite.set_y(_position.y());
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
