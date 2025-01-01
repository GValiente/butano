/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_BUILDER_H
#define BN_AFFINE_BG_BUILDER_H

/**
 * @file
 * bn::affine_bg_builder header file.
 *
 * @ingroup affine_bg
 */

#include "bn_camera_ptr.h"
#include "bn_affine_bg_item.h"
#include "bn_green_swap_mode.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_mat_attributes.h"

namespace bn
{

class affine_bg_ptr;

/**
 * @brief Creates affine_bg_map_ptr objects with custom attributes.
 *
 * If some of the attributes of the affine background to create differs from the default ones,
 * using this class improves performance.
 *
 * @ingroup affine_bg
 */
class affine_bg_builder
{

public:
    /**
     * @brief Constructor.
     * @param item affine_bg_item containing the required information to generate affine backgrounds.
     */
    explicit affine_bg_builder(const affine_bg_item& item) :
        _item(item)
    {
    }

    /**
     * @brief Constructor.
     * @param item affine_bg_item containing the required information to generate affine backgrounds.
     * @param map_index Index of the map to reference in item.map_item() to generate affine backgrounds.
     */
    affine_bg_builder(const affine_bg_item& item, int map_index);

    /**
     * @brief Constructor.
     * @param map affine_bg_map_ptr to copy for generating affine backgrounds.
     */
    explicit affine_bg_builder(const affine_bg_map_ptr& map) :
        _map(map)
    {
    }

    /**
     * @brief Constructor.
     * @param map affine_bg_map_ptr to move for generating affine backgrounds.
     */
    explicit affine_bg_builder(affine_bg_map_ptr&& map) :
        _map(move(map))
    {
    }

    /**
     * @brief Returns the affine_bg_item containing the required information to generate affine backgrounds
     * if it has one; bn::nullopt otherwise.
     */
    [[nodiscard]] const optional<affine_bg_item>& item() const
    {
        return _item;
    }

    /**
     * @brief Index of the map to reference in item.map_item() to generate affine backgrounds.
     */
    [[nodiscard]] int map_index() const
    {
        return _map_index;
    }

    /**
     * @brief Returns the horizontal position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    /**
     * @brief Returns the vertical position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param y vertical position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    /**
     * @brief Returns the position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the affine backgrounds to generate.
     * @param y Vertical position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    /**
     * @brief Sets the position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param position Position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    /**
     * @brief Returns the horizontal top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_y Vertical top-left position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the affine backgrounds to generate.
     * @param top_left_y Vertical top-left position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the affine backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_position Top-left position of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the rotation angle in degrees of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed rotation_angle() const
    {
        return _mat_attributes.rotation_angle();
    }

    /**
     * @brief Sets the rotation angle in degrees of the affine backgrounds to generate.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     * @return Reference to this.
     */
    affine_bg_builder& set_rotation_angle(fixed rotation_angle)
    {
        _mat_attributes.set_rotation_angle(rotation_angle);
        return *this;
    }

    /**
     * @brief Sets the rotation angle in degrees of the affine backgrounds to generate.
     * @param rotation_angle Rotation angle in degrees, in any range.
     * @return Reference to this.
     */
    affine_bg_builder& set_rotation_angle_safe(fixed rotation_angle)
    {
        _mat_attributes.set_rotation_angle_safe(rotation_angle);
        return *this;
    }

