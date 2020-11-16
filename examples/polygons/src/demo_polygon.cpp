#include "demo_polygon.h"

#include "bn_random.h"

namespace
{
    BN_DATA_EWRAM bn::array<bn::fixed_point, 4> _array = {};

    [[nodiscard]] bn::span<const bn::fixed_point> _build_array(bn::fixed x, bn::fixed y)
    {
        _array[0] = bn::fixed_point(x,         y);
        _array[1] = bn::fixed_point(x + 64,    y);
        _array[2] = bn::fixed_point(x + 64,    y + 78);
        _array[3] = bn::fixed_point(x,         y + 78);
        return bn::span<const bn::fixed_point>(_array.data(), _array.size());
    }
}

demo_polygon::demo_polygon(bn::fixed x, bn::fixed y) :
    polygon(_build_array(x, y)),
    _x(x),
    _y(y)
{
}

void demo_polygon::update(bn::random& random)
{
    if(_vertex_actions.empty() || _vertex_actions.front().done())
    {
        _vertex_actions.clear();

        int x = _x.right_shift_integer() + int(random.get() % 32);
        int y = _y.right_shift_integer() + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(0, *this), 32, bn::fixed_point(x, y));

        x = _x.right_shift_integer() + 32 + int(random.get() % 32);
        y = _y.right_shift_integer() + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(1, *this), 32, bn::fixed_point(x, y));

        x = _x.right_shift_integer() + 32 + int(random.get() % 32);
        y = _y.right_shift_integer() + 39 + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(2, *this), 32, bn::fixed_point(x, y));

        x = _x.right_shift_integer() + int(random.get() % 32);
        y = _y.right_shift_integer() + 39 + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(3, *this), 32, bn::fixed_point(x, y));
    }

    for(polygon_vertex_move_to_action& vertex_action : _vertex_actions)
    {
        vertex_action.update();
    }
}
