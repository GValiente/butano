/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H
#define BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_H

/**
 * @file
 * btn::sprite_affine_mat_attributes header file.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 */

#include "btn_math.h"
#include "btn_affine_mat_scale_lut.h"

namespace btn
{

/**
 * @brief Contains the attributes which define a sprite affine transformation matrix
 * (created with sprite_affine_mat_ptr static constructors).
 *
 * @ingroup sprite
 * @ingroup affine_mat
 */
class sprite_affine_mat_attributes
{

public:
    /**
     * @brief Identity constructor.
     */
    constexpr sprite_affine_mat_attributes() = default;

    /**
     * @brief Constructor.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     * @param horizontal_flip Indicates if this matrix is flipped in the horizontal axis or not.
     * @param vertical_flip Indicates if this matrix is flipped in the vertical axis or not.
     */
    constexpr sprite_affine_mat_attributes(fixed rotation_angle, fixed horizontal_scale, fixed vertical_scale,
                                           bool horizontal_flip, bool vertical_flip) :
        _rotation_angle(rotation_angle),
        _horizontal_scale(horizontal_scale),
        _vertical_scale(vertical_scale),
        _hflip(1 - (2 * horizontal_flip)),
        _vflip(1 - (2 * vertical_flip))
    {
        BTN_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle: ", rotation_angle);
        BTN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);
        BTN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

        _update_rotation_angle();
        _update_horizontal_scale();
        _update_vertical_scale();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Returns the rotation angle in degrees.
     */
    [[nodiscard]] constexpr fixed rotation_angle() const
    {
        return _rotation_angle;
    }

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     */
    constexpr void set_rotation_angle(fixed rotation_angle)
    {
        BTN_ASSERT(rotation_angle >= 0 && rotation_angle <= 360, "Invalid rotation angle: ", rotation_angle);

        _rotation_angle = rotation_angle;
        _update_rotation_angle();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Returns the horizontal scale.
     */
    [[nodiscard]] constexpr fixed horizontal_scale() const
    {
        return _horizontal_scale;
    }

    /**
     * @brief Sets the horizontal scale.
     */
    constexpr void set_horizontal_scale(fixed horizontal_scale)
    {
        BTN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);

        _horizontal_scale = horizontal_scale;
        _update_horizontal_scale();
        _update_pa();
        _update_pb();
    }

    /**
     * @brief Returns the vertical scale.
     */
    [[nodiscard]] constexpr fixed vertical_scale() const
    {
        return _vertical_scale;
    }

    /**
     * @brief Sets the vertical scale.
     */
    constexpr void set_vertical_scale(fixed vertical_scale)
    {
        BTN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

        _vertical_scale = vertical_scale;
        _update_vertical_scale();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Sets the scale.
     */
    constexpr void set_scale(fixed scale)
    {
        BTN_ASSERT(scale > 0, "Invalid scale: ", scale);

        _horizontal_scale = scale;
        _vertical_scale = scale;
        _update_horizontal_scale();
        _sy = _sx;
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Sets the scale.
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     */
    constexpr void set_scale(fixed horizontal_scale, fixed vertical_scale)
    {
        BTN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);
        BTN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

        _horizontal_scale = horizontal_scale;
        _vertical_scale = vertical_scale;
        _update_horizontal_scale();
        _update_vertical_scale();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Indicates if this matrix is flipped in the horizontal axis or not.
     */
    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _hflip == -1;
    }

    /**
     * @brief Sets if this matrix is flipped in the horizontal axis or not.
     */
    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _hflip = 1 - (2 * horizontal_flip);
        _update_pa();
        _update_pb();
    }

    /**
     * @brief Indicates if this matrix is flipped in the vertical axis or not.
     */
    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vflip == -1;
    }

    /**
     * @brief Sets if this matrix is flipped in the vertical axis or not.
     */
    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vflip = 1 - (2 * vertical_flip);
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not.
     */
    [[nodiscard]] constexpr bool identity() const
    {
        return flipped_identity() && _hflip == 1 && _vflip == 1;
    }

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not, ignoring flip attributes.
     */
    [[nodiscard]] constexpr bool flipped_identity() const
    {
        return _rotation_angle == 0 && _horizontal_scale == 1 && _vertical_scale == 1;
    }

    /**
     * @brief Indicates if a sprite_ptr attached to a sprite_affine_mat_ptr with these attributes
     * needs more pixels than the area specified by its shape and size to be drawn without clipping issues.
     */
    [[nodiscard]] constexpr bool double_size() const
    {
        fixed max_scale = max(_horizontal_scale, _vertical_scale);
        int cos = abs(int(_cos));
        int sin = abs(int(_sin));
        int size = (cos + sin) * max_scale.data();
        return size > 256 << 16;
    }

    /**
     * @brief Returns the value to commit to the first GBA register of a sprite_affine_mat_ptr with these attributes.
     */
    [[nodiscard]] constexpr int pa_register_value() const
    {
        return _pa;
    }

    /**
     * @brief Returns the value to commit to the second GBA register of a sprite_affine_mat_ptr with these attributes.
     */
    [[nodiscard]] constexpr int pb_register_value() const
    {
        return _pb;
    }

    /**
     * @brief Returns the value to commit to the third GBA register of a sprite_affine_mat_ptr with these attributes.
     */
    [[nodiscard]] constexpr int pc_register_value() const
    {
        return _pc;
    }

    /**
     * @brief Returns the value to commit to the fourth GBA register of a sprite_affine_mat_ptr with these attributes.
     */
    [[nodiscard]] constexpr int pd_register_value() const
    {
        return _pd;
    }

    /**
     * @brief Equal operator.
     * @param a First sprite_affine_mat_attributes to compare.
     * @param b Second sprite_affine_mat_attributes to compare.
     * @return `true` if the first sprite_affine_mat_attributes is equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return a._rotation_angle == b._rotation_angle && a._horizontal_scale == b._horizontal_scale &&
                a._vertical_scale == b._vertical_scale && a._hflip == b._hflip && a._vflip == b._vflip;
    }

    /**
     * @brief Not equal operator.
     * @param a First sprite_affine_mat_attributes to compare.
     * @param b Second sprite_affine_mat_attributes to compare.
     * @return `true` if the first sprite_affine_mat_attributes is not equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const sprite_affine_mat_attributes& a,
                                                   const sprite_affine_mat_attributes& b)
    {
        return ! (a == b);
    }

private:
    constexpr static const btn::fixed min_inv_scale = 128;
    constexpr static const btn::fixed min_scale = 1 / min_inv_scale;

    fixed _rotation_angle = 0;
    fixed _horizontal_scale = 1;
    fixed _vertical_scale = 1;
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

    constexpr void _update_horizontal_scale()
    {
        _sx = _output_scale(_horizontal_scale);
    }

    constexpr void _update_vertical_scale()
    {
        _sy = _output_scale(_vertical_scale);
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
