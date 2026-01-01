/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DP_DIRECT_BITMAP_BG_PTR_H
#define BN_DP_DIRECT_BITMAP_BG_PTR_H

/**
 * @file
 * bn::dp_direct_bitmap_bg_ptr header file.
 *
 * @ingroup dp_direct_bitmap_bg
 */

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_fixed_point.h"

namespace bn
{

class size;
class color;
class window;
class camera_ptr;
class bitmap_bg_attributes;
class affine_mat_attributes;
class affine_bg_mat_attributes;
class dp_direct_bitmap_bg_builder;
enum class green_swap_mode : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a dual page direct bitmap background.
 *
 * Several dp_direct_bitmap_bg_ptr objects may own the same background.
 *
 * The background is released when the last remaining dp_direct_bitmap_bg_ptr owning it is destroyed.
 *
 * @ingroup dp_direct_bitmap_bg
 */
class dp_direct_bitmap_bg_ptr
{

public:
    /**
     * @brief Returns the size in pixels of the backgrounds.
     */
    [[nodiscard]] static size dimensions();

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr.
     */
    [[nodiscard]] static dp_direct_bitmap_bg_ptr create();

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
     * @param builder dp_direct_bitmap_bg_builder reference.
     * @return The requested dp_direct_bitmap_bg_ptr.
     */
    [[nodiscard]] static dp_direct_bitmap_bg_ptr create(const dp_direct_bitmap_bg_builder& builder);

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr from a moved dp_direct_bitmap_bg_builder.
     * @param builder dp_direct_bitmap_bg_builder to move.
     * @return The requested dp_direct_bitmap_bg_ptr.
     */
    [[nodiscard]] static dp_direct_bitmap_bg_ptr create(dp_direct_bitmap_bg_builder&& builder);

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
     */
    [[nodiscard]] static optional<dp_direct_bitmap_bg_ptr> create_optional();

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr from a dp_direct_bitmap_bg_builder reference.
     * @param builder dp_direct_bitmap_bg_builder reference.
     * @return The requested dp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<dp_direct_bitmap_bg_ptr> create_optional(const dp_direct_bitmap_bg_builder& builder);

