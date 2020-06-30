#ifndef DEMO_POLYGON_H
#define DEMO_POLYGON_H

#include "polygon_actions.h"

namespace btn
{
    class random;
}

class demo_polygon : public polygon
{

public:
    demo_polygon(btn::fixed x, btn::fixed y);

    void update(btn::random& random);

private:
    btn::vector<polygon_vertex_move_to_action, 4> _vertex_actions;
    btn::fixed _x;
    btn::fixed _y;
};

#endif
