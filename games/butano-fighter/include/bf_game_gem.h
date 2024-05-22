/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_GEM_H
#define BF_GAME_GEM_H

#include "bn_span.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect_fwd.h"

namespace bf::game
{

class gem
{

public:
    gem(const bn::fixed_point& position, const bn::span<bn::sprite_tiles_ptr>& tiles,
        const bn::sprite_palette_ptr& palette, const bn::camera_ptr& camera);

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool intersects_hero(const bn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool done() const;

    [[nodiscard]] int experience(int hero_level) const;

    void update();

private:
    bn::fixed_point _position;
    bn::sprite_ptr _sprite;
    bn::span<bn::sprite_tiles_ptr> _tiles;
    int _counter;
};

}

#endif
