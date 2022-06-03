#ifndef POLYGON_SPRITE_H
#define POLYGON_SPRITE_H

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_display.h"
#include "bn_sprite_position_hbe_ptr.h"

class polygon;

class polygon_sprite
{

public:
    polygon_sprite(const polygon& polygon, int graphics_index, int z_order);

    polygon_sprite(const bn::span<const polygon*>& polygons, int graphics_index, int z_order);

    polygon_sprite(const polygon_sprite& other) = delete;

    polygon_sprite& operator=(const polygon_sprite& other) = delete;

    void reload_polygons()
    {
        _update = true;
    }

    void update();

private:
    struct hline
    {
        int ixl = bn::display::width();
        int ixr = 0;
    };

    bn::vector<const polygon*, 2> _polygons;
    bn::sprite_ptr _sprite;
    bn::array<bn::fixed, bn::display::height()> _vertical_values;
    bn::sprite_position_hbe_ptr _vertical_hbe;
    bn::array<bn::fixed, bn::display::height()> _horizontal_values;
    bn::sprite_position_hbe_ptr _horizontal_hbe;
    bool _update = true;

    static void _draw_not_horizontal_line(bool left, int x0, int y0, int x1, int y1, hline* hlines);

    static void _draw_horizontal_line(int x0, int x1, int y, hline* hlines);

    BN_CODE_IWRAM static void _draw_left_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BN_CODE_IWRAM static void _draw_right_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BN_CODE_IWRAM static void _draw_left_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                        hline* hlines);

    BN_CODE_IWRAM static void _draw_right_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction,
                                                         hline* hlines);

    BN_CODE_IWRAM static void _setup_attributes(const hline* hlines, bn::fixed* vertical_values,
                                                 bn::fixed* horizontal_values);
};

#endif
