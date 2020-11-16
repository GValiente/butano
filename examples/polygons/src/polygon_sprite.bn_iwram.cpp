#include "polygon_sprite.h"

void polygon_sprite::_draw_not_horizontal_line(bool left, int x0, int y0, int x1, int y1, hline* hlines)
{
    // https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-35.md

    if(y0 > y1)
    {
        bn::swap(x0, x1);
        bn::swap(y0, y1);
    }

    hline* current_hline = hlines + y0;
    const hline* last_hline = hlines + y1;
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    if(delta_x == 0)
    {
        int sy = delta_y > 0 ? 1 : -1;

        if(left)
        {
            while(true)
            {
                current_hline->ixl = x0;

                if(current_hline == last_hline)
                {
                    return;
                }

                current_hline += sy;
            }
        }
        else
        {
            while(true)
            {
                current_hline->ixr = x0;

                if(current_hline == last_hline)
                {
                    return;
                }

                current_hline += sy;
            }
        }
    }

    int x_direction;

    if(delta_x > 0)
    {
        x_direction = 1;
    }
    else
    {
        delta_x = -delta_x;
        x_direction = -1;
    }

    if(left)
    {
        if(delta_x > delta_y)
        {
            _draw_left_line_octant_0(x0, y0, delta_x, delta_y, x_direction, hlines);
        }
        else
        {
            _draw_left_line_octant_1(x0, y0, delta_x, delta_y, x_direction, hlines);
        }
    }
    else
    {
        if(delta_x > delta_y)
        {
            _draw_right_line_octant_0(x0, y0, delta_x, delta_y, x_direction, hlines);
        }
        else
        {
            _draw_right_line_octant_1(x0, y0, delta_x, delta_y, x_direction, hlines);
        }
    }
}

void polygon_sprite::_draw_horizontal_line(int x0, int x1, int y, hline* hlines)
{
    hline* current_hline = hlines + y;

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
}

void polygon_sprite::_draw_left_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction, hline* hlines)
{
    // https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-35.md

    hline* current_hline = hlines + y0;
    current_hline->ixl = x0;

    int delta_y2 = delta_y * 2;
    int delta_y2_minus_delta_x2 = delta_y2 - (delta_x * 2);
    int error_term = delta_y2 - delta_x;
    bool x_direction_positive = x_direction > 0;

    while(delta_x--)
    {
        bool new_line = error_term >= 0;

        if(new_line)
        {
            ++current_hline;
            error_term += delta_y2_minus_delta_x2;
            x0 += x_direction;
            current_hline->ixl = x0;
        }
        else
        {
            error_term += delta_y2;
            x0 += x_direction;

            if(x_direction_positive)
            {
                //
            }
            else
            {
                current_hline->ixl = x0;
            }
        }
    }
}

void polygon_sprite::_draw_right_line_octant_0(int x0, int y0, int delta_x, int delta_y, int x_direction, hline* hlines)
{
    // https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-35.md

    hline* current_hline = hlines + y0;
    current_hline->ixr = x0;

    int delta_y2 = delta_y * 2;
    int delta_y2_minus_delta_x2 = delta_y2 - (delta_x * 2);
    int error_term = delta_y2 - delta_x;
    bool x_direction_positive = x_direction > 0;

    while(delta_x--)
    {
        bool new_line = error_term >= 0;

        if(new_line)
        {
            ++current_hline;
            error_term += delta_y2_minus_delta_x2;
            x0 += x_direction;
            current_hline->ixr = x0;
        }
        else
        {
            error_term += delta_y2;
            x0 += x_direction;

            if(x_direction_positive)
            {
                current_hline->ixr = x0;
            }
            else
            {
                //
            }
        }
    }
}

void polygon_sprite::_draw_left_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction, hline* hlines)
{
    // https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-35.md

    hline* current_hline = hlines + y0;
    current_hline->ixl = x0;

    int delta_x2 = delta_x * 2;
    int delta_y2_minus_delta_x2 = delta_x2 - (delta_y * 2);
    int error_term = delta_x2 - delta_y;

    while(delta_y--)
    {
        if(error_term >= 0)
        {
            x0 += x_direction;
            error_term += delta_y2_minus_delta_x2;
        }
        else
        {
            error_term += delta_x2;
        }

        ++current_hline;
        current_hline->ixl = x0;
    }
}

void polygon_sprite::_draw_right_line_octant_1(int x0, int y0, int delta_x, int delta_y, int x_direction, hline* hlines)
{
    // https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-35.md

    hline* current_hline = hlines + y0;
    current_hline->ixr = x0;

    int delta_x2 = delta_x * 2;
    int delta_y2_minus_delta_x2 = delta_x2 - (delta_y * 2);
    int error_term = delta_x2 - delta_y;

    while(delta_y--)
    {
        if(error_term >= 0)
        {
            x0 += x_direction;
            error_term += delta_y2_minus_delta_x2;
        }
        else
        {
            error_term += delta_x2;
        }

        ++current_hline;
        current_hline->ixr = x0;
    }
}

void polygon_sprite::_setup_attributes(const hline* hlines, bn::fixed* vertical_values, bn::fixed* horizontal_values)
{
    bn::fixed invalid_y = bn::display::height();

    for(int index = 0; index < bn::display::height(); ++index)
    {
        const hline& hline = hlines[index];
        int ixl = hline.ixl;
        int length = hline.ixr - ixl;

        if(length > 0)
        {
            vertical_values[index] = -length + index + 1;
            horizontal_values[index] = ixl;
        }
        else
        {
            vertical_values[index] = invalid_y;
        }
    }
}
