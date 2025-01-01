/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_BUILDER_H
#define BN_SPRITE_BUILDER_H

/**
 * @file
 * bn::sprite_builder header file.
 *
 * @ingroup sprite
 */

#include "bn_camera_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_shape_size.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_double_size_mode.h"

namespace bn
{

class sprite_ptr;

/**
 * @brief Creates sprite_ptr objects with custom attributes.
 *
 * If some of the attributes of the sprite to create differs from the default ones,
 * using this class improves performance.
 *
 * @ingroup sprite
 */
class sprite_builder
{

public:
    /**
     * @brief Constructor.
     * @param item sprite_item containing the required information to generate sprites.
     */
    explicit sprite_builder(const sprite_item& item);

    /**
     * @brief Constructor.
     * @param item sprite_item containing the required information to generate sprites.
     * @param graphics_index Index of the tile set to reference in item.tiles_item() to generate sprites.
     */
    sprite_builder(const sprite_item& item, int graphics_index);

    /**
     * @brief Constructor.
     * @param shape_size Shape and size of the sprites to generate.
     * @param tiles Smart pointer to the sprite tile set used to generate sprites.
     * @param palette Smart pointer to the sprite color palette used to generate sprites.
     */
    sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    /**
     * @brief Returns the sprite_item containing the required information to generate sprites if it has one;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] const optional<sprite_item>& item() const
    {
        return _item;
    }

    /**
     * @brief Index of the tile set to reference in item.tiles_item() to generate sprites.
     */
    [[nodiscard]] int graphics_index() const
    {
        return _graphics_index;
    }

    /**
     * @brief Returns the shape and size of the sprites to generate.
     */
    [[nodiscard]] const sprite_shape_size& shape_size() const
    {
        return _shape_size;
    }

    /**
     * @brief Returns the horizontal position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    /**
     * @brief Returns the vertical position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param y Vertical position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    /**
     * @brief Returns the position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the sprites to generate (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the sprites to generate.
     * @param y Vertical position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    /**
     * @brief Sets the position of the sprites to generate (relative to their camera, if they are going to have one).
     * @param position Position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    /**
     * @brief Returns the horizontal top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_y Vertical top-left position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the sprites to generate.
     * @param top_left_y Vertical top-left position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the sprites to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_position Top-left position of the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the rotation angle in degrees of the sprites to generate.
     */
    [[nodiscard]] fixed rotation_angle() const;

    /**
     * @brief Sets the rotation angle in degrees of the sprites to generate.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     *
     * If the rotation angle is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given rotation angle is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_rotation_angle(fixed rotation_angle);

    /**
     * @brief Sets the rotation angle in degrees of the sprites to generate.
     * @param rotation_angle Rotation angle in degrees, in any range.
     *
     * If the rotation angle is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given rotation angle is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_rotation_angle_safe(fixed rotation_angle);

    /**
     * @brief Returns the horizontal scale of the sprites to generate.
     */
    [[nodiscard]] fixed horizontal_scale() const;

    /**
     * @brief Sets the horizontal scale of the sprites to generate.
     * @param horizontal_scale Horizontal scale of the sprites to generate.
     *
     * If the horizontal scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given horizontal scale is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_horizontal_scale(fixed horizontal_scale);

    /**
     * @brief Returns the vertical scale of the sprites to generate.
     */
    [[nodiscard]] fixed vertical_scale() const;

    /**
     * @brief Sets the vertical scale of the sprites to generate.
     * @param vertical_scale Vertical scale of the sprites to generate.
     *
     * If the vertical scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given vertical scale is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_vertical_scale(fixed vertical_scale);

    /**
     * @brief Sets the scale of the sprites to generate.
     * @param scale Scale of the sprites to generate.
     *
     * If the scale is != 1 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given vertical scale is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_scale(fixed scale);

    /**
     * @brief Sets the scale of the sprites to generate.
     * @param horizontal_scale Horizontal scale of the sprites to generate.
     * @param vertical_scale Vertical scale of the sprites to generate.
     *
     * If the horizontal or the vertical scale is != 1 and the builder doesn't have
     * an attached sprite_affine_mat_ptr, a new one with the given vertical scale is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_scale(fixed horizontal_scale, fixed vertical_scale);

    /**
     * @brief Returns the horizontal shear of the sprites to generate.
     */
    [[nodiscard]] fixed horizontal_shear() const;

    /**
     * @brief Sets the horizontal shear of the sprites to generate.
     * @param horizontal_shear Horizontal shear of the sprites to generate.
     *
     * If the horizontal shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given horizontal shear is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_horizontal_shear(fixed horizontal_shear);

    /**
     * @brief Returns the vertical shear of the sprites to generate.
     */
    [[nodiscard]] fixed vertical_shear() const;