    /**
     * @brief Returns the horizontal scale of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed horizontal_scale() const
    {
        return _mat_attributes.horizontal_scale();
    }

    /**
     * @brief Sets the horizontal scale of the affine backgrounds to generate.
     * @param horizontal_scale Horizontal scale of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_horizontal_scale(fixed horizontal_scale)
    {
        _mat_attributes.set_horizontal_scale(horizontal_scale);
        return *this;
    }

    /**
     * @brief Returns the vertical scale of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed vertical_scale() const
    {
        return _mat_attributes.horizontal_scale();
    }

    /**
     * @brief Sets the vertical scale of the affine backgrounds to generate.
     * @param vertical_scale Vertical scale of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_vertical_scale(fixed vertical_scale)
    {
        _mat_attributes.set_vertical_scale(vertical_scale);
        return *this;
    }

    /**
     * @brief Sets the scale of the affine backgrounds to generate.
     * @param scale Scale of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_scale(fixed scale)
    {
        _mat_attributes.set_scale(scale);
        return *this;
    }

    /**
     * @brief Sets the scale of the affine backgrounds to generate.
     * @param horizontal_scale Horizontal scale of the affine backgrounds to generate.
     * @param vertical_scale Vertical scale of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)
    {
        _mat_attributes.set_scale(horizontal_scale, vertical_scale);
        return *this;
    }

    /**
     * @brief Returns the horizontal shear of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed horizontal_shear() const
    {
        return _mat_attributes.horizontal_shear();
    }

    /**
     * @brief Sets the horizontal shear of the affine backgrounds to generate.
     * @param horizontal_shear Horizontal shear of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_horizontal_shear(fixed horizontal_shear)
    {
        _mat_attributes.set_horizontal_shear(horizontal_shear);
        return *this;
    }

    /**
     * @brief Returns the vertical shear of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed vertical_shear() const
    {
        return _mat_attributes.horizontal_shear();
    }

    /**
     * @brief Sets the vertical shear of the affine backgrounds to generate.
     * @param vertical_shear Vertical shear of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_vertical_shear(fixed vertical_shear)
    {
        _mat_attributes.set_vertical_shear(vertical_shear);
        return *this;
    }

    /**
     * @brief Sets the shear of the affine backgrounds to generate.
     * @param shear Shear of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_shear(fixed shear)
    {
        _mat_attributes.set_shear(shear);
        return *this;
    }

    /**
     * @brief Sets the shear of the affine backgrounds to generate.
     * @param horizontal_shear Horizontal shear of the affine backgrounds to generate.
     * @param vertical_shear Vertical shear of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_shear(fixed horizontal_shear, fixed vertical_shear)
    {
        _mat_attributes.set_shear(horizontal_shear, vertical_shear);
        return *this;
    }

    /**
     * @brief Indicates if the affine backgrounds to generate are flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const
    {
        return _mat_attributes.horizontal_flip();
    }

    /**
     * @brief Sets if the affine backgrounds to generate must be flipped in the horizontal axis or not.
     * @param horizontal_flip `true` if the affine backgrounds must be flipped in the horizontal axis;
     * `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_horizontal_flip(bool horizontal_flip)
    {
        _mat_attributes.set_horizontal_flip(horizontal_flip);
        return *this;
    }

    /**
     * @brief Indicates if the affine backgrounds to generate are flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const
    {
        return _mat_attributes.horizontal_flip();
    }

    /**
     * @brief Sets if the affine backgrounds to generate must be flipped in the vertical axis or not.
     * @param vertical_flip `true` if the affine backgrounds must be flipped in the vertical axis;
     * `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_vertical_flip(bool vertical_flip)
    {
        _mat_attributes.set_vertical_flip(vertical_flip);
        return *this;
    }

    /**
     * @brief Returns the horizontal position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed pivot_x() const
    {
        return _pivot_position.x();
    }

    /**
     * @brief Sets the horizontal position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     */
    void set_pivot_x(fixed pivot_x)
    {
        _pivot_position.set_x(pivot_x);
    }

    /**
     * @brief Returns the vertical position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     */
    [[nodiscard]] fixed pivot_y() const
    {
        return _pivot_position.y();
    }

    /**
     * @brief Sets the vertical position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     */
    void set_pivot_y(fixed pivot_y)
    {
        _pivot_position.set_y(pivot_y);
    }

    /**
     * @brief Returns the position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     */
    [[nodiscard]] const fixed_point& pivot_position() const
    {
        return _pivot_position;
    }

