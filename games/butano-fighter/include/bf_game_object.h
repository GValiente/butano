/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_OBJECT_H
#define BF_GAME_OBJECT_H

#include "bn_sprite_ptr.h"
#include "bn_fixed_size.h"
#include "bn_fixed_point.h"
#include "bn_fixed_rect_fwd.h"
#include "bn_sprite_palette_ptr.h"

namespace bf::game
{

class object
{

public:
    [[nodiscard]] static object create_hero_weapon(
            const bn::fixed_point& position, int hero_level, const bn::sprite_palette_ptr& flash_palette,
            const bn::camera_ptr& camera);

    [[nodiscard]] static object create_hero_bomb(
            const bn::fixed_point& position, const bn::sprite_palette_ptr& flash_palette,
            const bn::camera_ptr& camera);

    [[nodiscard]] const bn::fixed_point& position() const
    {
        return _position;
    }

    [[nodiscard]] bool intersects_hero(const bn::fixed_rect& hero_rect) const;

    [[nodiscard]] int experience(int hero_level) const;

    void update();

private:
    bn::fixed_point _position;
    bn::fixed_size _dimensions;
    bn::fixed_point _delta_position;
    bn::sprite_ptr _sprite;
    bn::sprite_palette_ptr _sprite_palette;
    bn::sprite_palette_ptr _flash_palette;
    int _flash_palette_counter;

    object(bn::sprite_ptr&& sprite, const bn::fixed_point& position, const bn::fixed_size& dimensions,
           const bn::fixed_point& delta_position, const bn::sprite_palette_ptr& flash_palette);
};

}

#endif
