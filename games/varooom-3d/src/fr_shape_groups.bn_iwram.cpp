/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_shape_groups.h"

#include "../../butano/hw/include/bn_hw_sprites.h"

namespace fr
{

namespace
{
    constexpr int split_length = 64 - 2;
}

void shape_groups::add_hlines(unsigned minimum_y, unsigned maximum_y, int width, bool x_outside, int color_index,
                              unsigned shading, const hline* hlines)
{
    const color_tiles_ids& tiles_ids = _color_tiles_ids[color_index];
    uint8_t palette_id = _palette_ids[shading];
    int attr1;
    int attr2;
    bool split;

    if(width < 8)
    {
        attr1 = bn::hw::sprites::second_attributes(0, bn::sprite_size::SMALL, false, false);
        attr2 = bn::hw::sprites::third_attributes(tiles_ids.small_tiles_id, palette_id, 3);
        split = false;
    }
    else if(width < 16)
    {
        attr1 = bn::hw::sprites::second_attributes(0, bn::sprite_size::NORMAL, false, false);
        attr2 = bn::hw::sprites::third_attributes(tiles_ids.normal_tiles_id, palette_id, 3);
        split = false;
    }
    else if(width < 32)
    {
        attr1 = bn::hw::sprites::second_attributes(0, bn::sprite_size::BIG, false, false);
        attr2 = bn::hw::sprites::third_attributes(tiles_ids.big_tiles_id, palette_id, 3);
        split = false;
    }
    else
    {
        attr1 = bn::hw::sprites::second_attributes(0, bn::sprite_size::HUGE, false, false);
        attr2 = bn::hw::sprites::third_attributes(tiles_ids.huge_tiles_id, palette_id, 3);
        split = width > split_length;
    }

    uint16_t* hdma_source = _hdma_source;
    int screen_line_elements = _max_hdma_sprites * 4;

    if(! split) [[likely]]
    {
        if(x_outside) [[unlikely]]
        {
            for(unsigned y = minimum_y; y <= maximum_y; ++y)
            {
                int xl = hlines[y].xl;

                if(xl < bn::display::width()) [[likely]]
                {
                    int xr = hlines[y].xr;

                    if(xr >= 0) [[likely]]
                    {
                        int hlines_count = _hlines_count[y];

                        if(hlines_count < _max_hdma_sprites) [[likely]]
                        {
                            _hlines_count[y] = hlines_count + 1;

                            uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
                            sprite_hdma_source += hlines_count * 4;

                            if(xl < 0)
                            {
                                xl = 0;
                            }

                            if(xr > bn::display::width() - 1)
                            {
                                xr = bn::display::width() - 1;
                            }

                            int length = xr - xl;
                            int sprite_y = int(y) - length;
                            sprite_hdma_source[0] = bn::hw::sprites::first_attributes(
                                        sprite_y, bn::sprite_shape::SQUARE, bn::bpp_mode::BPP_4, 0,
                                        true, false, false, false);

                            sprite_hdma_source[1] = attr1 + xl;

                            sprite_hdma_source[2] = attr2;
                        }
                    }
                }
            }
        }
        else
        {
            for(unsigned y = minimum_y; y <= maximum_y; ++y)
            {
                int hlines_count = _hlines_count[y];

                if(hlines_count < _max_hdma_sprites) [[likely]]
                {
                    _hlines_count[y] = hlines_count + 1;

                    uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
                    sprite_hdma_source += hlines_count * 4;

                    int xl = hlines[y].xl;
                    int xr = hlines[y].xr;
                    int length = xr - xl;
                    int sprite_y = int(y) - length;
                    sprite_hdma_source[0] = bn::hw::sprites::first_attributes(
                                sprite_y, bn::sprite_shape::SQUARE, bn::bpp_mode::BPP_4, 0,
                                true, false, false, false);

                    sprite_hdma_source[1] = attr1 + xl;

                    sprite_hdma_source[2] = attr2;
                }
            }
        }
    }
    else
    {
        if(x_outside) [[unlikely]]
        {
            for(unsigned y = minimum_y; y <= maximum_y; ++y)
            {
                int xl = hlines[y].xl;

                if(xl < bn::display::width()) [[likely]]
                {
                    int xr = hlines[y].xr;

                    if(xr >= 0) [[likely]]
                    {
                        if(xl < 0)
                        {
                            xl = 0;
                        }

                        if(xr > bn::display::width() - 1)
                        {
                            xr = bn::display::width() - 1;
                        }

                        int hlines_count = _hlines_count[y];
                        uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
                        sprite_hdma_source += hlines_count * 4;

                        bool keep_adding;

                        do
                        {
                            if(hlines_count < _max_hdma_sprites) [[likely]]
                            {
                                int length = xr - xl;
                                int sprite_y;
                                keep_adding = length > split_length;
                                ++hlines_count;

                                if(keep_adding)
                                {
                                    sprite_y = int(y) - split_length;
                                }
                                else
                                {
                                    sprite_y = int(y) - length;
                                }

                                sprite_hdma_source[0] = bn::hw::sprites::first_attributes(
                                            sprite_y, bn::sprite_shape::SQUARE, bn::bpp_mode::BPP_4, 0,
                                            true, false, false, false);

                                sprite_hdma_source[1] = attr1 + xl;

                                sprite_hdma_source[2] = attr2;

                                xl += split_length;
                                sprite_hdma_source += 4;
                            }
                            else
                            {
                                keep_adding = false;
                            }
                        }
                        while(keep_adding);

                        _hlines_count[y] = hlines_count;
                    }
                }
            }
        }
        else
        {
            for(unsigned y = minimum_y; y <= maximum_y; ++y)
            {
                int xl = hlines[y].xl;
                int xr = hlines[y].xr;

                int hlines_count = _hlines_count[y];
                uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
                sprite_hdma_source += hlines_count * 4;

                bool keep_adding;

                do
                {
                    if(hlines_count < _max_hdma_sprites) [[likely]]
                    {
                        int length = xr - xl;
                        int sprite_y;
                        keep_adding = length > split_length;
                        ++hlines_count;

                        if(keep_adding) [[likely]]
                        {
                            sprite_y = int(y) - split_length;
                        }
                        else
                        {
                            sprite_y = int(y) - length;
                        }

                        sprite_hdma_source[0] = bn::hw::sprites::first_attributes(
                                    sprite_y, bn::sprite_shape::SQUARE, bn::bpp_mode::BPP_4, 0,
                                    true, false, false, false);

                        sprite_hdma_source[1] = attr1 + xl;

                        sprite_hdma_source[2] = attr2;

                        xl += split_length;
                        sprite_hdma_source += 4;
                    }
                    else
                    {
                        keep_adding = false;
                    }
                }
                while(keep_adding);

                _hlines_count[y] = hlines_count;
            }
        }
    }
}

void shape_groups::add_sprite(unsigned minimum_y, unsigned maximum_y, uint16_t attr0, uint16_t attr1, uint16_t attr2)
{
    uint16_t* hdma_source = _hdma_source;
    int screen_line_elements = _max_hdma_sprites * 4;

    for(unsigned y = minimum_y; y <= maximum_y; ++y)
    {
        int hlines_count = _hlines_count[y];

        if(hlines_count < _max_hdma_sprites) [[likely]]
        {
            _hlines_count[y] = hlines_count + 1;

            uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
            sprite_hdma_source += hlines_count * 4;

            sprite_hdma_source[0] = attr0;

            sprite_hdma_source[1] = attr1;

            sprite_hdma_source[2] = attr2;
        }
    }
}

void shape_groups::_hide_left_hlines(const uint8_t* previous_hlines_count)
{
    uint16_t* hdma_source = _hdma_source;
    int screen_line_elements = _max_hdma_sprites * 4;

    for(int y = 0; y < bn::display::height(); ++y)
    {
        uint16_t* sprite_hdma_source = hdma_source + (y * screen_line_elements);
        int hlines_count = _hlines_count[y];
        int hlines_previous_count = previous_hlines_count[y];

        for(int hlines_index = hlines_count; hlines_index < hlines_previous_count; ++hlines_index)
        {
            sprite_hdma_source[hlines_index * 4] = ATTR0_HIDE;
        }
    }
}

}
