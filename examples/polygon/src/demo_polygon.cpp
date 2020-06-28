#include "demo_polygon.h"

#include "btn_random.h"

namespace
{
    const int duration_frames = 120;
}

demo_polygon::demo_polygon(const btn::sprite_item& sprite_item, btn::fixed x, btn::fixed y, int z_order) :
    _polygon(sprite_item, x, y, z_order)
{
}

void demo_polygon::update(btn::random& random)
{
    if(! _scale_x_action || _scale_x_action->done())
    {
        btn::fixed final_panning(-0.99);
        final_panning += btn::fixed::from_data(random.get() % btn::fixed(1.88).data());
        _panning_action.emplace(_polygon, duration_frames, final_panning);

        btn::fixed final_scale_x(0.5);
        final_scale_x += btn::fixed::from_data(random.get() % btn::fixed(0.5).data());
        _scale_x_action.emplace(_polygon, duration_frames, final_scale_x);

        btn::fixed final_scale_y(0.5);
        final_scale_y += btn::fixed::from_data(random.get() % btn::fixed(0.5).data());
        _scale_y_action.emplace(_polygon, duration_frames, final_scale_y);
    }

    _panning_action->update();
    _scale_x_action->update();
    _scale_y_action->update();
    _polygon.update();
}
