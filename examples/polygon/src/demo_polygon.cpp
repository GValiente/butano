#include "demo_polygon.h"

#include "btn_random.h"

demo_polygon::demo_polygon(const btn::span<const btn::fixed_point>& vertices, btn::fixed x, btn::fixed y,
                           int graphics_index, int z_order) :
    _polygon(vertices, x, y, graphics_index, z_order)
{
}

void demo_polygon::update(btn::random& random)
{
    if(_vertex_actions.empty() || _vertex_actions.front().done())
    {
        _vertex_actions.clear();

        for(int index = 0; index < 4; ++index)
        {
            int x = -32 + int(random.get() % 64);
            int y = -32 + int(random.get() % 64);
            polygon_vertex vertex(index, _polygon);
            _vertex_actions.emplace_back(vertex, 120, btn::fixed_point(x, y));
        }
    }

    for(polygon_vertex_move_to_action& vertex_action : _vertex_actions)
    {
        vertex_action.update();
    }

    _polygon.update();
}
