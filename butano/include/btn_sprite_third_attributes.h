/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_THIRD_ATTRIBUTES_H
#define BTN_SPRITE_THIRD_ATTRIBUTES_H

#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

class sprite_third_attributes
{

public:
    sprite_third_attributes(sprite_tiles_ptr tiles, sprite_palette_ptr palette, int bg_priority);

    [[nodiscard]] const sprite_tiles_ptr& tiles() const
    {
        return _tiles;
    }

    void set_tiles(const sprite_tiles_ptr& tiles)
    {
        _tiles = tiles;
    }

    void set_tiles(sprite_tiles_ptr&& tiles)
    {
        _tiles = move(tiles);
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return _palette;
    }

    void set_palette(const sprite_palette_ptr& palette)
    {
        _palette = palette;
    }

    void set_palette(sprite_palette_ptr&& palette)
    {
        _palette = move(palette);
    }

    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    void set_bg_priority(int bg_priority);

    [[nodiscard]] friend bool operator==(const sprite_third_attributes& a, const sprite_third_attributes& b) = default;

private:
    sprite_tiles_ptr _tiles;
    sprite_palette_ptr _palette;
    int8_t _bg_priority;
};

}

#endif

