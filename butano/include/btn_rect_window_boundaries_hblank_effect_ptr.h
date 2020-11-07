/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_RECT_WINDOW_BOUNDARIES_HBLANK_EFFECT_PTR_H
#define BTN_RECT_WINDOW_BOUNDARIES_HBLANK_EFFECT_PTR_H

#include "btn_rect_window.h"
#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes the boundaries
 * of a rect window in each screen horizontal line.
 *
 * @ingroup rect_window
 * @ingroup hblank_effect
 */
class rect_window_boundaries_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a rect_window_boundaries_hblank_effect_ptr which changes the horizontal boundaries
     * of a rect window in each screen horizontal line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal boundaries of the given
     * rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hblank_effect_ptr.
     */
    [[nodiscard]] static rect_window_boundaries_hblank_effect_ptr create_horizontal(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hblank_effect_ptr which changes the horizontal boundaries
     * of a rect window in each screen horizontal line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal boundaries of the given
     * rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hblank_effect_ptr if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<rect_window_boundaries_hblank_effect_ptr> create_horizontal_optional(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hblank_effect_ptr which changes the vertical boundaries
     * of a rect window in each screen vertical line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical boundaries of the given
     * rect window in each screen vertical line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hblank_effect_ptr.
     */
    [[nodiscard]] static rect_window_boundaries_hblank_effect_ptr create_vertical(
            rect_window window, const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Creates a rect_window_boundaries_hblank_effect_ptr which changes the vertical boundaries
     * of a rect window in each screen vertical line.
     * @param window Rect window to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical boundaries of the given
     * rect window in each screen vertical line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested rect_window_boundaries_hblank_effect_ptr if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<rect_window_boundaries_hblank_effect_ptr> create_vertical_optional(
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
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     */
    [[nodiscard]] span<const pair<fixed, fixed>> deltas_ref() const;

    /**
     * @brief Sets the reference to an array of 160 deltas to add to the horizontal or vertical boundaries
     * of the managed rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     */
    void set_deltas_ref(const span<const pair<fixed, fixed>>& deltas_ref);

    /**
     * @brief Rereads the content of the referenced deltas to add to the horizontal or vertical boundaries
     * of the managed rect window in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hblank_effect_ptr
     * to avoid dangling references.
     */
    void reload_deltas_ref();

    /**
     * @brief Exchanges the contents of this rect_window_boundaries_hblank_effect_ptr with those of the other one.
     * @param other rect_window_boundaries_hblank_effect_ptr to exchange the contents with.
     */
    void swap(rect_window_boundaries_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a rect_window_boundaries_hblank_effect_ptr with those of another one.
     * @param a First rect_window_boundaries_hblank_effect_ptr to exchange the contents with.
     * @param b Second rect_window_boundaries_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(rect_window_boundaries_hblank_effect_ptr& a, rect_window_boundaries_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    rect_window _window;

    rect_window_boundaries_hblank_effect_ptr(int id, rect_window window);
};

}

#endif
