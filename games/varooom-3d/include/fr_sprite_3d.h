/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SPRITE_3D_H
#define FR_SPRITE_3D_H

#include "bn_math.h"
#include "bn_intrusive_list.h"

#include "fr_sin_cos.h"
#include "fr_point_3d.h"

namespace fr
{

class sprite_3d_item;

class sprite_3d : public bn::intrusive_list_node_type
{

public:
    explicit sprite_3d(sprite_3d_item& item) :
        _item(item)
    {
    }

    [[nodiscard]] const sprite_3d_item& item() const
    {
        return _item;
    }

    [[nodiscard]] sprite_3d_item& item()
    {
        return _item;
    }

    [[nodiscard]] constexpr const point_3d& position() const
    {
        return _position;
    }

    constexpr void set_position(const point_3d& position)
    {
        _position = position;
    }

    [[nodiscard]] constexpr bn::fixed scale() const
    {
        return _scale;
    }

    constexpr void set_scale(bn::fixed scale)
    {
        BN_ASSERT(scale > 0, "Invalid scale: ", scale);

        _scale = scale;
    }

    [[nodiscard]] constexpr bn::fixed theta() const
    {
        return _theta;
    }

   constexpr void set_theta(bn::fixed theta)
    {
        if(theta > 0xFFFF)
        {
            theta -= 0xFFFF;
        }
        else if(theta < 0)
        {
            theta += 0xFFFF;
        }

        BN_ASSERT(theta >= 0 && theta <= 0xFFFF, "Invalid theta: ", theta);

        int old_angle = _theta.right_shift_integer();
        int new_angle = theta.right_shift_integer();
        _theta = theta;

        if(old_angle != new_angle)
        {
            _theta_sin = sin(new_angle);
            _theta_cos = cos(new_angle);
        }
    }

    [[nodiscard]] constexpr point_3d rotate(const vertex_3d& vertex) const
    {
        bn::fixed theta_sin = _theta_sin;
        bn::fixed theta_cos = _theta_cos;
        bn::fixed vx = vertex.point().x();
        bn::fixed vy = vertex.point().y();
        bn::fixed vz = vertex.point().z();
        bn::fixed vxy = vertex.xy();
        bn::fixed rx = (theta_cos + vy).safe_multiplication(vx) + vz.unsafe_multiplication(theta_sin) - vxy;
        bn::fixed ry = vy.safe_multiplication(1 + vx) - vxy;
        bn::fixed rz = (-theta_sin + vy).safe_multiplication(vx) + vz.unsafe_multiplication(theta_cos) - vxy;

        return point_3d(rx, ry, rz);
    }

    [[nodiscard]] constexpr point_3d transform(const vertex_3d& vertex) const
    {
        point_3d result = rotate(vertex);
        bn::fixed scale = _scale;

        if(scale != 1)
        {
            result.set_x(result.x().unsafe_multiplication(scale));
            result.set_y(result.y().unsafe_multiplication(scale));
            result.set_z(result.z().unsafe_multiplication(scale));
        }

        return result + _position;
    }

private:
    sprite_3d_item& _item;
    point_3d _position;
    bn::fixed _scale = 1;
    bn::fixed _theta;
    bn::fixed _theta_sin;
    bn::fixed _theta_cos = 1;
};

}

#endif
