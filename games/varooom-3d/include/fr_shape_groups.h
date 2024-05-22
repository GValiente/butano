/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SHAPE_GROUPS_H
#define FR_SHAPE_GROUPS_H

#include "bn_span.h"
#include "bn_color.h"
#include "bn_vector.h"
#include "bn_limits.h"
#include "bn_display.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"

#include "fr_model_3d_item.h"

namespace fr
{

class shape_groups
{

public:
    class hline
    {

    public:
        int xl;
        int xr;
    };

    shape_groups();

    ~shape_groups()
    {
        _clear();
    }

    void load_colors(const bn::span<const bn::color>& colors);

    void set_fade(bn::color color, bn::fixed intensity);

    void enable_drawing()
    {
        _draw_enabled = true;
    }

    BN_CODE_IWRAM void add_hlines(unsigned minimum_y, unsigned maximum_y, int width, bool x_outside, int color_index,
                                  unsigned shading, const hline* hlines);

    BN_CODE_IWRAM void add_sprite(unsigned minimum_y, unsigned maximum_y,
                                  uint16_t attr0, uint16_t attr1, uint16_t attr2);

    void update();

private:
    static constexpr int _max_palettes = 8;
    static constexpr int _max_hdma_sprites = 23;
    static constexpr int _hdma_source_size = (bn::display::height() + 1) * 4 * _max_hdma_sprites;

    class color_tiles
    {

    public:
        bn::sprite_tiles_ptr small_tiles;
        bn::sprite_tiles_ptr normal_tiles;
        bn::sprite_tiles_ptr big_tiles;
        bn::sprite_tiles_ptr huge_tiles;

        color_tiles(const bn::sprite_tiles_item& small_item, const bn::sprite_tiles_item& normal_item,
                    const bn::sprite_tiles_item& big_item, const bn::sprite_tiles_item& huge_item);
    };

    class color_tiles_ids
    {

    public:
        uint16_t small_tiles_id;
        uint16_t normal_tiles_id;
        uint16_t big_tiles_id;
        uint16_t huge_tiles_id;

        void load(const color_tiles& color_tiles);
    };

    alignas(int) bn::vector<color_tiles, face_3d::max_colors> _color_tiles;
    alignas(int) color_tiles_ids _color_tiles_ids[face_3d::max_colors];
    alignas(int) bn::color _colors[face_3d::max_colors];

    alignas(int) bn::vector<bn::sprite_palette_ptr, _max_palettes> _palettes;
    alignas(int) uint8_t _palette_ids[_max_palettes];

    alignas(int) uint8_t _hlines_count[bn::display::height()] = {};
    alignas(int) uint8_t _previous_hlines_count_a[bn::display::height()] = {};
    alignas(int) uint8_t _previous_hlines_count_b[bn::display::height()] = {};

    alignas(int) uint16_t _hdma_source_a[_hdma_source_size];
    alignas(int) uint16_t _hdma_source_b[_hdma_source_size];
    uint16_t* _hdma_source = _hdma_source_a;

    bool _draw_enabled = false;

    BN_CODE_IWRAM void _hide_left_hlines(const uint8_t* previous_hlines_count);

    void _clear();
};

}

#endif
