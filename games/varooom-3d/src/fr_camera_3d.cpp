/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_camera_3d.h"

#include "bn_math.h"

#include "fr_sin_cos.h"

namespace fr
{

camera_3d::camera_3d() :
    _position(0, 256, 0)
{
    set_phi(0);
}

void camera_3d::set_position(const point_3d& position)
{
    BN_ASSERT(position.y() >= 2, "Invalid y: ", position.y());

    _position = position;
}

void camera_3d::set_phi(bn::fixed phi)
{
    if(phi > 0xFFFF)
    {
        phi -= 0xFFFF;
    }
    else if(phi < 0)
    {
        phi += 0xFFFF;
    }

    BN_ASSERT(phi >= 0 && phi <= 0xFFFF, "Invalid phi: ", phi);

    int angle = phi.right_shift_integer();
    bn::fixed sf = sin(angle);
    bn::fixed cf = cos(angle);
    _phi = phi;

    _u.set_x(cf);
    _u.set_z(sf);

    _v.set_x(sf);
    _v.set_z(-cf);
}

}
