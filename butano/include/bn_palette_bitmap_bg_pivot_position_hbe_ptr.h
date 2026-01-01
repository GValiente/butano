/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_BG_PIVOT_POSITION_HBE_PTR_H
#define BN_PALETTE_BITMAP_BG_PIVOT_POSITION_HBE_PTR_H

/**
 * @file
 * bn::palette_bitmap_bg_pivot_position_hbe_ptr header file.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the position of the affine transformations pivot point of a palette bitmap background
 * in each screen horizontal line.
 *
 * @ingroup affine_bg
 * @ingroup hblank_effect
 */
class palette_bitmap_bg_pivot_position_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes
     * the horizontal position of the affine transformations pivot point of a palette bitmap background
     * in each screen horizontal line.
     * @param bg Background to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal position
     * of the affine transformations pivot point of the given background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pivot_position_hbe_ptr.
     */
    [[nodiscard]] static palette_bitmap_bg_pivot_position_hbe_ptr create_horizontal(
            palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes
     * the vertical position of the affine transformations pivot point of a palette bitmap background
     * in each screen horizontal line.
     * @param bg Background to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical position
     * of the affine transformations pivot point of the given background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pivot_position_hbe_ptr.
     */
    [[nodiscard]] static palette_bitmap_bg_pivot_position_hbe_ptr create_vertical(
            palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes
     * the horizontal position of the affine transformations pivot point of a palette bitmap background
     * in each screen horizontal line.
     * @param bg Background to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal position
     * of the affine transformations pivot point of the given background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<palette_bitmap_bg_pivot_position_hbe_ptr> create_horizontal_optional(
            palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes
     * the vertical position of the affine transformations pivot point of a palette bitmap background
     * in each screen horizontal line.
     * @param bg Background to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical position
     * of the affine transformations pivot point of the given background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<palette_bitmap_bg_pivot_position_hbe_ptr> create_vertical_optional(
            palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref);

    /**
     * @brief Returns the background modified by this H-Blank effect.
     */
    [[nodiscard]] const palette_bitmap_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns the referenced array of 160 deltas to add to the horizontal or vertical position
     * of the affine transformations pivot point of the managed background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const fixed> deltas_ref() const;

    /**
     * @brief Sets the reference to an array of 160 deltas to add to the horizontal or vertical position
     * of the affine transformations pivot point of the managed background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     */
    void set_deltas_ref(const span<const fixed>& deltas_ref);

    /**
     * @brief Rereads the content of the referenced deltas to add to the horizontal or vertical position
     * of the affine transformations pivot point of the managed background in each screen horizontal line.
     *
     * The deltas are not copied but referenced,
     * so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
     */
    void reload_deltas_ref();

    /**
     * @brief Exchanges the contents of this palette_bitmap_bg_pivot_position_hbe_ptr with those of the other one.
     * @param other palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
     */
    void swap(palette_bitmap_bg_pivot_position_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a palette_bitmap_bg_pivot_position_hbe_ptr with those of another one.
     * @param a First palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
     * @param b Second palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
     */
    friend void swap(palette_bitmap_bg_pivot_position_hbe_ptr& a, palette_bitmap_bg_pivot_position_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    palette_bitmap_bg_ptr _bg;

    palette_bitmap_bg_pivot_position_hbe_ptr(int id, palette_bitmap_bg_ptr&& bg);
};

}

#endif
