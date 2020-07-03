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

    hline* current_hline = hlines + y0;

    if(dy == 0)
    {
        if(x0 < x1)
        {
            if(x0 < current_hline->ixl)
            {
                current_hline->ixl = x0;
            }

            if(x1 > current_hline->ixr)
            {
                current_hline->ixr = x1;
            }
        }
        else
        {
            if(x1 < current_hline->ixl)
            {
                current_hline->ixl = x1;
            }

            if(x0 > current_hline->ixr)
            {
                current_hline->ixr = x0;
            }
        }

        return;
    }

    const hline* last_hline = hlines + y1;
    int dx = btn::abs(x1 - x0);
    int sy = y0 < y1 ? 1 : -1;

    if(dx == 0)
    {
        while(true)
        {
            if(x0 < current_hline->ixl)
            {
                current_hline->ixl = x0;
            }

            if(x0 > current_hline->ixr)
            {
                current_hline->ixr = x0;
            }

            if(current_hline == last_hline)
            {
                return;
            }

            current_hline += sy;
        }
    }

    int sx = x0 < x1 ? 1 : -1;
    int err = dx + dy;  // error value e_xy

    while(true)
    {
        if(x0 < current_hline->ixl)
        {
            current_hline->ixl = x0;
        }

        if(x0 > current_hline->ixr)
        {
            current_hline->ixr = x0;
        }

        if(x0 == x1 && current_hline == last_hline)
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
            current_hline += sy;
        }
    }
}

void polygon_sprite::_setup_attributes(const void* base_sprite_handle_ptr, const hline* hlines, int z_order,
                                       int max_polygon_sprites, int minimum_y, int maximum_y, uint16_t* hdma_source)
{
    auto typed_base_sprite_handle_ptr = static_cast<const btn::hw::sprites::handle_type*>(base_sprite_handle_ptr);
    btn::hw::sprites::handle_type base_sprite_handle = *typed_base_sprite_handle_ptr;
    uint16_t* sprite_hdma_source = hdma_source + (z_order * 4);
    sprite_hdma_source += minimum_y * max_polygon_sprites * 4;

    for(int index = minimum_y; index <= maximum_y; ++index)
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
            btn::hw::sprites::hide(sprite_hdma_source[0]);
        }

        sprite_hdma_source += max_polygon_sprites * 4;
    }
}