    /**
     * @brief Sets the vertical shear of the sprites to generate.
     * @param vertical_shear Vertical shear of the sprites to generate.
     *
     * If the vertical shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given vertical shear is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_vertical_shear(fixed vertical_shear);

    /**
     * @brief Sets the shear of the sprites to generate.
     * @param shear Shear of the sprites to generate.
     *
     * If the shear is != 0 and the builder doesn't have an attached sprite_affine_mat_ptr,
     * a new one with the given vertical shear is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_shear(fixed shear);

    /**
     * @brief Sets the shear of the sprites to generate.
     * @param horizontal_shear Horizontal shear of the sprites to generate.
     * @param vertical_shear Vertical shear of the sprites to generate.
     *
     * If the horizontal or the vertical shear is != 0 and the builder doesn't have
     * an attached sprite_affine_mat_ptr, a new one with the given vertical shear is attached to it.
     *
     * @return Reference to this.
     */
    sprite_builder& set_shear(fixed horizontal_shear, fixed vertical_shear);

    /**
     * @brief Returns the priority of the sprites to generate relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     */
    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    /**
     * @brief Sets the priority of the sprites to generate relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     *
     * @param bg_priority Priority relative to backgrounds in the range [0..3].
     * @return Reference to this.
     */
    sprite_builder& set_bg_priority(int bg_priority);

    /**
     * @brief Returns the priority of the sprites to generate relative to other ones.
     *
     * Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
     *
     * Sprites are grouped in layers depending of their background priority and z order,
     * so to reduce memory usage and improve performance, please use as less unique z orders as possible.
     */
    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    /**
     * @brief Sets the priority of the sprites to generate relative to other ones.
     *
     * Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
     *
     * Sprites are grouped in layers depending of their background priority and z order,
     * so to reduce memory usage and improve performance, please use as less unique z orders as possible.
     *
     * @param z_order Priority relative to other sprites in the range [-32767..32767].
     * @return Reference to this.
     */
    sprite_builder& set_z_order(int z_order);

    /**
     * @brief Indicates if the sprites to generate are flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const;

    /**
     * @brief Sets if the sprites to generate must be flipped in the horizontal axis or not.
     * @param horizontal_flip `true` if the sprites must be flipped in the horizontal axis;
     * `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_horizontal_flip(bool horizontal_flip);

    /**
     * @brief Indicates if the sprites to generate are flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const;

    /**
     * @brief Sets if the sprites to generate must be flipped in the vertical axis or not.
     * @param vertical_flip `true` if the sprites must be flipped in the vertical axis; `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_vertical_flip(bool vertical_flip);

    /**
     * @brief Indicates if the mosaic effect must be applied to the sprites to generate or not.
     */
    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the sprites to generate or not.
     * @param mosaic_enabled `true` if the mosaic effect must be applied; `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    /**
     * @brief Indicates if blending must be applied to the sprites to generate or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return _blending_enabled;
    }

    /**
     * @brief Sets if blending must be applied to the sprites to generate or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
     *
     * @param blending_enabled `true` if blending must be applied; `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_blending_enabled(bool blending_enabled);

    /**
     * @brief Indicates if the sprites to generate must be part of the silhouette of the sprite window or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
     */
    [[nodiscard]] bool window_enabled() const
    {
        return _window_enabled;
    }

    /**
     * @brief Sets if the sprites to generate must be part of the silhouette of the sprite window or not.
     *
     * Keep in mind that blending and window attributes can't be enabled on a sprite at the same time.
     *
     * @param window_enabled `true` if the sprites must be part of the silhouette of the sprite window;
     * `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_window_enabled(bool window_enabled);

    /**
     * @brief Returns the area the sprites to generate use to be drawn
     * when they have an attached sprite_affine_mat_ptr.
     */
    [[nodiscard]] sprite_double_size_mode double_size_mode() const
    {
        return _double_size_mode;
    }

    /**
     * @brief Specifies the area the sprites to generate use to be drawn
     * when they have an attached sprite_affine_mat_ptr.
     * @param double_size_mode sprite_double_size_mode enum value.
     * @return Reference to this.
     */
    sprite_builder& set_double_size_mode(sprite_double_size_mode double_size_mode)
    {
        _double_size_mode = double_size_mode;
        return *this;
    }

    /**
     * @brief Indicates if the sprites to generate must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    /**
     * @brief Sets if the sprites to generate must be committed to the GBA or not.
     * @param visible `true` if the sprites must be committed to the GBA; `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    /**
     * @brief Returns the camera_ptr to attach to the sprites to generate (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const
    {
        return _camera;
    }

    /**
     * @brief Sets the camera_ptr to attach to the sprites to generate.
     * @param camera camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_camera(const camera_ptr& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets the camera_ptr to attach to the sprites to generate.
     * @param camera camera_ptr to move to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_camera(camera_ptr&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the sprites to generate.
     * @param camera Optional camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_camera(const optional<camera_ptr>& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the sprites to generate.
     * @param camera Optional camera_ptr to move to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_camera(optional<camera_ptr>&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Removes the camera_ptr to attach to the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& remove_camera()
    {
        _camera.reset();
        return *this;
    }

    /**
     * @brief Releases and returns the camera_ptr to attach to the sprites to generate (if any).
     */
    [[nodiscard]] optional<camera_ptr> release_camera();

    /**
     * @brief Generates and returns a sprite_ptr without releasing the acquired resources.
     */
    [[nodiscard]] sprite_ptr build() const;

