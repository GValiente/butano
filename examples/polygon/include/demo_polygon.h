#ifndef DEMO_POLYGON_H
#define DEMO_POLYGON_H

#include "btn_optional.h"
#include "polygon_actions.h"

namespace btn
{
    class random;
}

class demo_polygon
{

public:
    demo_polygon(const btn::sprite_item& sprite_item, btn::fixed x, btn::fixed y, int z_order);

    void update(btn::random& random);

private:
    polygon _polygon;
    btn::optional<polygon_panning_to_action> _panning_action;
    btn::optional<polygon_scale_x_to_action> _scale_x_action;
    btn::optional<polygon_scale_y_to_action> _scale_y_action;
};

#endif
