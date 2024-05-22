/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SCOREBOARD_NUMBER_H
#define FR_SCOREBOARD_NUMBER_H

#include "bn_vector.h"
#include "bn_string.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_builder.h"

namespace fr
{

template<int MaxSize>
class scoreboard_number
{

public:
    scoreboard_number(const bn::sprite_shape_size& shape_size, const bn::sprite_tiles_item& tiles_item,
                      int graphics_offset, const bn::sprite_palette_ptr& palette, bn::fixed x, bn::fixed y) :
        scoreboard_number(shape_size, tiles_item, graphics_offset, palette, x, y, shape_size.width())
    {
    }

    scoreboard_number(const bn::sprite_shape_size& shape_size, const bn::sprite_tiles_item& tiles_item,
                      int graphics_offset, const bn::sprite_palette_ptr& palette, bn::fixed x, bn::fixed y,
                      bn::fixed width) :
        _tiles_item(tiles_item),
        _width(width),
        _graphics_offset(graphics_offset)
    {
        bn::sprite_builder builder(shape_size, tiles_item.create_tiles(graphics_offset), palette);
        builder.set_bg_priority(1);
        x += width / 2;

        for(int index = 0; index < MaxSize; ++index)
        {
            builder.set_position(x, y);
            _sprites.push_back(builder.build());
            x += width;
        }
    }

    [[nodiscard]] bn::fixed x() const
    {
        return _sprites[0].x();
    }

    void set_x(bn::fixed x)
    {
        bn::fixed width = _width;

        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_x(x);
            x += width;
        }
    }

    void set_visible(bool visible)
    {
        for(bn::sprite_ptr& sprite : _sprites)
        {
            sprite.set_visible(visible);
        }
    }

    void show(int digit)
    {
        BN_ASSERT(digit >= 0, "Negative digit: ", digit);

        bn::string<32> text = bn::to_string<32>(digit);
        int characters_count = text.size();
        BN_ASSERT(characters_count <= MaxSize, "Invalid digit: ", digit, " - ", MaxSize);

        bn::sprite_ptr* sprites = _sprites.data();
        const char* characters = text.data();
        int sprite_index = MaxSize - 1;
        int character_index = characters_count - 1;
        int graphics_offset = _graphics_offset;

        while(character_index >= 0)
        {
            char character = characters[character_index];
            int tiles_index = int(character - '0');
            sprites[sprite_index].set_tiles(_tiles_item.create_tiles(tiles_index + graphics_offset));
            --sprite_index;
            --character_index;
        }

        while(sprite_index >= 0)
        {
            sprites[sprite_index].set_tiles(_tiles_item.create_tiles(graphics_offset));
            --sprite_index;
        }
    }

private:
    const bn::sprite_tiles_item& _tiles_item;
    bn::vector<bn::sprite_ptr, MaxSize> _sprites;
    bn::fixed _width;
    int _graphics_offset;
};

}

#endif