    /**
     * @brief Generates and returns a sprite_ptr releasing the acquired resources.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] sprite_ptr release_build();

    /**
     * @brief Generates and returns a sprite_ptr without releasing the acquired resources if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_ptr> build_optional() const;

    /**
     * @brief Generates and returns a sprite_ptr releasing the acquired resources if it could be allocated;
     * bn::nullopt otherwise.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<sprite_ptr> release_build_optional();

    /**
     * @brief Generates and returns a sprite_tiles_ptr without releasing the acquired resources.
     */
    [[nodiscard]] sprite_tiles_ptr tiles() const;

    /**
     * @brief Generates and returns a sprite_palette_ptr without releasing the acquired resources.
     */
    [[nodiscard]] sprite_palette_ptr palette() const;

    /**
     * @brief Generates and returns a sprite_tiles_ptr without releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> tiles_optional() const;

    /**
     * @brief Generates and returns a sprite_palette_ptr without releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_palette_ptr> palette_optional() const;

    /**
     * @brief Generates and returns a sprite_tiles_ptr releasing the acquired resources.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] sprite_tiles_ptr release_tiles();

    /**
     * @brief Generates and returns a sprite_palette_ptr releasing the acquired resources.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] sprite_palette_ptr release_palette();

    /**
     * @brief Generates and returns a sprite_tiles_ptr releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> release_tiles_optional();

    /**
     * @brief Generates and returns a sprite_palette_ptr releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     *
     * sprite_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<sprite_palette_ptr> release_palette_optional();

    /**
     * @brief Returns the sprite_affine_mat_ptr to attach to the sprites to generate (if any).
     */
    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const
    {
        return _affine_mat;
    }

    /**
     * @brief Sets the sprite_affine_mat_ptr to attach to the sprites to generate.
     *
     * This method calls set_remove_affine_mat_when_not_needed(false).
     *
     * @param affine_mat sprite_affine_mat_ptr to copy to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_affine_mat(const sprite_affine_mat_ptr& affine_mat);

    /**
     * @brief Sets the sprite_affine_mat_ptr to attach to the sprites to generate.
     *
     * This method calls set_remove_affine_mat_when_not_needed(false).
     *
     * @param affine_mat sprite_affine_mat_ptr to move to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_affine_mat(sprite_affine_mat_ptr&& affine_mat);

    /**
     * @brief Sets or removes the sprite_affine_mat_ptr to attach to the sprites to generate.
     *
     * This method calls set_remove_affine_mat_when_not_needed(false)
     * if the given sprite_affine_mat_ptr is not null.
     *
     * @param affine_mat Optional sprite_affine_mat_ptr to copy to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat);

    /**
     * @brief Sets or removes the sprite_affine_mat_ptr to attach to the sprites to generate.
     *
     * This method calls set_remove_affine_mat_when_not_needed(false)
     * if the given sprite_affine_mat_ptr is not null.
     *
     * @param affine_mat Optional sprite_affine_mat_ptr to move to the builder.
     * @return Reference to this.
     */
    sprite_builder& set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat);

    /**
     * @brief Removes the sprite_affine_mat_ptr to attach to the sprites to generate.
     * @return Reference to this.
     */
    sprite_builder& remove_affine_mat()
    {
        _affine_mat.reset();
        return *this;
    }

    /**
     * @brief Releases and returns the sprite_affine_mat_ptr to attach to the sprites to generate (if any).
     */
    [[nodiscard]] optional<sprite_affine_mat_ptr> release_affine_mat();

    /**
     * @brief Indicates if the sprite_affine_mat_ptr attached to the sprites to generate (if any) must be removed
     * when it is not needed anymore.
     *
     * A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.
     */
    [[nodiscard]] bool remove_affine_mat_when_not_needed() const
    {
        return _remove_affine_mat_when_not_needed;
    }

    /**
     * @brief Sets if the sprite_affine_mat_ptr attached to the sprites to generate (if any) must be removed
     * when it is not needed anymore.
     *
     * A sprite does not need an sprite_affine_mat_ptr when its flipped_identity() method returns `true`.
     * @param remove_when_not_needed `true` if the sprite_affine_mat_ptr must be removed
     * when it is not needed anymore; `false` otherwise.
     * @return Reference to this.
     */
    sprite_builder& set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)
    {
        _remove_affine_mat_when_not_needed = remove_when_not_needed;
        return *this;
    }

private:
    optional<sprite_item> _item;
    fixed_point _position;
    int _graphics_index;
    int _bg_priority = 3;
    int _z_order = 0;
    optional<sprite_tiles_ptr> _tiles;
    optional<sprite_palette_ptr> _palette;
    optional<sprite_affine_mat_ptr> _affine_mat;
    optional<camera_ptr> _camera;
    sprite_shape_size _shape_size;
    sprite_double_size_mode _double_size_mode = sprite_double_size_mode::AUTO;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _window_enabled = false;
    bool _visible = true;
    bool _remove_affine_mat_when_not_needed = true;
};

}

#endif