    /**
     * @brief Sets the position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     * @param pivot_x Horizontal position of the pivot point.
     * @param pivot_y Vertical position of the pivot point.
     */
    void set_pivot_position(fixed pivot_x, fixed pivot_y)
    {
        _pivot_position.set_x(pivot_x);
        _pivot_position.set_y(pivot_y);
    }

    /**
     * @brief Sets the position of the affine transformations pivot point
     * of the affine backgrounds to generate.
     * @param pivot_position Position of the pivot point.
     */
    void set_pivot_position(const fixed_point& pivot_position)
    {
        _pivot_position = pivot_position;
    }

    /**
     * @brief Returns the attributes of the affine transformation matrix of the affine backgrounds to generate.
     */
    [[nodiscard]] const affine_mat_attributes& mat_attributes() const
    {
        return _mat_attributes;
    }

    /**
     * @brief Sets the attributes of the affine transformation matrix of the affine backgrounds to generate.
     * @param mat_attributes affine_mat_attributes of the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& set_affine_mat(const affine_mat_attributes& mat_attributes)
    {
        _mat_attributes = mat_attributes;
        return *this;
    }

    /**
     * @brief Returns the priority of the affine backgrounds to generate relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    /**
     * @brief Sets the priority of the affine backgrounds to generate relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     * @return Reference to this.
     */
    affine_bg_builder& set_priority(int priority);

    /**
     * @brief Returns the priority of the affine backgrounds to generate relative to other backgrounds,
     * excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     */
    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    /**
     * @brief Sets the priority of the affine backgrounds to generate relative to other backgrounds,
     * excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     *
     * @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
     * @return Reference to this.
     */
    affine_bg_builder& set_z_order(int z_order);

    /**
     * @brief Indicates if the affine backgrounds to generate wrap around at the edges or not.
     */
    [[nodiscard]] bool wrapping_enabled() const
    {
        return _wrapping_enabled;
    }

    /**
     * @brief Sets if the affine backgrounds to generate must wrap around at the edges or not.
     * @param wrapping_enabled `true` if they must wrap around at the edges; `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_wrapping_enabled(bool wrapping_enabled)
    {
        _wrapping_enabled = wrapping_enabled;
        return *this;
    }

    /**
     * @brief Indicates if the mosaic effect must be applied to the affine backgrounds to generate or not.
     */
    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the affine backgrounds to generate or not.
     * @param mosaic_enabled `true` if the mosaic effect must be applied; `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    /**
     * @brief Indicates if blending must be applied to the affine backgrounds to generate or not.
     *
     * Blending is applied to affine backgrounds by making them part of the blending top layer.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return blending_top_enabled();
    }

    /**
     * @brief Sets if blending must be applied to the affine backgrounds to generate or not.
     *
     * Blending is applied to affine backgrounds by making them part of the blending top layer.
     * @param blending_enabled `true` if blending must be applied; `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
        return *this;
    }

    /**
     * @brief Indicates if the affine backgrounds to generate must be part of the blending top layer or not.
     */
    [[nodiscard]] bool blending_top_enabled() const
    {
        return _blending_top_enabled;
    }

    /**
     * @brief Sets if the affine backgrounds to generate must be part of the blending top layer or not.
     * @param blending_top_enabled `true` if generated backgrounds must be part of the blending top layer;
     * `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_blending_top_enabled(bool blending_top_enabled)
    {
        _blending_top_enabled = blending_top_enabled;
        return *this;
    }

    /**
     * @brief Indicates if the affine backgrounds to generate must be part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled() const
    {
        return _blending_bottom_enabled;
    }

    /**
     * @brief Sets if the affine backgrounds to generate must be part of the blending bottom layer or not.
     * @param blending_bottom_enabled `true` if generated backgrounds must be part of the blending bottom layer;
     * `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)
    {
        _blending_bottom_enabled = blending_bottom_enabled;
        return *this;
    }

    /**
     * @brief Indicates how the affine backgrounds to generate must be displayed when green swap is enabled.
     */
    [[nodiscard]] bn::green_swap_mode green_swap_mode() const
    {
        return _green_swap_mode;
    }

