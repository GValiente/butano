/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_PTR_H
#define BN_AFFINE_BG_PTR_H

/**
 * @file
 * bn::affine_bg_ptr header file.
 *
 * @ingroup affine_bg
 */

#include "bn_optional.h"
#include "bn_fixed_point.h"

namespace bn
{

class size;
class window;
class camera_ptr;
class bg_palette_ptr;
class affine_bg_item;
class bg_palette_item;
class affine_bg_builder;
class affine_bg_map_ptr;
class affine_bg_map_item;
class affine_bg_tiles_ptr;
class affine_bg_tiles_item;
class affine_bg_attributes;
class affine_mat_attributes;
class affine_bg_mat_attributes;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of an affine background.
 *
 * Several affine_bg_ptr objects may own the same affine background.
 *
 * The affine background is released when the last remaining affine_bg_ptr owning it is destroyed.
 *
 * @ingroup affine_bg
 */
class affine_bg_ptr
{

public:
    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(fixed x, fixed y, const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(fixed x, fixed y, const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param position Position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(const fixed_point& position, const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param position Position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(
            const fixed_point& position, const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(fixed x, fixed y, affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param position Position of the affine background.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(const fixed_point& position, affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from an affine_bg_builder reference.
     * @param builder affine_bg_builder reference.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(const affine_bg_builder& builder);

    /**
     * @brief Creates an affine_bg_ptr from a moved affine_bg_builder.
     * @param builder affine_bg_builder to move.
     * @return The requested affine_bg_ptr.
     */
    [[nodiscard]] static affine_bg_ptr create(affine_bg_builder&& builder);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(fixed x, fixed y, const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(
            fixed x, fixed y, const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param position Position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(
            const fixed_point& position, const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_item.
     * @param position Position of the affine background.
     * @param item affine_bg_item containing the required information to generate the affine background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(
            const fixed_point& position, const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param x Horizontal position of the affine background.
     * @param y Vertical position of the affine background.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(fixed x, fixed y, affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from the given affine_bg_map_ptr.
     * @param position Position of the affine background.
     * @param map affine_bg_map_ptr used by the affine background.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(
            const fixed_point& position, affine_bg_map_ptr map);

    /**
     * @brief Creates an affine_bg_ptr from an affine_bg_builder reference.
     * @param builder affine_bg_builder reference.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(const affine_bg_builder& builder);

    /**
     * @brief Creates an affine_bg_ptr from a moved affine_bg_builder.
     * @param builder affine_bg_builder to move.
     * @return The requested affine_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_ptr> create_optional(affine_bg_builder&& builder);

    /**
     * @brief Copy constructor.
     * @param other affine_bg_ptr to copy.
     */
    affine_bg_ptr(const affine_bg_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other affine_bg_ptr to copy.
     * @return Reference to this.
     */
    affine_bg_ptr& operator=(const affine_bg_ptr& other);

    /**
     * @brief Move constructor.
     * @param other affine_bg_ptr to move.
     */
    affine_bg_ptr(affine_bg_ptr&& other) noexcept :
        affine_bg_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    /**
     * @brief Move assignment operator.
     * @param other affine_bg_ptr to move.
     * @return Reference to this.
     */
    affine_bg_ptr& operator=(affine_bg_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced affine background if no more affine_bg_ptr objects reference to it.
     */
    ~affine_bg_ptr();

    /**
     * @brief Returns the size in pixels of the affine background.
     */
    [[nodiscard]] size dimensions() const;

    /**
     * @brief Indicates if this affine background is big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] bool big() const;

    /**
     * @brief Returns the tiles used by this affine background.
     */
    [[nodiscard]] const affine_bg_tiles_ptr& tiles() const;

    /**
     * @brief Sets the tiles used by this affine background.
     * @param tiles affine_bg_tiles_ptr to copy.
     *
     * It must be compatible with the current map of the affine background.
     */
    void set_tiles(const affine_bg_tiles_ptr& tiles);

    /**
     * @brief Sets the tiles used by this affine background.
     * @param tiles affine_bg_tiles_ptr to move.
     *
     * It must be compatible with the current map of the affine background.
     */
    void set_tiles(affine_bg_tiles_ptr&& tiles);

    /**
     * @brief Replaces the tiles used by this affine background
     * with a new tile set created with the given affine_bg_tiles_item.
     *
     * Before creating a new background tile set,
     * the affine_bg_tiles_ptr used by this affine background is removed, so VRAM usage is reduced.
     *
     * The new background tiles must be compatible with the current map of the affine background.
     *
     * @param tiles_item It creates the new background tiles to use by this affine background.
     */
    void set_tiles(const affine_bg_tiles_item& tiles_item);

    /**
     * @brief Returns the color palette used by this affine background.
     */
    [[nodiscard]] const bg_palette_ptr& palette() const;

    /**
     * @brief Sets the color palette to use by this affine background.
     * @param palette bg_palette_ptr to copy.
     */
    void set_palette(const bg_palette_ptr& palette);

    /**
     * @brief Sets the color palette to use by this affine background.
     * @param palette bg_palette_ptr to move.
     */
    void set_palette(bg_palette_ptr&& palette);

    /**
     * @brief Replaces the color palette used by this affine background
     * with a new one created with the given bg_palette_item.
     *
     * Before creating a new color palette, the bg_palette_ptr used by this affine background is removed,
     * so VRAM usage is reduced.
     *
     * @param palette_item It creates the color palette to use by this affine background.
     */
    void set_palette(const bg_palette_item& palette_item);

    /**
     * @brief Returns the map used by this affine background.
     */
    [[nodiscard]] const affine_bg_map_ptr& map() const;

    /**
     * @brief Sets the map used by this affine background.
     * @param map affine_bg_map_ptr to copy.
     */
    void set_map(const affine_bg_map_ptr& map);

    /**
     * @brief Sets the map used by this affine background.
     * @param map affine_bg_map_ptr to move.
     */
    void set_map(affine_bg_map_ptr&& map);

    /**
     * @brief Replaces the map used by this affine background
     * with a new map created with the given affine_bg_map_item.
     *
     * Before creating a new map, the affine_bg_map_ptr used by this affine background is removed,
     * so VRAM usage is reduced.
     *
     * @param map_item It creates the new map to use by this affine background.
     */
    void set_map(const affine_bg_map_item& map_item);

    /**
     * @brief Replaces the map used by this affine background
     * with a new map created with the given affine_bg_map_item.
     *
     * Before creating a new map, the affine_bg_map_ptr used by this affine background is removed,
     * so VRAM usage is reduced.
     *
     * @param map_item It creates the new map to use by this affine background.
     * @param map_index Index of the map to reference in map_item.
     */
    void set_map(const affine_bg_map_item& map_item, int map_index);

    /**
     * @brief Replaces the tiles, the color palette and the map used by this affine background
     * with the created with the given affine_bg_item.
     *
     * Before creating new resources, the resources used by this affine background are removed,
     * so VRAM usage is reduced.
     *
     * @param item It creates the resources to use by this affine background.
     */
    void set_item(const affine_bg_item& item);

    /**
     * @brief Replaces the tiles, the color palette and the map used by this affine background
     * with the created with the given affine_bg_item.
     *
     * Before creating new resources, the resources used by this affine background are removed,
     * so VRAM usage is reduced.
     *
     * @param item It creates the resources to use by this affine background.
     * @param map_index Index of the map to reference in item.map_item().
     */
    void set_item(const affine_bg_item& item, int map_index);

    /**
     * @brief Returns the horizontal position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed x() const;

    /**
     * @brief Sets the horizontal position of the affine background (relative to its camera, if it has one).
     */
    void set_x(fixed x);

    /**
     * @brief Returns the vertical position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed y() const;

    /**
     * @brief Sets the vertical position of the affine background (relative to its camera, if it has one).
     */
    void set_y(fixed y);

    /**
     * @brief Returns the position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] const fixed_point& position() const;

    /**
     * @brief Sets the position of the affine background (relative to its camera, if it has one).
     * @param x Horizontal position of the affine background (relative to its camera, if it has one).
     * @param y Vertical position of the affine background (relative to its camera, if it has one).
     */
    void set_position(fixed x, fixed y);

    /**
     * @brief Sets the position of the affine background (relative to its camera, if it has one).
     */
    void set_position(const fixed_point& position);

    /**
     * @brief Returns the horizontal top-left position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the affine background (relative to its camera, if it has one).
     */
    void set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the affine background (relative to its camera, if it has one).
     */
    void set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the affine background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the affine background (relative to its camera, if it has one).
     * @param top_left_x Horizontal top-left position of the affine background (relative to its camera, if it has one).
     * @param top_left_y Vertical top-left position of the affine background (relative to its camera, if it has one).
     */
    void set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the affine background (relative to its camera, if it has one).
     */
    void set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the rotation angle in degrees of the affine background.
     */
    [[nodiscard]] fixed rotation_angle() const;

    /**
     * @brief Sets the rotation angle in degrees of the affine background.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     */
    void set_rotation_angle(fixed rotation_angle);

    /**
     * @brief Sets the rotation angle in degrees of the affine background.
     * @param rotation_angle Rotation angle in degrees, in any range.
     */
    void set_rotation_angle_safe(fixed rotation_angle);

    /**
     * @brief Returns the horizontal scale of the affine background.
     */
    [[nodiscard]] fixed horizontal_scale() const;

    /**
     * @brief Sets the horizontal scale of the affine background.
     */
    void set_horizontal_scale(fixed horizontal_scale);

    /**
     * @brief Returns the vertical scale of the affine background.
     */
    [[nodiscard]] fixed vertical_scale() const;

    /**
     * @brief Sets the vertical scale of the affine background.
     */
    void set_vertical_scale(fixed vertical_scale);

    /**
     * @brief Sets the scale of the affine background.
     */
    void set_scale(fixed scale);

    /**
     * @brief Sets the scale of the affine background.
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     */
    void set_scale(fixed horizontal_scale, fixed vertical_scale);

    /**
     * @brief Returns the horizontal shear of the affine background.
     */
    [[nodiscard]] fixed horizontal_shear() const;

    /**
     * @brief Sets the horizontal shear of the affine background.
     */
    void set_horizontal_shear(fixed horizontal_shear);

    /**
     * @brief Returns the vertical shear of the affine background.
     */
    [[nodiscard]] fixed vertical_shear() const;

    /**
     * @brief Sets the vertical shear of the affine background.
     */
    void set_vertical_shear(fixed vertical_shear);

    /**
     * @brief Sets the shear of the affine background.
     */
    void set_shear(fixed shear);

    /**
     * @brief Sets the shear of the affine background.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     */
    void set_shear(fixed horizontal_shear, fixed vertical_shear);

    /**
     * @brief Indicates if this affine background is flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const;

    /**
     * @brief Sets if this affine background must be flipped in the horizontal axis or not.
     */
    void set_horizontal_flip(bool horizontal_flip);

    /**
     * @brief Indicates if this affine background must be flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const;

    /**
     * @brief Sets if this affine background must be flipped in the vertical axis or not.
     */
    void set_vertical_flip(bool vertical_flip);

    /**
     * @brief Returns the horizontal position of the affine transformations pivot point of this affine background.
     */
    [[nodiscard]] fixed pivot_x() const;

    /**
     * @brief Sets the horizontal position of the affine transformations pivot point of this affine background.
     */
    void set_pivot_x(fixed pivot_x);

    /**
     * @brief Returns the vertical position of the affine transformations pivot point of this affine background.
     */
    [[nodiscard]] fixed pivot_y() const;

    /**
     * @brief Sets the vertical position of the affine transformations pivot point of this affine background.
     */
    void set_pivot_y(fixed pivot_y);

    /**
     * @brief Returns the position of the affine transformations pivot point of this affine background.
     */
    [[nodiscard]] const fixed_point& pivot_position() const;

    /**
     * @brief Sets the position of the affine transformations pivot point of this affine background.
     * @param pivot_x Horizontal position of the pivot point.
     * @param pivot_y Vertical position of the pivot point.
     */
    void set_pivot_position(fixed pivot_x, fixed pivot_y);

    /**
     * @brief Sets the position of the affine transformations pivot point of this affine background.
     * @param pivot_position Position of the pivot point.
     */
    void set_pivot_position(const fixed_point& pivot_position);

    /**
     * @brief Returns the attributes of the position and the transformation matrix of this affine background.
     */
    [[nodiscard]] const affine_bg_mat_attributes& mat_attributes() const;

    /**
     * @brief Sets the attributes of the affine transformation matrix of this affine background.
     */
    void set_mat_attributes(const affine_mat_attributes& mat_attributes);

    /**
     * @brief Returns the priority of the affine background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const;

    /**
     * @brief Sets the priority of the affine background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     */
    void set_priority(int priority);

    /**
     * @brief Returns the priority of the affine background relative to other backgrounds, excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     */
    [[nodiscard]] int z_order() const;

    /**
     * @brief Sets the priority of the affine background relative to other backgrounds, excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     *
     * @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
     */
    void set_z_order(int z_order);

    /**
     * @brief Modify this affine background to be drawn above all of the other backgrounds with the same priority.
     */
    void put_above();

    /**
     * @brief Modify this affine background to be drawn below all of the other backgrounds with the same priority.
     */
    void put_below();

    /**
     * @brief Indicates if this affine background wraps around at the edges or not.
     */
    [[nodiscard]] bool wrapping_enabled() const;

    /**
     * @brief Sets if this affine background must wrap around at the edges or not.
     */
    void set_wrapping_enabled(bool wrapping_enabled);

    /**
     * @brief Indicates if the mosaic effect must be applied to this affine background or not.
     */
    [[nodiscard]] bool mosaic_enabled() const;

    /**
     * @brief Sets if the mosaic effect must be applied to this affine background or not.
     */
    void set_mosaic_enabled(bool mosaic_enabled);

    /**
     * @brief Indicates if blending must be applied to this affine background or not.
     *
     * Blending is applied to this affine background by making it part of the blending top layer.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return blending_top_enabled();
    }

    /**
     * @brief Sets if blending must be applied to this affine background or not.
     *
     * Blending is applied to this affine background by making it part of the blending top layer.
     */
    void set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
    }

    /**
     * @brief Indicates if this affine background is part of the blending top layer or not.
     */
    [[nodiscard]] bool blending_top_enabled() const;

    /**
     * @brief Sets if this affine background is part of the blending top layer or not.
     */
    void set_blending_top_enabled(bool blending_top_enabled);

    /**
     * @brief Indicates if this affine background is part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled() const;

    /**
     * @brief Sets if this affine background is part of the blending bottom layer or not.
     */
    void set_blending_bottom_enabled(bool blending_bottom_enabled);

    /**
     * @brief Indicates if this affine background must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const;

    /**
     * @brief Sets if this affine background must be committed to the GBA or not.
     */
    void set_visible(bool visible);

    /**
     * @brief Indicates if this affine background is visible in the given window or not.
     */
    [[nodiscard]] bool visible_in_window(const window& window) const;

    /**
     * @brief Sets if this affine background must be visible in the given window or not.
     */
    void set_visible_in_window(bool visible, window& window);

    /**
     * @brief Returns the camera_ptr attached to this affine background (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const;

    /**
     * @brief Sets the camera_ptr attached to this affine background.
     * @param camera camera_ptr to copy to this affine background.
     */
    void set_camera(const camera_ptr& camera);

    /**
     * @brief Sets the camera_ptr attached to this affine background.
     * @param camera camera_ptr to move to this affine background.
     */
    void set_camera(camera_ptr&& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this affine background.
     * @param camera Optional camera_ptr to copy to this affine background.
     */
    void set_camera(const optional<camera_ptr>& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this affine background.
     * @param camera Optional camera_ptr to move to this affine background.
     */
    void set_camera(optional<camera_ptr>&& camera);

    /**
     * @brief Removes the camera_ptr attached to this affine background (if any).
     */
    void remove_camera();

    /**
     * @brief Returns the attributes to commit to the GBA for this affine background.
     */
    [[nodiscard]] affine_bg_attributes attributes() const;

    /**
     * @brief Sets the attributes to commit to the GBA for this affine background.
     */
    void set_attributes(const affine_bg_attributes& attributes);

    /**
     * @brief Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
     *
     * Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
     *
     * A background doesn't have an assigned hardware ID if it is not visible.
     *
     * Assigned hardware ID can change after calling this method
     * if some properties of this background or others are updated.
     *
     * Call this method at your own risk.
     */
    [[nodiscard]] optional<int> hw_id() const;

    /**
     * @brief Returns the internal handle.
     */
    [[nodiscard]] const void* handle() const
    {
        return _handle;
    }

    /**
     * @brief Exchanges the contents of this affine_bg_ptr with those of the other one.
     * @param other affine_bg_ptr to exchange the contents with.
     */
    void swap(affine_bg_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of an affine_bg_ptr with those of another one.
     * @param a First affine_bg_ptr to exchange the contents with.
     * @param b Second affine_bg_ptr to exchange the contents with.
     */
    friend void swap(affine_bg_ptr& a, affine_bg_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const affine_bg_ptr& a, const affine_bg_ptr& b) = default;

private:
    using handle_type = void*;

    handle_type _handle;

    explicit affine_bg_ptr(handle_type handle) :
        _handle(handle)
    {
    }
};


/**
 * @brief Hash support for affine_bg_ptr.
 *
 * @ingroup affine_bg
 * @ingroup functional
 */
template<>
struct hash<affine_bg_ptr>
{
    /**
     * @brief Returns the hash of the given affine_bg_ptr.
     */
    [[nodiscard]] unsigned operator()(const affine_bg_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
