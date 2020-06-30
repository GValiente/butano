#include "demo_polygon.h"

#include "btn_random.h"

namespace
{
    BTN_DATA_EWRAM btn::array<btn::fixed_point, 4> _array = {};

    [[nodiscard]] btn::span<const btn::fixed_point> _build_array(btn::fixed x, btn::fixed y)
    {
        _array[0] = btn::fixed_point(x,         y);
        _array[1] = btn::fixed_point(x + 64,    y);
        _array[2] = btn::fixed_point(x + 64,    y + 78);
        _array[3] = btn::fixed_point(x,         y + 78);
        return btn::span<const btn::fixed_point>(_array.data(), _array.size());
    }
}

demo_polygon::demo_polygon(btn::fixed x, btn::fixed y) :
    polygon(_build_array(x, y)),
    _x(x),
    _y(y)
{
}

void demo_polygon::update(btn::random& random)
{
    if(_vertex_actions.empty() || _vertex_actions.front().done())
    {
        _vertex_actions.clear();

        int x = _x.integer() + int(random.get() % 32);
        int y = _y.integer() + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(0, *this), 32, btn::fixed_point(x, y));

        x = _x.integer() + 32 + int(random.get() % 32);
        y = _y.integer() + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(1, *this), 32, btn::fixed_point(x, y));

        x = _x.integer() + 32 + int(random.get() % 32);
        y = _y.integer() + 39 + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(2, *this), 32, btn::fixed_point(x, y));

        x = _x.integer() + int(random.get() % 32);
        y = _y.integer() + 39 + int(random.get() % 32);
        _vertex_actions.emplace_back(polygon_vertex(3, *this), 32, btn::fixed_point(x, y));
    }

    for(polygon_vertex_move_to_action& vertex_action : _vertex_actions)
    {
        vertex_action.update();
    }
}
