#include "btn_camera.h"

#include "btn_fixed_point.h"
#include "btn_bgs_manager.h"
#include "btn_sprites_manager.h"

namespace btn::camera
{

namespace
{
    class static_data
    {

    public:
        fixed_point position;
    };

    BTN_DATA_EWRAM static_data data;
}

fixed x()
{
    return data.position.x();
}

fixed y()
{
    return data.position.y();
}

const fixed_point& position()
{
    return data.position;
}

void set_position(fixed x, fixed y)
{
    set_position(fixed_point(x, y));
}

void set_position(const fixed_point& position)
{
    data.position = position;
    sprites_manager::update_camera();
    bgs_manager::update_camera();
}

}
