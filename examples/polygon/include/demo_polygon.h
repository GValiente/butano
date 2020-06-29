#ifndef DEMO_POLYGON_H
#define DEMO_POLYGON_H

#include "polygon_actions.h"

namespace btn
{
    class random;
}

class demo_polygon
{

public:
    demo_polygon(const btn::span<const btn::fixed_point>& vertices, btn::fixed x, btn::fixed y, int graphics_index,
                 int z_order);

    void update(btn::random& random);

private:
    polygon _polygon;
    btn::vector<polygon_vertex_move_to_action, 4> _vertex_actions;
};

#endif
