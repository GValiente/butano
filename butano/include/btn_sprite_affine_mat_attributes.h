#ifndef BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H
#define BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H

#include "btn_math.h"
#include "btn_algorithm.h"

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
        _horizontal_flip(horizontal_flip),
        _vertical_flip(vertical_flip)
    {
        BTN_CONSTEXPR_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle");
        BTN_CONSTEXPR_ASSERT(scale_x > 0, "Invalid scale x");
        BTN_CONSTEXPR_ASSERT(scale_y > 0, "Invalid scale y");

        _update_rotation_angle();
        _update_scale_x();
        _update_scale_y();
        _update_horizontal_flip();
        _update_vertical_flip();
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
    }

    constexpr void set_scale(fixed scale)
    {
        BTN_CONSTEXPR_ASSERT(scale > 0, "Invalid scale");

        _scale_x = scale;
        _scale_y = scale;
        _update_scale_x();
        _sy = _sx;
    }

    constexpr void set_scale(fixed scale_x, fixed scale_y)
    {
        BTN_CONSTEXPR_ASSERT(scale_x > 0, "Invalid scale x");
        BTN_CONSTEXPR_ASSERT(scale_y > 0, "Invalid scale y");

        _scale_x = scale_x;
        _scale_y = scale_y;
        _update_scale_x();
        _update_scale_y();
    }

    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _horizontal_flip = horizontal_flip;
        _update_horizontal_flip();
    }

    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vertical_flip;
    }

    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vertical_flip = vertical_flip;
        _update_vertical_flip();
    }

    [[nodiscard]] constexpr bool identity() const
    {
        return _rotation_angle == 0 && _scale_x == 1 && _scale_y == 1 && ! _horizontal_flip && ! _vertical_flip;
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
        int cos = _cos;
        int sx = _hflip * _sx;
        return cos * sx >> 12;
    }

    [[nodiscard]] constexpr int second_register_value() const
    {
        int sin = _sin;
        int sx = _hflip * _sx;
        return -sin * sx >> 12;
    }

    [[nodiscard]] constexpr int third_register_value() const
    {
        int sin = _sin;
        int sy = _vflip * _sy;
        return sin * sy >> 12;
    }

    [[nodiscard]] constexpr int fourth_register_value() const
    {
        int cos = _cos;
        int sy = _vflip * _sy;
        return cos * sy >> 12;
    }

    [[nodiscard]] constexpr friend bool operator==(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return a._rotation_angle == b._rotation_angle && a._scale_x == b._scale_x && a._scale_y == b._scale_y &&
                a._horizontal_flip == b._horizontal_flip && a._vertical_flip == b._vertical_flip;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return ! (a == b);
    }

private:
    fixed _rotation_angle = 0;
    fixed _scale_x = 1;
    fixed _scale_y = 1;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
    int16_t _sin = 0;
    int16_t _cos = int16_t(fixed(1).data());
    uint16_t _sx = uint16_t(fixed_t<8>(1).data());
    uint16_t _sy = uint16_t(fixed_t<8>(1).data());
    int8_t _hflip = 1;
    int8_t _vflip = 1;

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
        if(_scale_x == 1)
        {
            _sx = uint16_t(fixed_t<8>(1).data());
        }
        else
        {
            fixed inv_scale_x = 1 / _scale_x;
            _sx = uint16_t(fixed_t<8>(inv_scale_x).data());
        }
    }

    constexpr void _update_scale_y()
    {
        if(_scale_y == 1)
        {
            _sy = uint16_t(fixed_t<8>(1).data());
        }
        else
        {
            fixed inv_scale_y = 1 / _scale_y;
            _sy = uint16_t(fixed_t<8>(inv_scale_y).data());
        }
    }

    constexpr void _update_horizontal_flip()
    {
        _hflip = 1 - (2 * _horizontal_flip);
    }

    constexpr void _update_vertical_flip()
    {
        _vflip = 1 - (2 * _vertical_flip);
    }
};

}

#endif

