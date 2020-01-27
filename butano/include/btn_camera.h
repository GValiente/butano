#ifndef BTN_CAMERA_H
#define BTN_CAMERA_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class fixed_point;
}

namespace btn::camera
{
    [[nodiscard]] fixed x();

    [[nodiscard]] fixed y();

    [[nodiscard]] const fixed_point& position();

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);
}

#endif
