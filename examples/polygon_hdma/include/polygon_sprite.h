#ifndef POLYGON_SPRITE_H
#define POLYGON_SPRITE_H

#include "btn_vector.h"
#include "btn_display.h"
#include "btn_span_fwd.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{
    class fixed_point;
}

class polygon;

class polygon_sprite
{

public:
    polygon_sprite(const polygon& polygon, int graphics_index, int z_order);

    polygon_sprite(const btn::span<const polygon*>& polygons, int graphics_index, int z_order);

    polygon_sprite(const polygon_sprite& other) = delete;

    polygon_sprite& operator=(const polygon_sprite& other) = delete;

    void update(int max_polygon_sprites, uint16_t* hdma_source);

private:
    struct hline
    {
        int ixl = btn::display::width();
        int ixr = 0;
    };

    btn::vector<const polygon*, 2> _polygons;
    btn::sprite_tiles_ptr _tiles;
    btn::sprite_palette_ptr _palette;
    int _z_order;
    int _minimum_y = 0;
    int _maximum_y = btn::display::height() - 1;

    static void _draw_line(bool left, int x0, int y0, int x1, int y1, hline* hlines);

    BTN_CODE_IWRAM static void _draw_left_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BTN_CODE_IWRAM static void _draw_right_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BTN_CODE_IWRAM static void _draw_left_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BTN_CODE_IWRAM static void _draw_right_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BTN_CODE_IWRAM static void _setup_attributes(const void* base_sprite_handle_ptr, const hline* hlines, int z_order,
                                                 int max_polygon_sprites, int minimum_y, int maximum_y,
                                                 uint16_t* hdma_source);
};

#endif
