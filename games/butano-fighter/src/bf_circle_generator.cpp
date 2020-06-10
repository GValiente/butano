#include "bf_circle_generator.h"

#include "btn_span.h"
#include "btn_cstring.h"
#include "btn_display.h"

namespace bf
{

void circle_generator::set_radius(btn::fixed radius)
{
    BTN_ASSERT(radius >= 0, "Invalid radius: ", radius);

    _radius = radius;
}

void circle_generator::generate(btn::span<btn::pair<btn::fixed, btn::fixed>> values) const
{
    // https://www.coranac.com/tonc/text/dma.htm#sec-demo

    int values_count = values.size();

    if(! values_count)
    {
        return;
    }

    btn::fixed y0 = (btn::display::height() / 2) + _origin_y;
    btn::fixed x = 0;
    btn::fixed y = _radius;
    btn::fixed d = 1 - _radius;

    btn::pair<btn::fixed, btn::fixed>* values_data = values.data();
    btn::memclear(values_data, unsigned(values_count) * sizeof(btn::pair<btn::fixed, btn::fixed>));

    while(y >= x)
    {
        int top = (y0 - x).integer();
        int bottom = (y0 + x).integer();

        if(top >= 0 && top < values_count)
        {
            values_data[top] = btn::make_pair(-y, y);
        }

        if(bottom >= 0 && bottom < values_count)
        {
            values_data[bottom] = btn::make_pair(-y, y);
        }

        if(d >= 0)
        {
            top = (y0 - y).integer();
            bottom = (y0 + y).integer();

            if(top >= 0 && top < values_count)
            {
                values_data[top] = btn::make_pair(-x, x);
            }

            if(bottom >= 0 && bottom < values_count)
            {
                values_data[bottom] = btn::make_pair(-x, x);
            }

            y -= 1;
            d -= 2 * y;
        }

        d += 2 * x + 3;
        x += 1;
    }
}

}
