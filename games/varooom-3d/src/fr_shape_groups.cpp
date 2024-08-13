/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_shape_groups.h"

#include "bn_hdma.h"
#include "bn_memory.h"
#include "bn_sprites.h"
#include "../../butano/hw/include/bn_hw_sprites.h"

#include "bn_sprite_tiles_items_shape_group_texture_1_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_1_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_1_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_1_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_2_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_2_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_2_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_2_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_3_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_3_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_3_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_3_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_4_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_4_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_4_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_4_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_5_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_5_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_5_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_5_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_6_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_6_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_6_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_6_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_7_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_7_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_7_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_7_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_8_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_8_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_8_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_8_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_9_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_9_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_9_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_9_64.h"
#include "bn_sprite_tiles_items_shape_group_texture_10_8.h"
#include "bn_sprite_tiles_items_shape_group_texture_10_16.h"
#include "bn_sprite_tiles_items_shape_group_texture_10_32.h"
#include "bn_sprite_tiles_items_shape_group_texture_10_64.h"

namespace fr
{

namespace
{
    [[nodiscard]] bn::color _brightness_color(bn::color color, int brightness)
    {
        int red = (color.red() * brightness) / 32;
        int green = (color.green() * brightness) / 32;
        int blue = (color.blue() * brightness) / 32;
        int color_value = red + (green << 5) + (blue << 10);
        return bn::color(color_value);
    }
}

shape_groups::shape_groups()
{
    for(int index = 0; index < _hdma_source_size; index += 4)
    {
        bn::hw::sprites::hide_and_destroy(_hdma_source_a[index]);
        bn::hw::sprites::hide_and_destroy(_hdma_source_b[index]);
    }
}

void shape_groups::load_colors(const bn::span<const bn::color>& colors)
{
    int colors_count = colors.size();
    BN_ASSERT(colors_count <= face_3d::max_colors, "Invalid colors count: ", colors_count);

    if(! colors_count)
    {
        _color_tiles.clear();
        _palettes.clear();
        return;
    }

    int color_tiles_count = colors_count;
    int current_color_tiles_count = _color_tiles.size();
    bool reload_palettes;

    if(current_color_tiles_count < color_tiles_count)
    {
        reload_palettes = true;

        for(int index = current_color_tiles_count; index < color_tiles_count; ++index)
        {
            switch(index)
            {

            case 0:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_1_8,
                                          bn::sprite_tiles_items::shape_group_texture_1_16,
                                          bn::sprite_tiles_items::shape_group_texture_1_32,
                                          bn::sprite_tiles_items::shape_group_texture_1_64);
                break;

            case 1:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_2_8,
                                          bn::sprite_tiles_items::shape_group_texture_2_16,
                                          bn::sprite_tiles_items::shape_group_texture_2_32,
                                          bn::sprite_tiles_items::shape_group_texture_2_64);
                break;

            case 2:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_3_8,
                                          bn::sprite_tiles_items::shape_group_texture_3_16,
                                          bn::sprite_tiles_items::shape_group_texture_3_32,
                                          bn::sprite_tiles_items::shape_group_texture_3_64);
                break;

            case 3:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_4_8,
                                          bn::sprite_tiles_items::shape_group_texture_4_16,
                                          bn::sprite_tiles_items::shape_group_texture_4_32,
                                          bn::sprite_tiles_items::shape_group_texture_4_64);
                break;

