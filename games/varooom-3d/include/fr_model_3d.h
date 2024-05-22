/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_3D_H
#define FR_MODEL_3D_H

#include "bn_intrusive_list.h"

#include "fr_sin_cos.h"
#include "fr_model_3d_item.h"

namespace fr
{

class model_3d : public bn::intrusive_list_node_type
{

public:
    constexpr explicit model_3d(const model_3d_item& item) :
        _item(item)
    {
    }

    [[nodiscard]] constexpr const model_3d_item& item() const
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

    [[nodiscard]] constexpr bn::fixed phi() const
    {
        return _phi;
    }

    constexpr void set_phi(bn::fixed phi)
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

        int old_angle = _phi.right_shift_integer();
        int new_angle = phi.right_shift_integer();
        _phi = phi;

        if(old_angle != new_angle)
        {
            _phi_sin = sin(new_angle);
            _phi_cos = cos(new_angle);
            _update = true;
        }
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
            _update = true;
        }
    }

    [[nodiscard]] constexpr bn::fixed psi() const
    {
        return _psi;
    }

    constexpr void set_psi(bn::fixed psi)
    {
        if(psi > 0xFFFF)
        {
            psi -= 0xFFFF;
        }
        else if(psi < 0)
        {
            psi += 0xFFFF;
        }

        BN_ASSERT(psi >= 0 && psi <= 0xFFFF, "Invalid psi: ", psi);

        int old_angle = _psi.right_shift_integer();
        int new_angle = psi.right_shift_integer();
        _psi = psi;

        if(old_angle != new_angle)
        {
            _psi_sin = sin(new_angle);
            _psi_cos = cos(new_angle);
            _update = true;
        }
    }

    [[nodiscard]] constexpr point_3d rotate(const vertex_3d& vertex) const
    {
        bn::fixed vx = vertex.point().x();
        bn::fixed vy = vertex.point().y();
        bn::fixed vz = vertex.point().z();
        bn::fixed vxy = vertex.xy();
        bn::fixed rx = (_xx + vy).safe_multiplication(_xy + vx) + vz.unsafe_multiplication(_xz) - _xx_xy - vxy;
        bn::fixed ry = (_yx + vy).safe_multiplication(_yy + vx) + vz.unsafe_multiplication(_yz) - _yx_yy - vxy;
        bn::fixed rz = (_zx + vy).safe_multiplication(_zy + vx) + vz.unsafe_multiplication(_zz) - _zx_zy - vxy;

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

    constexpr void update()
    {
        if(! _update)
        {
            return;
        }

        bn::fixed phi_sin = _phi_sin;
        bn::fixed phi_cos = _phi_cos;
        bn::fixed theta_sin = _theta_sin;
        bn::fixed theta_cos = _theta_cos;
        bn::fixed psi_sin = _psi_sin;
        bn::fixed psi_cos = _psi_cos;
        _update = false;

        bn::fixed phi_cos_theta_sin = phi_cos.unsafe_multiplication(theta_sin);
        _xx = phi_cos.unsafe_multiplication(theta_cos);
        _xy = phi_cos_theta_sin.unsafe_multiplication(psi_sin) - phi_sin.unsafe_multiplication(psi_cos);
        _xz = phi_cos_theta_sin.unsafe_multiplication(psi_cos) + phi_sin.unsafe_multiplication(psi_sin);

        bn::fixed phi_sin_theta_sin = phi_sin.unsafe_multiplication(theta_sin);
        _yx = phi_sin.unsafe_multiplication(theta_cos);
        _yy = phi_sin_theta_sin.unsafe_multiplication(psi_sin) + phi_cos.unsafe_multiplication(psi_cos);
        _yz = phi_sin_theta_sin.unsafe_multiplication(psi_cos) - phi_cos.unsafe_multiplication(psi_sin);

        _zx = -theta_sin;
        _zy = theta_cos.unsafe_multiplication(psi_sin);
        _zz = theta_cos.unsafe_multiplication(psi_cos);

        _xx_xy = _xx.unsafe_multiplication(_xy);
        _yx_yy = _yx.unsafe_multiplication(_yy);
        _zx_zy = _zx.unsafe_multiplication(_zy);
    }

private:
    const model_3d_item& _item;
    point_3d _position;
    bn::fixed _scale = 1;
    bn::fixed _phi;
    bn::fixed _phi_sin;
    bn::fixed _phi_cos = 1;
    bn::fixed _theta;
    bn::fixed _theta_sin;
    bn::fixed _theta_cos = 1;
    bn::fixed _psi;
    bn::fixed _psi_sin;
    bn::fixed _psi_cos = 1;
    bn::fixed _xx;
    bn::fixed _xy;
    bn::fixed _xz;
    bn::fixed _yx;
    bn::fixed _yy;
    bn::fixed _yz;
    bn::fixed _zx;
    bn::fixed _zy;
    bn::fixed _zz;
    bn::fixed _xx_xy;
    bn::fixed _yx_yy;
    bn::fixed _zx_zy;
    bool _update = true;
};

}

#endif
