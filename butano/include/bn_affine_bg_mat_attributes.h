/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_MAT_ATTRIBUTES_H
#define BN_AFFINE_BG_MAT_ATTRIBUTES_H

/**
 * @file
 * bn::affine_bg_mat_attributes header file.
 *
 * @ingroup affine_bg
 * @ingroup affine_mat
 */

#include "bn_display.h"
#include "bn_fixed_size.h"
#include "bn_fixed_point.h"
#include "bn_affine_mat_attributes.h"

namespace bn
{

/**
 * @brief Contains the attributes which define the position and the transformation matrix of an affine background.
 *
 * @ingroup affine_bg
 * @ingroup affine_mat
 */
class affine_bg_mat_attributes
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr affine_bg_mat_attributes() = default;

    /**
     * @brief Constructor.
     * @param position Affine background absolute position (ignoring its camera).
     * @param half_dimensions Half of the affine background size.
     * @param pivot_position Position of the affine transformations pivot point.
     * @param mat_attributes Attributes which define the base affine transformation matrix.
     */
    constexpr affine_bg_mat_attributes(
            const fixed_point& position, const fixed_size& half_dimensions, const fixed_point& pivot_position,
            const affine_mat_attributes& mat_attributes) :
        _position(position),
        _half_dimensions(half_dimensions),
        _pivot_position(pivot_position),
        _mat_attributes(mat_attributes)
    {
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the horizontal absolute position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal absolute position of the affine background (ignoring its camera).
     */
    constexpr void set_x(fixed x)
    {
        _position.set_x(x);
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the absolute vertical position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the absolute vertical position of the affine background (ignoring its camera).
     */
    constexpr void set_y(fixed y)
    {
        _position.set_y(y);
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the absolute position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the absolute position of the affine background (ignoring its camera).
     * @param x Horizontal absolute position of the affine background.
     * @param y Vertical absolute position of the affine background.
     */
    constexpr void set_position(fixed x, fixed y)
    {
        _position.set_x(x);
        _position.set_y(y);
        _update_dx_and_dy();
    }

    /**
     * @brief Sets the absolute position of the affine background (ignoring its camera).
     * @param position Absolute position of the affine background.
     */
    constexpr void set_position(const fixed_point& position)
    {
        _position = position;
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the horizontal absolute top-left position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr fixed top_left_x() const
    {
        return _to_top_left_x(x());
    }

    /**
     * @brief Sets the horizontal absolute top-left position of the affine background (ignoring its camera).
     */
    constexpr void set_top_left_x(fixed top_left_x)
    {
        set_x(_from_top_left_x(top_left_x));
    }

    /**
     * @brief Returns the absolute vertical top-left position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr fixed top_left_y() const
    {
        return _to_top_left_y(y());
    }

    /**
     * @brief Sets the absolute vertical top-left position of the affine background (ignoring its camera).
     */
    constexpr void set_top_left_y(fixed top_left_y)
    {
        set_y(_from_top_left_y(top_left_y));
    }

    /**
     * @brief Returns the absolute top-left position of the affine background (ignoring its camera).
     */
    [[nodiscard]] constexpr fixed_point top_left_position() const
    {
        return _to_top_left_position(position());
    }

    /**
     * @brief Sets the absolute top-left position of the affine background (ignoring its camera).
     * @param top_left_x Horizontal absolute top-left position of the affine background.
     * @param top_left_y Vertical absolute top-left position of the affine background.
     */
    constexpr void set_top_left_position(fixed top_left_x, fixed top_left_y)
    {
        set_position(_from_top_left_position(fixed_point(top_left_x, top_left_y)));
    }

    /**
     * @brief Sets the absolute top-left position of the affine background (ignoring its camera).
     * @param top_left_position Absolute top-left position of the affine background.
     */
    constexpr void set_top_left_position(const fixed_point& top_left_position)
    {
        set_position(_from_top_left_position(top_left_position));
    }

    /**
     * @brief Returns the half of the affine background size.
     */
    [[nodiscard]] constexpr const fixed_size& half_dimensions() const
    {
        return _half_dimensions;
    }

    /**
     * @brief Sets the half of the affine background size.
     */
    constexpr void set_half_dimensions(const fixed_size& half_dimensions)
    {
        _half_dimensions = half_dimensions;
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the horizontal position of the affine transformations pivot point.
     */
    [[nodiscard]] constexpr fixed pivot_x() const
    {
        return _pivot_position.x();
    }

    /**
     * @brief Sets the horizontal position of the affine transformations pivot point.
     */
    constexpr void set_pivot_x(fixed pivot_x)
    {
        _pivot_position.set_x(pivot_x);
        _update_dx();
    }

    /**
     * @brief Returns the vertical position of the affine transformations pivot point.
     */
    [[nodiscard]] constexpr fixed pivot_y() const
    {
        return _pivot_position.y();
    }

    /**
     * @brief Sets the vertical position of the affine transformations pivot point.
     */
    constexpr void set_pivot_y(fixed pivot_y)
    {
        _pivot_position.set_y(pivot_y);
        _update_dy();
    }

    /**
     * @brief Returns the position of the affine transformations pivot point.
     */
    [[nodiscard]] constexpr const fixed_point& pivot_position() const
    {
        return _pivot_position;
    }

    /**
     * @brief Sets the position of the affine transformations pivot point.
     * @param pivot_x Horizontal position of the pivot point.
     * @param pivot_y Vertical position of the pivot point.
     */
    constexpr void set_pivot_position(fixed pivot_x, fixed pivot_y)
    {
        _pivot_position.set_x(pivot_x);
        _pivot_position.set_y(pivot_y);
        _update_dx_and_dy();
    }

    /**
     * @brief Sets the position of the affine transformations pivot point.
     * @param pivot_position Position of the pivot point.
     */
    constexpr void set_pivot_position(const fixed_point& pivot_position)
    {
        _pivot_position = pivot_position;
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the rotation angle in degrees.
     */
    [[nodiscard]] constexpr fixed rotation_angle() const
    {
        return _mat_attributes.rotation_angle();
    }

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     */
    constexpr void set_rotation_angle(fixed rotation_angle)
    {
        _mat_attributes.set_rotation_angle(rotation_angle);
        _update_dx_and_dy();
    }

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in any range.
     */
    constexpr void set_rotation_angle_safe(fixed rotation_angle)
    {
        _mat_attributes.set_rotation_angle_safe(rotation_angle);
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the horizontal scale.
     */
    [[nodiscard]] constexpr fixed horizontal_scale() const
    {
        return _mat_attributes.horizontal_scale();
    }

    /**
     * @brief Sets the horizontal scale.
     */
    constexpr void set_horizontal_scale(fixed horizontal_scale)
    {
        _mat_attributes.set_horizontal_scale(horizontal_scale);
        _update_dx();
    }

    /**
     * @brief Returns the vertical scale.
     */
    [[nodiscard]] constexpr fixed vertical_scale() const
    {
        return _mat_attributes.vertical_scale();
    }

    /**
     * @brief Sets the vertical scale.
     */
    constexpr void set_vertical_scale(fixed vertical_scale)
    {
        _mat_attributes.set_vertical_scale(vertical_scale);
        _update_dy();
    }

    /**
     * @brief Sets the scale.
     */
    constexpr void set_scale(fixed scale)
    {
        _mat_attributes.set_scale(scale);
        _update_dx_and_dy();
    }

    /**
     * @brief Sets the scale.
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     */
    constexpr void set_scale(fixed horizontal_scale, fixed vertical_scale)
    {
        _mat_attributes.set_scale(horizontal_scale, vertical_scale);
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the horizontal shear.
     */
    [[nodiscard]] constexpr fixed horizontal_shear() const
    {
        return _mat_attributes.horizontal_shear();
    }

    /**
     * @brief Sets the horizontal shear.
     */
    constexpr void set_horizontal_shear(fixed horizontal_shear)
    {
        _mat_attributes.set_horizontal_shear(horizontal_shear);
        _update_dx();
    }

    /**
     * @brief Returns the vertical shear.
     */
    [[nodiscard]] constexpr fixed vertical_shear() const
    {
        return _mat_attributes.vertical_shear();
    }

    /**
     * @brief Sets the vertical shear.
     */
    constexpr void set_vertical_shear(fixed vertical_shear)
    {
        _mat_attributes.set_vertical_shear(vertical_shear);
        _update_dy();
    }

    /**
     * @brief Sets the shear.
     */
    constexpr void set_shear(fixed shear)
    {
        _mat_attributes.set_shear(shear);
        _update_dx_and_dy();
    }

    /**
     * @brief Sets the shear.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     */
    constexpr void set_shear(fixed horizontal_shear, fixed vertical_shear)
    {
        _mat_attributes.set_shear(horizontal_shear, vertical_shear);
        _update_dx_and_dy();
    }

    /**
     * @brief Indicates if this matrix is flipped in the horizontal axis or not.
     */
    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _mat_attributes.horizontal_flip();
    }

    /**
     * @brief Sets if this matrix is flipped in the horizontal axis or not.
     */
    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _mat_attributes.set_horizontal_flip(horizontal_flip);
        _update_dx();
    }

    /**
     * @brief Indicates if this matrix is flipped in the vertical axis or not.
     */
    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _mat_attributes.vertical_flip();
    }

    /**
     * @brief Sets if this matrix is flipped in the vertical axis or not.
     */
    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _mat_attributes.set_vertical_flip(vertical_flip);
        _update_dy();
    }

    /**
     * @brief Returns the attributes which define the base affine transformation matrix.
     */
    [[nodiscard]] constexpr const affine_mat_attributes& mat_attributes() const
    {
        return _mat_attributes;
    }

    /**
     * @brief Sets the attributes which define the base affine transformation matrix.
     */
    constexpr void set_mat_attributes(const affine_mat_attributes& mat_attributes)
    {
        _mat_attributes = mat_attributes;
        _update_dx_and_dy();
    }

    /**
     * @brief Returns the value to commit to the first GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pa_register_value() const
    {
        return _mat_attributes.pa_register_value();
    }

    /**
     * @brief Returns the value to commit to the second GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pb_register_value() const
    {
        return _mat_attributes.pb_register_value();
    }

    /**
     * @brief Returns the value to commit to the third GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pc_register_value() const
    {
        return _mat_attributes.pc_register_value();
    }

    /**
     * @brief Returns the value to commit to the fourth GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int pd_register_value() const
    {
        return _mat_attributes.pd_register_value();
    }

    /**
     * @brief Returns the value to commit to the fifth GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int dx_register_value() const
    {
        return _dx;
    }

    /**
     * @brief Returns the value to commit to the fifth GBA register of
     * an affine background transformation matrix with these attributes.
     */
    [[nodiscard]] constexpr int dy_register_value() const
    {
        return _dy;
    }

    /**
     * @brief Equal operator.
     * @param a First affine_bg_mat_attributes to compare.
     * @param b Second affine_bg_mat_attributes to compare.
     * @return `true` if the first affine_bg_mat_attributes is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const affine_bg_mat_attributes& a,
                                                   const affine_bg_mat_attributes& b)
    {
        return a._position == b._position && a._half_dimensions == b._half_dimensions &&
                a._pivot_position == b._pivot_position && a._mat_attributes == b._mat_attributes;
    }

private:
    static constexpr int _right_shift = fixed::precision() - 8;

    fixed_point _position;
    fixed_size _half_dimensions;
    fixed_point _pivot_position;
    affine_mat_attributes _mat_attributes;
    int _dx = 0 - (256 * (display::width() / 2));
    int _dy = 0 - (256 * (display::height() / 2));

    [[nodiscard]] constexpr fixed _to_top_left_x(fixed x) const
    {
        return x + ((display::width() / 2) - _half_dimensions.width());
    }

    [[nodiscard]] constexpr fixed _from_top_left_x(fixed top_left_x) const
    {
        return top_left_x - ((display::width() / 2) - _half_dimensions.width());
    }

    [[nodiscard]] constexpr fixed _to_top_left_y(fixed y) const
    {
        return y + ((display::height() / 2) - _half_dimensions.height());
    }

    [[nodiscard]] constexpr fixed _from_top_left_y(fixed top_left_y) const
    {
        return top_left_y - ((display::height() / 2) - _half_dimensions.height());
    }

    [[nodiscard]] constexpr fixed_point _to_top_left_position(const fixed_point& position) const
    {
        return fixed_point(_to_top_left_x(position.x()), _to_top_left_y(position.y()));
    }

    [[nodiscard]] constexpr fixed_point _from_top_left_position(const fixed_point& top_left_position) const
    {
        return fixed_point(_from_top_left_x(top_left_position.x()), _from_top_left_y(top_left_position.y()));
    }

    constexpr void _update_dx()
    {
        int tex_x = (_pivot_position.x() + _half_dimensions.width()).data() >> _right_shift;
        int scr_x = _position.x().right_shift_integer() + (display::width() / 2);
        int scr_y = _position.y().right_shift_integer() + (display::height() / 2);
        _dx = tex_x - ((_mat_attributes.pa_register_value() * scr_x) + (_mat_attributes.pb_register_value() * scr_y));
    }

    constexpr void _update_dy()
    {
        int tex_y = (_pivot_position.y() + _half_dimensions.height()).data() >> _right_shift;
        int scr_x = _position.x().right_shift_integer() + (display::width() / 2);
        int scr_y = _position.y().right_shift_integer() + (display::height() / 2);
        _dy = tex_y - ((_mat_attributes.pc_register_value() * scr_x) + (_mat_attributes.pd_register_value() * scr_y));
    }

    constexpr void _update_dx_and_dy()
    {
        int tex_x = (_pivot_position.x() + _half_dimensions.width()).data() >> _right_shift;
        int tex_y = (_pivot_position.y() + _half_dimensions.height()).data() >> _right_shift;
        int scr_x = _position.x().right_shift_integer() + (display::width() / 2);
        int scr_y = _position.y().right_shift_integer() + (display::height() / 2);
        _dx = tex_x - ((_mat_attributes.pa_register_value() * scr_x) + (_mat_attributes.pb_register_value() * scr_y));
        _dy = tex_y - ((_mat_attributes.pc_register_value() * scr_x) + (_mat_attributes.pd_register_value() * scr_y));
    }
};

}

#endif
