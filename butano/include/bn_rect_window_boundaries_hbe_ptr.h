/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RECT_WINDOW_BOUNDARIES_HBE_PTR_H
#define BN_RECT_WINDOW_BOUNDARIES_HBE_PTR_H

/**
 * @file
 * bn::rect_window_boundaries_hbe_ptr header file.
 *
 * @ingroup rect_window
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_rect_window.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the boundaries of a rect window in each screen horizontal line.
 *
 * @ingroup rect_window
 * @ingroup hblank_effect
 */
class rect_window_boundaries_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a rect_window_boundaries_hbe_ptr which changes the horizontal boundaries
     * of a rect window in each screen horizontal line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal boundaries of the given
     * rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hbe_ptr.
     */
    [[nodiscard]] static rect_window_boundaries_hbe_ptr create_horizontal(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hbe_ptr which changes the vertical boundaries
     * of a rect window in each screen vertical line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical boundaries of the given
     * rect window in each screen vertical line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hbe_ptr.
     */
    [[nodiscard]] static rect_window_boundaries_hbe_ptr create_vertical(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hbe_ptr which changes the horizontal boundaries
     * of a rect window in each screen horizontal line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal boundaries of the given
     * rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<rect_window_boundaries_hbe_ptr> create_horizontal_optional(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hbe_ptr which changes the vertical boundaries
     * of a rect window in each screen vertical line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical boundaries of the given
     * rect window in each screen vertical line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<rect_window_boundaries_hbe_ptr> create_vertical_optional(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Returns the rect window modified by this H-Blank effect.
     */
    [[nodiscard]] const rect_window& window() const
    {
        return _window;
    }

    /**
     * @brief Returns the referenced array of 160 deltas to add to the horizontal or vertical boundaries
     * of the managed rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     */
    [[nodiscard]] span<const pair<fixed, fixed>> deltas_ref() const;

    /**
     * @brief Sets the reference to an array of 160 deltas to add to the horizontal or vertical boundaries
     * of the managed rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     */
    void set_deltas_ref(const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Rereads the content of the referenced deltas to add to the horizontal or vertical boundaries
     * of the managed rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
     * to avoid dangling references.
     */
    void reload_deltas_ref();

    /**
     * @brief Exchanges the contents of this rect_window_boundaries_hbe_ptr with those of the other one.
     * @param other rect_window_boundaries_hbe_ptr to exchange the contents with.
     */
    void swap(rect_window_boundaries_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a rect_window_boundaries_hbe_ptr with those of another one.
     * @param a First rect_window_boundaries_hbe_ptr to exchange the contents with.
     * @param b Second rect_window_boundaries_hbe_ptr to exchange the contents with.
     */
    friend void swap(rect_window_boundaries_hbe_ptr& a, rect_window_boundaries_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    rect_window _window;

    rect_window_boundaries_hbe_ptr(int id, rect_window window);
};

}

#endif
