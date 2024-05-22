/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_gem.h"

#include "bn_fixed_rect.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_gem.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

namespace
{
    constexpr bn::fixed_size dimensions(15, 15);
    constexpr int flash_frames = 60;

    [[nodiscard]] bn::sprite_ptr _create_sprite(
        const bn::fixed_point& position, const bn::span<bn::sprite_tiles_ptr>& tiles,
        const bn::sprite_palette_ptr& palette, const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::gem.shape_size(), tiles[0], palette);
        builder.set_position(position);
        builder.set_z_order(constants::gems_z_order);
        builder.set_camera(camera);
        return builder.release_build();
    }
}

gem::gem(const bn::fixed_point& position, const bn::span<bn::sprite_tiles_ptr>& tiles,
         const bn::sprite_palette_ptr& palette, const bn::camera_ptr& camera) :
    _position(position),
    _sprite(_create_sprite(position, tiles, palette, camera)),
    _tiles(tiles),
    _counter(flash_frames)
{
}

bool gem::intersects_hero(const bn::fixed_rect& hero_rect) const
{
    return bn::fixed_rect(_position, dimensions).intersects(hero_rect);
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
