#include "polygon_sprite.h"

#include "btn_math.h"
#include "btn_fixed_point.h"
#include "../../butano/hw/include/btn_hw_sprites.h"

void polygon_sprite::_draw_line(const btn::fixed_point& from, const btn::fixed_point& to, hline* hlines)
{
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    int x0 = from.x().integer();
    int y0 = from.y().integer();
    int x1 = to.x().integer();
    int y1 = to.y().integer();
    int dy = -btn::abs(y1 - y0);

    if(dy == 0)
    {
        hline& hline = hlines[y0];

        if(x0 < x1)
        {
            if(x0 < hline.ixl)
            {
                hline.ixl = x0;
            }

            if(x1 > hline.ixr)
            {
                hline.ixr = x1;
            }
        }
        else
        {
            if(x1 < hline.ixl)
            {
                hline.ixl = x1;
            }

            if(x0 > hline.ixr)
            {
                hline.ixr = x0;
            }
        }

        return;
    }

    int dx = btn::abs(x1 - x0);
    int sy = y0 < y1 ? 1 : -1;

    if(dx == 0)
    {
        while(true)
        {
            hline& hline = hlines[y0];

            if(x0 < hline.ixl)
            {
                hline.ixl = x0;
            }

            if(x0 > hline.ixr)
            {
                hline.ixr = x0;
            }

            if(y0 == y1)
            {
                return;
            }

            y0 += sy;
        }
    }

    int sx = x0 < x1 ? 1 : -1;
    int err = dx + dy;  // error value e_xy

    while(true)
    {
        hline& hline = hlines[y0];

        if(x0 < hline.ixl)
        {
            hline.ixl = x0;
        }

        if(x0 > hline.ixr)
        {
            hline.ixr = x0;
        }

        if(x0 == x1 && y0 == y1)
        {
            return;
        }

        int e2 = err * 2;

        if(e2 >= dy) // e_xy + e_x > 0
        {
            err += dy;
            x0 += sx;
        }

        if(e2 <= dx) // e_xy + e_y < 0
        {
            err += dx;
            y0 += sy;
        }
    }
}

void polygon_sprite::_setup_attributes(const void* base_sprite_handle_ptr, const hline* hlines, int z_order,
                                       int max_polygon_sprites, uint16_t* hdma_source)
{
    auto typed_base_sprite_handle_ptr = static_cast<const btn::hw::sprites::handle_type*>(base_sprite_handle_ptr);
    btn::hw::sprites::handle_type base_sprite_handle = *typed_base_sprite_handle_ptr;
    uint16_t* sprite_hdma_source = hdma_source + (z_order * 4);

    for(int index = 0; index < btn::display::height(); ++index)
    {
        const hline& hline = hlines[index];
        int ixl = hline.ixl;
        int length = hline.ixr - ixl;

        if(length > 0)
        {
            sprite_hdma_source[0] = base_sprite_handle.attr0;
            btn::hw::sprites::set_y(index - length + 1, sprite_hdma_source[0]);
            sprite_hdma_source[1] = base_sprite_handle.attr1;
            btn::hw::sprites::set_x(ixl, sprite_hdma_source[1]);
            sprite_hdma_source[2] = base_sprite_handle.attr2;
        }
        else
        {
            btn::hw::sprites::set_y(btn::display::height(), sprite_hdma_source[0]);
        }

        sprite_hdma_source += max_polygon_sprites * 4;
    }
}
