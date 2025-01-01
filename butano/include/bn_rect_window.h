/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECT_WINDOW_H
#define BN_RECT_WINDOW_H

/**
 * @file
 * bn::rect_window header file.
 *
 * @ingroup rect_window
 */

#include "bn_optional.h"
#include "bn_fixed_rect.h"
#include "bn_inside_window.h"
#include "bn_top_left_fixed_rect.h"

namespace bn
{

class camera_ptr;

/**
 * @brief Rectangular window.
 *
 * @ingroup rect_window
 */
class rect_window : public inside_window
{

public:
    /**
     * @brief Returns the closest window to the screen.
     */
    [[nodiscard]] static rect_window internal()
    {
        return rect_window(window::internal().id());
    }

    /**
     * @brief Returns the closest window to the screen not covered by the internal window.
     */
    [[nodiscard]] static rect_window external()
    {
        return rect_window(window::external().id());
    }

    /**
     * @brief Returns the position of the top boundary of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top() const;

    /**
     * @brief Sets the position of the top boundary of this window (relative to its camera, if it has one).
     */
    void set_top(fixed top);

    /**
     * @brief Returns the position of the left boundary of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed left() const;

    /**
     * @brief Sets the position of the left boundary of this window (relative to its camera, if it has one).
     */
    void set_left(fixed left);

    /**
     * @brief Returns the position of the top-left corner of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] const fixed_point& top_left() const;

    /**
     * @brief Sets the position of the top-left corner of this window (relative to its camera, if it has one).
     * @param top Position of the top boundary of this window (relative to its camera, if it has one).
     * @param left Position of the left boundary of this window (relative to its camera, if it has one).
     */
    void set_top_left(fixed top, fixed left);

    /**
     * @brief Sets the position of the top-left corner of this window (relative to its camera, if it has one).
     */
    void set_top_left(const fixed_point& top_left);

    /**
     * @brief Returns the position of the bottom boundary of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed bottom() const;

    /**
     * @brief Sets the position of the bottom boundary of this window (relative to its camera, if it has one).
     */
    void set_bottom(fixed bottom);

    /**
     * @brief Returns the position of the right boundary of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed right() const;

    /**
     * @brief Sets the position of the right boundary of this window (relative to its camera, if it has one).
     */
    void set_right(fixed right);

    /**
     * @brief Returns the position of the bottom-right corner of this window (relative to its camera, if it has one).
     */
    [[nodiscard]] const fixed_point& bottom_right() const;

    /**
     * @brief Sets the position of the bottom-right corner of this window (relative to its camera, if it has one).
     * @param bottom Position of the bottom boundary of this window (relative to its camera, if it has one).
     * @param right Position of the right boundary of this window (relative to its camera, if it has one).
     */
    void set_bottom_right(fixed bottom, fixed right);

    /**
     * @brief Sets the position of the bottom-right corner of this window (relative to its camera, if it has one).
     */
    void set_bottom_right(const fixed_point& bottom_right);

    /**
     * @brief Returns the boundaries of this window (relative to its camera, if it has one) as a fixed_rect.
     */
    [[nodiscard]] fixed_rect boundaries() const;

    /**
     * @brief Returns the boundaries of this window (relative to its camera, if it has one) as a top_left_fixed_rect.
     */
    [[nodiscard]] top_left_fixed_rect top_left_boundaries() const;

    /**
     * @brief Sets the boundaries of this window (relative to its camera, if it has one).
     * @param top Position of the top boundary of this window (relative to its camera, if it has one).
     * @param left Position of the left boundary of this window (relative to its camera, if it has one).
     * @param bottom Position of the bottom boundary of this window (relative to its camera, if it has one).
     * @param right Position of the right boundary of this window (relative to its camera, if it has one).
     */
    void set_boundaries(fixed top, fixed left, fixed bottom, fixed right);

    /**
     * @brief Sets the boundaries of this window (relative to its camera, if it has one).
     * @param top_left Position of the top-left corner of this window (relative to its camera, if it has one).
     * @param bottom_right Position of the bottom-right corner of this window (relative to its camera, if it has one).
     */
    void set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right);

    /**
     * @brief Sets the boundaries of this window (relative to its camera, if it has one).
     */
    void set_boundaries(const fixed_rect& boundaries);

    /**
     * @brief Sets the boundaries of this window (relative to its camera, if it has one).
     */
    void set_boundaries(const top_left_fixed_rect& boundaries);

    /**
     * @brief Restores the boundaries of this window to its initial values.
     */
    void restore_boundaries();

    /**
     * @brief Returns the camera_ptr attached to this rect window (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const;

    /**
     * @brief Sets the camera_ptr attached to this rect window.
     * @param camera camera_ptr to copy to this rect window.
     */
    void set_camera(const camera_ptr& camera);

    /**
     * @brief Sets the camera_ptr attached to this rect window.
     * @param camera camera_ptr to move to this rect window.
     */
    void set_camera(camera_ptr&& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this rect window.
     * @param camera Optional camera_ptr to copy to this rect window.
     */
    void set_camera(const optional<camera_ptr>& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this rect window.
     * @param camera Optional camera_ptr to move to this rect window.
     */
    void set_camera(optional<camera_ptr>&& camera);

    /**
     * @brief Removes the camera_ptr attached to this rect window (if any).
     */
    void remove_camera();

private:
    friend class window;

    explicit rect_window(int id) :
        inside_window(id)
    {
    }
};

}

#endif