            case 4:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_5_8,
                                          bn::sprite_tiles_items::shape_group_texture_5_16,
                                          bn::sprite_tiles_items::shape_group_texture_5_32,
                                          bn::sprite_tiles_items::shape_group_texture_5_64);
                break;

            case 5:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_6_8,
                                          bn::sprite_tiles_items::shape_group_texture_6_16,
                                          bn::sprite_tiles_items::shape_group_texture_6_32,
                                          bn::sprite_tiles_items::shape_group_texture_6_64);
                break;

            case 6:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_7_8,
                                          bn::sprite_tiles_items::shape_group_texture_7_16,
                                          bn::sprite_tiles_items::shape_group_texture_7_32,
                                          bn::sprite_tiles_items::shape_group_texture_7_64);
                break;

            case 7:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_8_8,
                                          bn::sprite_tiles_items::shape_group_texture_8_16,
                                          bn::sprite_tiles_items::shape_group_texture_8_32,
                                          bn::sprite_tiles_items::shape_group_texture_8_64);
                break;

            case 8:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_9_8,
                                          bn::sprite_tiles_items::shape_group_texture_9_16,
                                          bn::sprite_tiles_items::shape_group_texture_9_32,
                                          bn::sprite_tiles_items::shape_group_texture_9_64);
                break;

            case 9:
                _color_tiles.emplace_back(bn::sprite_tiles_items::shape_group_texture_10_8,
                                          bn::sprite_tiles_items::shape_group_texture_10_16,
                                          bn::sprite_tiles_items::shape_group_texture_10_32,
                                          bn::sprite_tiles_items::shape_group_texture_10_64);
                break;

            default:
                BN_ERROR("Invalid color tiles index: ", index);
                break;
            }

            _color_tiles_ids[index].load(_color_tiles.back());
        }
    }
    else
    {
        if(current_color_tiles_count > color_tiles_count)
        {
            _color_tiles.shrink(color_tiles_count);
        }

        reload_palettes = colors != bn::span<const bn::color>(_colors, colors_count);
    }

    if(reload_palettes)
    {
        bn::color palettes_colors[_max_palettes][16];

        for(int color_index = 0; color_index < colors_count; ++color_index)
        {
            bn::color color = colors[color_index];
            _colors[color_index] = color;

            int palette_color_index = color_index + 1;
            int brightness = 32 - 7;

            for(bn::color* palette_colors : palettes_colors)
            {
                palette_colors[palette_color_index] = _brightness_color(color, brightness);
                ++brightness;
            }
        }

        if(_palettes.empty())
        {
            for(int palette_index = 0; palette_index < _max_palettes; ++palette_index)
            {
                bn::sprite_palette_item palette_item(palettes_colors[palette_index], bn::bpp_mode::BPP_4);
                bn::sprite_palette_ptr palette = palette_item.create_new_palette();
                _palette_ids[palette_index] = uint8_t(palette.id());
                _palettes.push_back(bn::move(palette));
            }
        }
        else
        {
            for(int palette_index = 0; palette_index < _max_palettes; ++palette_index)
            {
                bn::sprite_palette_item palette_item(palettes_colors[palette_index], bn::bpp_mode::BPP_4);
                _palettes[palette_index].set_colors(palette_item);
            }
        }
    }
}

void shape_groups::set_fade(bn::color color, bn::fixed intensity)
{
    for(bn::sprite_palette_ptr& palette : _palettes)
    {
        palette.set_fade(color, intensity);
    }
}

void shape_groups::update()
{
    if(_draw_enabled)
    {
        uint16_t* hdma_source = _hdma_source;
        _draw_enabled = false;

        if(hdma_source == _hdma_source_a)
        {
            _hide_left_hlines(_previous_hlines_count_a);
        }
        else
        {
            _hide_left_hlines(_previous_hlines_count_b);
        }

        int max_sprites = _max_hdma_sprites;
        int scanline_elements = max_sprites * 4;
        int hdma_source_size = bn::display::height() * scanline_elements;
        bn::memory::copy(hdma_source[0], scanline_elements, hdma_source[hdma_source_size]);

        bn::span<const uint16_t> hdma_source_ref(hdma_source + scanline_elements, hdma_source_size);
        bn::hdma::start(hdma_source_ref, bn::hw::sprites::vram()[128 - max_sprites].attr0);

        if(hdma_source == _hdma_source_a)
        {
            bn::memory::copy(*_hlines_count, bn::display::height(), *_previous_hlines_count_a);
            _hdma_source = _hdma_source_b;
        }
        else
        {
            bn::memory::copy(*_hlines_count, bn::display::height(), *_previous_hlines_count_b);
            _hdma_source = _hdma_source_a;
        }

        bn::memory::clear(bn::display::height(), *_hlines_count);
    }
    else
    {
        _clear();
    }
}

void shape_groups::_clear()
{
    if(bn::hdma::running())
    {
        bn::hdma::stop();
        bn::sprites::reload();
    }
}

shape_groups::color_tiles::color_tiles(
        const bn::sprite_tiles_item& small_item, const bn::sprite_tiles_item& normal_item,
        const bn::sprite_tiles_item& big_item, const bn::sprite_tiles_item& huge_item) :
    small_tiles(small_item.create_tiles()),
    normal_tiles(normal_item.create_tiles()),
    big_tiles(big_item.create_tiles()),
    huge_tiles(huge_item.create_tiles())
{
}

void shape_groups::color_tiles_ids::load(const color_tiles& color_tiles)
{
    small_tiles_id = uint16_t(color_tiles.small_tiles.id());
    normal_tiles_id = uint16_t(color_tiles.normal_tiles.id());
    big_tiles_id = uint16_t(color_tiles.big_tiles.id());
    huge_tiles_id = uint16_t(color_tiles.huge_tiles.id());
}

}