    /**
     * @brief Creates a dp_direct_bitmap_bg_ptr from a moved dp_direct_bitmap_bg_builder.
     * @param builder dp_direct_bitmap_bg_builder to move.
     * @return The requested dp_direct_bitmap_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<dp_direct_bitmap_bg_ptr> create_optional(dp_direct_bitmap_bg_builder&& builder);

    /**
     * @brief Copy constructor.
     * @param other dp_direct_bitmap_bg_ptr to copy.
     */
    dp_direct_bitmap_bg_ptr(const dp_direct_bitmap_bg_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other dp_direct_bitmap_bg_ptr to copy.
     * @return Reference to this.
     */
    dp_direct_bitmap_bg_ptr& operator=(const dp_direct_bitmap_bg_ptr& other);

    /**
     * @brief Move constructor.
     * @param other dp_direct_bitmap_bg_ptr to move.
     */
    dp_direct_bitmap_bg_ptr(dp_direct_bitmap_bg_ptr&& other) noexcept :
        dp_direct_bitmap_bg_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    /**
     * @brief Move assignment operator.
     * @param other dp_direct_bitmap_bg_ptr to move.
     * @return Reference to this.
     */
    dp_direct_bitmap_bg_ptr& operator=(dp_direct_bitmap_bg_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced background if no more dp_direct_bitmap_bg_ptr objects reference to it.
     */
    ~dp_direct_bitmap_bg_ptr();

    /**
     * @brief Returns a span of the hidden frame buffer.
     */
    [[nodiscard]] span<const color> page() const;

    /**
     * @brief Returns a span of the hidden frame buffer.
     */
    [[nodiscard]] span<color> page();

    /**
     * @brief Swaps the hidden frame buffer in the next core::update call.
     */
    void flip_page_later();

    /**
     * @brief Swaps the hidden frame buffer without waiting to the next core::update call.
     *
     * Expect lack of vsync issues like screen tearing.
     */
    void flip_page_now();

    /**
     * @brief Returns the horizontal position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed x() const;

    /**
     * @brief Sets the horizontal position of the background (relative to its camera, if it has one).
     */
    void set_x(fixed x);

    /**
     * @brief Returns the vertical position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed y() const;

    /**
     * @brief Sets the vertical position of the background (relative to its camera, if it has one).
     */
    void set_y(fixed y);

    /**
     * @brief Returns the position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] const fixed_point& position() const;

    /**
     * @brief Sets the position of the background (relative to its camera, if it has one).
     * @param x Horizontal position of the background (relative to its camera, if it has one).
     * @param y Vertical position of the background (relative to its camera, if it has one).
     */
    void set_position(fixed x, fixed y);

    /**
     * @brief Sets the position of the background (relative to its camera, if it has one).
     */
    void set_position(const fixed_point& position);

    /**
     * @brief Returns the horizontal top-left position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the background (relative to its camera, if it has one).
     */
    void set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the background (relative to its camera, if it has one).
     */
    void set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the background (relative to its camera, if it has one).
     * @param top_left_x Horizontal top-left position of the background (relative to its camera, if it has one).
     * @param top_left_y Vertical top-left position of the background (relative to its camera, if it has one).
     */
    void set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the background (relative to its camera, if it has one).
     */
    void set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the rotation angle in degrees of the background.
     */
    [[nodiscard]] fixed rotation_angle() const;

    /**
     * @brief Sets the rotation angle in degrees of the background.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     */
    void set_rotation_angle(fixed rotation_angle);

    /**
     * @brief Sets the rotation angle in degrees of the background.
     * @param rotation_angle Rotation angle in degrees, in any range.
     */
    void set_rotation_angle_safe(fixed rotation_angle);

    /**
     * @brief Returns the horizontal scale of the background.
     */
    [[nodiscard]] fixed horizontal_scale() const;

    /**
     * @brief Sets the horizontal scale of the background.
     */
    void set_horizontal_scale(fixed horizontal_scale);

    /**
     * @brief Returns the vertical scale of the background.
     */
    [[nodiscard]] fixed vertical_scale() const;

    /**
     * @brief Sets the vertical scale of the background.
     */
    void set_vertical_scale(fixed vertical_scale);

    /**
     * @brief Sets the scale of the background.
     */
    void set_scale(fixed scale);

    /**
     * @brief Sets the scale of the background.
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     */
    void set_scale(fixed horizontal_scale, fixed vertical_scale);

    /**
     * @brief Scales the background to fill the screen.
     */
    void expand();

    /**
     * @brief Returns the horizontal shear of the background.
     */
    [[nodiscard]] fixed horizontal_shear() const;

    /**
     * @brief Sets the horizontal shear of the background.
     */
    void set_horizontal_shear(fixed horizontal_shear);

    /**
     * @brief Returns the vertical shear of the background.
     */
    [[nodiscard]] fixed vertical_shear() const;

    /**
     * @brief Sets the vertical shear of the background.
     */
    void set_vertical_shear(fixed vertical_shear);

    /**
     * @brief Sets the shear of the background.
     */
    void set_shear(fixed shear);

    /**
     * @brief Sets the shear of the background.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     */
    void set_shear(fixed horizontal_shear, fixed vertical_shear);

    /**
     * @brief Indicates if this background is flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const;

    /**
     * @brief Sets if this background must be flipped in the horizontal axis or not.
     */
    void set_horizontal_flip(bool horizontal_flip);

    /**
     * @brief Indicates if this background must be flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const;

    /**
     * @brief Sets if this background must be flipped in the vertical axis or not.
     */
    void set_vertical_flip(bool vertical_flip);

    /**
     * @brief Returns the horizontal position of the affine transformations pivot point of this background.
     */
    [[nodiscard]] fixed pivot_x() const;

    /**
     * @brief Sets the horizontal position of the affine transformations pivot point of this background.
     */
    void set_pivot_x(fixed pivot_x);

    /**
     * @brief Returns the vertical position of the affine transformations pivot point of this background.
     */
    [[nodiscard]] fixed pivot_y() const;

    /**
     * @brief Sets the vertical position of the affine transformations pivot point of this background.
     */
    void set_pivot_y(fixed pivot_y);

    /**
     * @brief Returns the position of the affine transformations pivot point of this background.
     */
    [[nodiscard]] const fixed_point& pivot_position() const;

    /**
     * @brief Sets the position of the affine transformations pivot point of this background.
     * @param pivot_x Horizontal position of the pivot point.
     * @param pivot_y Vertical position of the pivot point.
     */
    void set_pivot_position(fixed pivot_x, fixed pivot_y);

    /**
     * @brief Sets the position of the affine transformations pivot point of this background.
     * @param pivot_position Position of the pivot point.
     */
    void set_pivot_position(const fixed_point& pivot_position);

    /**
     * @brief Returns the attributes of the position and the transformation matrix of this background.
     */
    [[nodiscard]] const affine_bg_mat_attributes& mat_attributes() const;

    /**
     * @brief Sets the attributes of the affine transformation matrix of this background.
     */
    void set_mat_attributes(const affine_mat_attributes& mat_attributes);

    /**
     * @brief Returns the priority of the background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const;

    /**
     * @brief Sets the priority of the background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     */
    void set_priority(int priority);

    /**
     * @brief Indicates if the mosaic effect must be applied to this background or not.
     */
    [[nodiscard]] bool mosaic_enabled() const;

    /**
     * @brief Sets if the mosaic effect must be applied to this background or not.
     */
    void set_mosaic_enabled(bool mosaic_enabled);

    /**
     * @brief Indicates if blending must be applied to this background or not.
     *
     * Blending is applied to this background by making it part of the blending top layer.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return blending_top_enabled();
    }

    /**
     * @brief Sets if blending must be applied to this background or not.
     *
     * Blending is applied to this background by making it part of the blending top layer.
     */
    void set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
    }

    /**
     * @brief Indicates if this background is part of the blending top layer or not.
     */
    [[nodiscard]] bool blending_top_enabled() const;

    /**
     * @brief Sets if this background is part of the blending top layer or not.
     */
    void set_blending_top_enabled(bool blending_top_enabled);

    /**
     * @brief Indicates if this background is part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled() const;

    /**
     * @brief Sets if this background is part of the blending bottom layer or not.
     */
    void set_blending_bottom_enabled(bool blending_bottom_enabled);

    /**
     * @brief Indicates how this background must be displayed when green swap is enabled.
     */
    [[nodiscard]] bn::green_swap_mode green_swap_mode() const;

    /**
     * @brief Sets how this background must be displayed when green swap is enabled.
     */
    void set_green_swap_mode(bn::green_swap_mode green_swap_mode);

    /**
     * @brief Indicates if this background must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const;

    /**
     * @brief Sets if this background must be committed to the GBA or not.
     */
    void set_visible(bool visible);

    /**
     * @brief Indicates if this background is visible in the given window or not.
     */
    [[nodiscard]] bool visible_in_window(const window& window) const;

    /**
     * @brief Sets if this background must be visible in the given window or not.
     */
    void set_visible_in_window(bool visible, window& window);

    /**
     * @brief Returns the camera_ptr attached to this background (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const;

    /**
     * @brief Sets the camera_ptr attached to this background.
     * @param camera camera_ptr to copy to this background.
     */
    void set_camera(const camera_ptr& camera);

    /**
     * @brief Sets the camera_ptr attached to this background.
     * @param camera camera_ptr to move to this background.
     */
    void set_camera(camera_ptr&& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this background.
     * @param camera Optional camera_ptr to copy to this background.
     */
    void set_camera(const optional<camera_ptr>& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this background.
     * @param camera Optional camera_ptr to move to this background.
     */
    void set_camera(optional<camera_ptr>&& camera);

    /**
     * @brief Removes the camera_ptr attached to this background (if any).
     */
    void remove_camera();

    /**
     * @brief Returns the attributes to commit to the GBA for this background.
     */
    [[nodiscard]] bitmap_bg_attributes attributes() const;

    /**
     * @brief Sets the attributes to commit to the GBA for this background.
     */
    void set_attributes(const bitmap_bg_attributes& attributes);

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
     * @brief Exchanges the contents of this dp_direct_bitmap_bg_ptr with those of the other one.
     * @param other dp_direct_bitmap_bg_ptr to exchange the contents with.
     */
    void swap(dp_direct_bitmap_bg_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of an dp_direct_bitmap_bg_ptr with those of another one.
     * @param a First dp_direct_bitmap_bg_ptr to exchange the contents with.
     * @param b Second dp_direct_bitmap_bg_ptr to exchange the contents with.
     */
    friend void swap(dp_direct_bitmap_bg_ptr& a, dp_direct_bitmap_bg_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const dp_direct_bitmap_bg_ptr& a, const dp_direct_bitmap_bg_ptr& b) = default;

private:
    using handle_type = void*;

    handle_type _handle;

    explicit dp_direct_bitmap_bg_ptr(handle_type handle) :
        _handle(handle)
    {
    }
};


/**
 * @brief Hash support for dp_direct_bitmap_bg_ptr.
 *
 * @ingroup dp_direct_bitmap_bg
 * @ingroup functional
 */
template<>
struct hash<dp_direct_bitmap_bg_ptr>
{
    /**
     * @brief Returns the hash of the given dp_direct_bitmap_bg_ptr.
     */
    [[nodiscard]] unsigned operator()(const dp_direct_bitmap_bg_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
