/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_CAMERA_3D_H
#define FR_CAMERA_3D_H

#include "fr_point_3d.h"

namespace fr
{

class camera_3d
{

public:
    camera_3d();

    [[nodiscard]] const point_3d& position() const
    {
        return _position;
    }

    void set_position(const point_3d& position);

    [[nodiscard]] bn::fixed phi() const
    {
        return _phi;
    }

    void set_phi(bn::fixed phi);

    [[nodiscard]] const point_3d& u() const
    {
        return _u;
    }

    [[nodiscard]] const point_3d& v() const
    {
        return _v;
    }

private:
    point_3d _position;
    bn::fixed _phi;
    point_3d _u;
    point_3d _v;
};

}

#endif
