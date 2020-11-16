#ifndef DEMO_POLYGON_H
#define DEMO_POLYGON_H

#include "polygon_actions.h"

namespace bn
{
    class random;
}

class demo_polygon : public polygon
{

public:
    demo_polygon(bn::fixed x, bn::fixed y);

    void update(bn::random& random);

private:
    bn::vector<polygon_vertex_move_to_action, 4> _vertex_actions;
    bn::fixed _x;
    bn::fixed _y;
};

#endif
