#include "polygon_sprite.h"

#include "btn_math.h"
#include "btn_fixed_point.h"

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
        hline.ixl = btn::min(hline.ixl, x0);
        hline.ixr = btn::max(hline.ixr, x0);
        hline.ixl = btn::min(hline.ixl, x1);
        hline.ixr = btn::max(hline.ixr, x1);
        return;
    }

    int dx = btn::abs(x1 - x0);
    int sy = y0 < y1 ? 1 : -1;

    if(dx == 0)
    {
        while(true)
        {
            hline& hline = hlines[y0];
            hline.ixl = btn::min(hline.ixl, x0);
            hline.ixr = btn::max(hline.ixr, x0);

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
        hline.ixl = btn::min(hline.ixl, x0);
        hline.ixr = btn::max(hline.ixr, x0);

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

void polygon_sprite::_setup_attributes(const hline* hlines, btn::sprite_first_attributes* first_attributes,
                                       btn::sprite_regular_second_attributes* second_attributes)
{
    for(int index = 0; index < btn::display::height(); ++index)
    {
        const hline& hline = hlines[index];
        int ixl = hline.ixl;
        int length = hline.ixr - ixl;

        if(length > 0)
        {
            first_attributes[index].set_y(-length + index + 1);
            second_attributes[index].set_x(ixl);
        }
        else
        {
            first_attributes[index].set_y(btn::display::height());
        }
    }
}