    /**
     * @brief Sets how the affine backgrounds to generate must be displayed when green swap is enabled.
     * @param green_swap_mode Green swap mode.
     * @return Reference to this.
     */
    affine_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)
    {
        _green_swap_mode = green_swap_mode;
        return *this;
    }

    /**
     * @brief Indicates if the affine backgrounds to generate must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    /**
     * @brief Sets if the affine backgrounds to generate must be committed to the GBA or not.
     * @param visible `true` if the affine backgrounds must be committed to the GBA; `false` otherwise.
     * @return Reference to this.
     */
    affine_bg_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    /**
     * @brief Returns the camera_ptr to attach to the affine backgrounds to generate (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const
    {
        return _camera;
    }

    /**
     * @brief Sets the camera_ptr to attach to the affine backgrounds to generate.
     * @param camera camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    affine_bg_builder& set_camera(const camera_ptr& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets the camera_ptr to attach to the affine backgrounds to generate.
     * @param camera camera_ptr to move to the builder.
     * @return Reference to this.
     */
    affine_bg_builder& set_camera(camera_ptr&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the affine backgrounds to generate.
     * @param camera Optional camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    affine_bg_builder& set_camera(const optional<camera_ptr>& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the affine backgrounds to generate.
     * @param camera Optional camera_ptr to move to the builder.
     * @return Reference to this.
     */
    affine_bg_builder& set_camera(optional<camera_ptr>&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Removes the camera_ptr to attach to the affine backgrounds to generate.
     * @return Reference to this.
     */
    affine_bg_builder& remove_camera()
    {
        _camera.reset();
        return *this;
    }

    /**
     * @brief Releases and returns the camera_ptr to attach to the affine backgrounds to generate (if any).
     */
    [[nodiscard]] optional<camera_ptr> release_camera();

    /**
     * @brief Generates and returns an affine_bg_ptr without releasing the acquired resources.
     */
    [[nodiscard]] affine_bg_ptr build() const;

    /**
     * @brief Generates and returns an affine_bg_ptr releasing the acquired resources.
     *
     * affine_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] affine_bg_ptr release_build();

    /**
     * @brief Generates and returns an affine_bg_ptr
     * without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_ptr> build_optional() const;

    /**
     * @brief Generates and returns an affine_bg_ptr releasing the acquired resources if it could be allocated;
     * bn::nullopt otherwise.
     *
     * affine_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<affine_bg_ptr> release_build_optional();

    /**
     * @brief Generates and returns an affine_bg_map_ptr without releasing the acquired resources.
     */
    [[nodiscard]] affine_bg_map_ptr map() const;

    /**
     * @brief Generates and returns an affine_bg_map_ptr without releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> map_optional() const;

    /**
     * @brief Generates and returns an affine_bg_map_ptr releasing the acquired resources.
     *
     * affine_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] affine_bg_map_ptr release_map();

    /**
     * @brief Generates and returns an affine_bg_map_ptr releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     *
     * affine_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> release_map_optional();

private:
    affine_mat_attributes _mat_attributes;
    optional<affine_bg_item> _item;
    fixed_point _position;
    fixed_point _pivot_position;
    int _map_index = 0;
    int _z_order = 0;
    optional<affine_bg_map_ptr> _map;
    optional<camera_ptr> _camera;
    bn::green_swap_mode _green_swap_mode = green_swap_mode::DEFAULT;
    uint8_t _priority = 3;
    bool _wrapping_enabled = true;
    bool _mosaic_enabled = false;
    bool _blending_top_enabled = false;
    bool _blending_bottom_enabled = true;
    bool _visible = true;

    [[nodiscard]] size _dimensions() const;
};

}

#endif

