#ifndef BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H
#define BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H

#include "btn_math.h"
#include "btn_affine_mat_scale_lut.h"

namespace btn
{

class sprite_affine_mat_attributes
{

public:
    constexpr sprite_affine_mat_attributes() = default;

    constexpr sprite_affine_mat_attributes(fixed rotation_angle, fixed scale_x, fixed scale_y, bool horizontal_flip,
                                           bool vertical_flip) :
        _rotation_angle(rotation_angle),
        _scale_x(scale_x),
        _scale_y(scale_y),
        _hflip(1 - (2 * horizontal_flip)),
        _vflip(1 - (2 * vertical_flip))
    {
        BTN_CONSTEXPR_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle");
        BTN_CONSTEXPR_ASSERT(scale_x > 0, "Invalid scale x");
        BTN_CONSTEXPR_ASSERT(scale_y > 0, "Invalid scale y");

        _update_rotation_angle();
        _update_scale_x();
        _update_scale_y();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    [[nodiscard]] constexpr fixed rotation_angle() const
    {
        return _rotation_angle;
    }

    constexpr void set_rotation_angle(fixed rotation_angle)
    {
        BTN_CONSTEXPR_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle");

        _rotation_angle = rotation_angle;
        _update_rotation_angle();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    [[nodiscard]] constexpr fixed scale_x() const
    {
        return _scale_x;
    }

    constexpr void set_scale_x(fixed scale_x)
    {
        BTN_CONSTEXPR_ASSERT(scale_x > 0, "Invalid scale x");

        _scale_x = scale_x;
        _update_scale_x();
        _update_pa();
        _update_pb();
    }

    [[nodiscard]] constexpr fixed scale_y() const
    {
        return _scale_y;
    }

    constexpr void set_scale_y(fixed scale_y)
    {
        BTN_CONSTEXPR_ASSERT(scale_y > 0, "Invalid scale y");

        _scale_y = scale_y;
        _update_scale_y();
        _update_pc();
        _update_pd();
    }

    constexpr void set_scale(fixed scale)
    {
        BTN_CONSTEXPR_ASSERT(scale > 0, "Invalid scale");

        _scale_x = scale;
        _scale_y = scale;
        _update_scale_x();
        _sy = _sx;
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    constexpr void set_scale(fixed scale_x, fixed scale_y)
    {
        BTN_CONSTEXPR_ASSERT(scale_x > 0, "Invalid scale x");
        BTN_CONSTEXPR_ASSERT(scale_y > 0, "Invalid scale y");

        _scale_x = scale_x;
        _scale_y = scale_y;
        _update_scale_x();
        _update_scale_y();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _hflip == -1;
    }

    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _hflip = 1 - (2 * horizontal_flip);
        _update_pa();
        _update_pb();
    }

    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vflip == -1;
    }

    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vflip = 1 - (2 * vertical_flip);
        _update_pc();
        _update_pd();
    }

    [[nodiscard]] constexpr bool identity() const
    {
        return _rotation_angle == 0 && _scale_x == 1 && _scale_y == 1 && _hflip == 1 && _vflip == 1;
    }

    [[nodiscard]] constexpr bool double_size() const
    {
        fixed max_scale = max(_scale_x, _scale_y);
        int cos = abs(int(_cos));
        int sin = abs(int(_sin));
        int size = (cos + sin) * max_scale.data();
        return size > 256 << 16;
    }

    [[nodiscard]] constexpr int first_register_value() const
    {
        return _pa;
    }

    [[nodiscard]] constexpr int second_register_value() const
    {
        return _pb;
    }

    [[nodiscard]] constexpr int third_register_value() const
    {
        return _pc;
    }

    [[nodiscard]] constexpr int fourth_register_value() const
    {
        return _pd;
    }

    [[nodiscard]] constexpr friend bool operator==(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return a._rotation_angle == b._rotation_angle && a._scale_x == b._scale_x && a._scale_y == b._scale_y &&
                a._hflip == b._hflip && a._vflip == b._vflip;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return ! (a == b);
    }

private:
    constexpr static const btn::fixed min_inv_scale = 128;
    constexpr static const btn::fixed min_scale = 1 / min_inv_scale;

    fixed _rotation_angle = 0;
    fixed _scale_x = 1;
    fixed _scale_y = 1;
    int8_t _hflip = 1;
    int8_t _vflip = 1;
    int16_t _sin = 0;
    int16_t _cos = int16_t(fixed(1).data());
    uint16_t _sx = uint16_t(fixed_t<8>(1).data());
    uint16_t _sy = uint16_t(fixed_t<8>(1).data());
    int16_t _pa = 256;
    int16_t _pb = 0;
    int16_t _pc = 0;
    int16_t _pd = 256;

    [[nodiscard]] constexpr static uint16_t _output_scale(fixed scale)
    {
        if(scale == 1)
        {
            return uint16_t(fixed_t<8>(1).data());
        }

        if(scale <= min_scale)
        {
            return uint16_t(fixed_t<8>(min_inv_scale).data());
        }

        fixed_t<8> scale_8(scale);
        int scale_8_data = scale_8.data();

        if(scale_8_data < affine_mat_scale_lut.size())
        {
            return affine_mat_scale_lut.data()[scale_8_data];
        }

        int one = fixed_t<8>(1).data() * fixed_t<8>::scale();
        return uint16_t(one / scale_8_data);
    }

    constexpr void _update_rotation_angle()
    {
        if(_rotation_angle == 0)
        {
            _sin = 0;
            _cos = int16_t(fixed(1).data());
        }
        else
        {
            _sin = int16_t(degrees_sin(_rotation_angle).data());
            _cos = int16_t(degrees_cos(_rotation_angle).data());
        }
    }

    constexpr void _update_scale_x()
    {
        _sx = _output_scale(_scale_x);
    }

    constexpr void _update_scale_y()
    {
        _sy = _output_scale(_scale_y);
    }

    constexpr void _update_pa()
    {
        _pa = int16_t(_cos * (_sx * int(_hflip)) >> 12);
    }

    constexpr void _update_pb()
    {
        _pb = int16_t(-_sin * (_sx * int(_hflip)) >> 12);
    }

    constexpr void _update_pc()
    {
        _pc = int16_t(_sin * (_sy * int(_vflip)) >> 12);
    }

    constexpr void _update_pd()
    {
        _pd = int16_t(_cos * (_sy * int(_vflip)) >> 12);
    }
};

}

#endif
