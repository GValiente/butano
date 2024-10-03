/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_MAT_ATTRIBUTES_H
#define BN_AFFINE_MAT_ATTRIBUTES_H

/**
 * @file
 * bn::affine_mat_attributes header file.
 *
 * @ingroup affine_mat
 */

#include "bn_math.h"

namespace bn
{

/**
 * @brief Contains the attributes which define an affine transformation matrix.
 *
 * @ingroup affine_mat
 */
class affine_mat_attributes
{

public:
    /**
     * @brief Identity constructor.
     */
    constexpr affine_mat_attributes() = default;

    /**
     * @brief Constructor.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     * @param horizontal_flip Indicates if this matrix is flipped in the horizontal axis or not.
     * @param vertical_flip Indicates if this matrix is flipped in the vertical axis or not.
     */
    constexpr affine_mat_attributes(fixed rotation_angle, fixed horizontal_scale, fixed vertical_scale,
                                    bool horizontal_flip, bool vertical_flip) :
        affine_mat_attributes(rotation_angle, horizontal_scale, vertical_scale, 0, 0, horizontal_flip, vertical_flip)
    {
    }

    /**
     * @brief Constructor.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     * @param horizontal_flip Indicates if this matrix is flipped in the horizontal axis or not.
     * @param vertical_flip Indicates if this matrix is flipped in the vertical axis or not.
     */
    constexpr affine_mat_attributes(fixed rotation_angle, fixed horizontal_scale, fixed vertical_scale,
                                    fixed horizontal_shear, fixed vertical_shear, bool horizontal_flip,
                                    bool vertical_flip) :
        _rotation_angle(rotation_angle),
        _horizontal_scale(horizontal_scale),
        _vertical_scale(vertical_scale),
        _horizontal_shear(horizontal_shear),
        _vertical_shear(vertical_shear),
        _hflip(int8_t(1 - (2 * horizontal_flip))),
        _vflip(int8_t(1 - (2 * vertical_flip)))
    {
        BN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);
        BN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

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
        _rotation_angle = rotation_angle;
        _update_rotation_angle();
        _update_pa();
        _update_pb();
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in any range.
     */
    constexpr void set_rotation_angle_safe(fixed rotation_angle)
    {
        set_rotation_angle(safe_degrees_angle(rotation_angle));
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
        BN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);

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
        BN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

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
        BN_ASSERT(scale > 0, "Invalid scale: ", scale);

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
        BN_ASSERT(horizontal_scale > 0, "Invalid horizontal scale: ", horizontal_scale);
        BN_ASSERT(vertical_scale > 0, "Invalid vertical scale: ", vertical_scale);

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
     * @brief Returns the horizontal shear.
     */
    [[nodiscard]] constexpr fixed horizontal_shear() const
    {
        return _horizontal_shear;
    }

    /**
     * @brief Sets the horizontal shear.
     */
    constexpr void set_horizontal_shear(fixed horizontal_shear)
    {
        _horizontal_shear = horizontal_shear;
        _update_pb();
    }

    /**
     * @brief Returns the vertical shear.
     */
    [[nodiscard]] constexpr fixed vertical_shear() const
    {
        return _vertical_shear;
    }

    /**
     * @brief Sets the vertical shear.
     */
    constexpr void set_vertical_shear(fixed vertical_shear)
    {
        _vertical_shear = vertical_shear;
        _update_pc();
    }

    /**
     * @brief Sets the shear.
     */
    constexpr void set_shear(fixed shear)
    {
        set_horizontal_shear(shear);
        set_vertical_shear(shear);
    }

    /**
     * @brief Sets the shear.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     */
    constexpr void set_shear(fixed horizontal_shear, fixed vertical_shear)
    {
        set_horizontal_shear(horizontal_shear);
        set_vertical_shear(vertical_shear);
    }

    /**
     * @brief Indicates if this matrix is flipped in the horizontal axis or not.
     */
    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _hflip < 0;
    }

