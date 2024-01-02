#ifndef POLYGON_SPRITE_H
#define POLYGON_SPRITE_H

#include "bn_vector.h"
#include "bn_display.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"

class polygon;

class polygon_sprite
{

public:
    polygon_sprite(const polygon& polygon, int graphics_index, int z_order);

    polygon_sprite(const bn::span<const polygon*>& polygons, int graphics_index, int z_order);

    polygon_sprite(const polygon_sprite& other) = delete;

    polygon_sprite& operator=(const polygon_sprite& other) = delete;

    void update(int max_polygon_sprites, uint16_t* hdma_source);

private:
    struct hline
    {
        int ixl = bn::display::width();
        int ixr = 0;
    };

    bn::vector<const polygon*, 2> _polygons;
    bn::sprite_tiles_ptr _tiles;
    bn::sprite_palette_ptr _palette;
    int _z_order;
    int _minimum_y = 0;
    int _maximum_y = bn::display::height() - 1;

    BN_CODE_IWRAM static void _draw_not_horizontal_line(bool left, int x0, int y0, int x1, int y1, hline* hlines);

    BN_CODE_IWRAM static void _draw_horizontal_line(int x0, int x1, int y, hline* hlines);

    BN_CODE_IWRAM static void _draw_left_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BN_CODE_IWRAM static void _draw_right_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BN_CODE_IWRAM static void _draw_left_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BN_CODE_IWRAM static void _draw_right_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BN_CODE_IWRAM static void _setup_attributes(const void* base_sprite_handle_ptr, const hline* hlines, int z_order,
                                                 int max_polygon_sprites, int minimum_y, int maximum_y,
                                                 uint16_t* hdma_source);
};

#endif