    /**
     * @brief Sets if this matrix is flipped in the horizontal axis or not.
     */
    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _hflip = int8_t(1 - (2 * horizontal_flip));
        _update_pa();
        _update_pb();
    }

    /**
     * @brief Indicates if this matrix is flipped in the vertical axis or not.
     */
    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vflip < 0;
    }

    /**
     * @brief Sets if this matrix is flipped in the vertical axis or not.
     */
    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vflip = int8_t(1 - (2 * vertical_flip));
        _update_pc();
        _update_pd();
    }

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not.
     */
    [[nodiscard]] constexpr bool identity() const
    {
        return _rotation_angle == 0 && _horizontal_scale == 1 && _vertical_scale == 1 &&
                _horizontal_shear == 0 && _vertical_shear == 0 && _hflip >= 0 && _vflip >= 0 &&
                _pa == 256 && _pb == 0 && _pc == 0 && _pd == 256;
    }

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not, ignoring flip attributes.
     */
    [[nodiscard]] constexpr bool flipped_identity() const
    {
        return _rotation_angle == 0 && _horizontal_scale == 1 && _vertical_scale == 1 &&
                _horizontal_shear == 0 && _vertical_shear == 0 &&
                abs(_pa) == 256 && _pb == 0 && _pc == 0 && abs(_pd) == 256;
    }

    /**
     * @brief Returns the value to commit to the first GBA register of
     * an affine transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pa_register_value() const
    {
        return _pa;
    }

    /**
     * @brief Returns the value to commit to the second GBA register of
     * an affine transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pb_register_value() const
    {
        return _pb;
    }

    /**
     * @brief Returns the value to commit to the third GBA register of
     * an affine transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pc_register_value() const
    {
        return _pc;
    }

    /**
     * @brief Returns the value to commit to the fourth GBA register of
     * an affine transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pd_register_value() const
    {
        return _pd;
    }

    /**
     * @brief UNSAFE: sets the values to commit to the GBA registers of an affine transformation matrix
     * bypassing the rest of the parameters.
     *
     * The rest of the parameters are not updated, so the affine transformation matrix ends in an inconsistent state.
     */
    constexpr void unsafe_set_register_values(int pa, int pb, int pc, int pd)
    {
        _pa = int16_t(pa);
        _pb = int16_t(pb);
        _pc = int16_t(pc);
        _pd = int16_t(pd);
    }

    /**
     * @brief Equal operator.
     * @param a First affine_mat_attributes to compare.
     * @param b Second affine_mat_attributes to compare.
     * @return `true` if the first affine_mat_attributes is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const affine_mat_attributes& a, const affine_mat_attributes& b)
    {
        return a._rotation_angle == b._rotation_angle &&
                a._horizontal_scale == b._horizontal_scale && a._vertical_scale == b._vertical_scale &&
                a._horizontal_shear == b._horizontal_shear && a._vertical_shear == b._vertical_shear &&
                a._hflip == b._hflip && a._vflip == b._vflip &&
                a._pa == b._pa && a._pb == b._pb && a._pc == b._pc && a._pd == b._pd;
    }

private:
    constexpr static fixed _min_inv_scale = 128;
    constexpr static fixed _min_scale = 1 / _min_inv_scale;

    friend class affine_mat_attributes_reader;

    fixed _rotation_angle = 0;
    fixed _horizontal_scale = 1;
    fixed _vertical_scale = 1;
    fixed _horizontal_shear = 0;
    fixed _vertical_shear = 0;
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
        if(scale <= _min_scale)
        {
            return uint16_t(fixed_t<8>(_min_inv_scale).data());
        }

        fixed_t<8> scale_8(scale);
        int scale_8_data = scale_8.data();

        if(scale_8_data < reciprocal_16_lut_size) [[likely]]
        {
            if consteval
            {
                return uint16_t(calculate_reciprocal_lut_value<16>(scale_8_data).data());
            }

            return reciprocal_16_lut._data[scale_8_data];
        }

        return uint16_t(calculate_reciprocal_lut_value<16>(scale_8_data).data());
    }

    constexpr void _update_rotation_angle()
    {
        fixed rotation_angle = _rotation_angle;

        if(rotation_angle == 0)
        {
            _sin = 0;
            _cos = int16_t(fixed(1).data());
        }
        else
        {
            pair<fixed, fixed> sin_and_cos = degrees_lut_sin_and_cos(rotation_angle);
            _sin = int16_t(sin_and_cos.first.data());
            _cos = int16_t(sin_and_cos.second.data());
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
        int rot_scale = -_sin * (_sx * int(_hflip)) >> 12;
        int shear = _horizontal_shear.data() >> 4;
        _pb = int16_t(rot_scale + shear);
    }

    constexpr void _update_pc()
    {
        int rot_scale = _sin * (_sy * int(_vflip)) >> 12;
        int shear = _vertical_shear.data() >> 4;
        _pc = int16_t(rot_scale + shear);
    }

    constexpr void _update_pd()
    {
        _pd = int16_t(_cos * (_sy * int(_vflip)) >> 12);
    }
};

}

#endif
