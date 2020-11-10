/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_PTR_H
#define BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_PTR_H

/**
 * @file
 * btn::bg_palettes_transparent_color_hblank_effect_ptr header file.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup hblank_effect
 */

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class color;

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the transparent color of the backgrounds in each screen horizontal line.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup hblank_effect
 */
class bg_palettes_transparent_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a bg_palettes_transparent_color_hblank_effect_ptr
     * which changes the transparent color of the backgrounds in each screen horizontal line.
     * @param colors_ref Reference to an array of 160 color objects
     * which indicate the transparent color of the backgrounds in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive
     * the bg_palettes_transparent_color_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested bg_palettes_transparent_color_hblank_effect_ptr.
     */
    [[nodiscard]] static bg_palettes_transparent_color_hblank_effect_ptr create(
            const span<const color>& colors_ref);

    /**
     * @brief Creates a bg_palettes_transparent_color_hblank_effect_ptr
     * which changes the transparent color of the backgrounds in each screen horizontal line.
     * @param colors_ref Reference to an array of 160 color objects
     * which indicate the transparent color of the backgrounds in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive
     * the bg_palettes_transparent_color_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested bg_palettes_transparent_color_hblank_effect_ptr if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] static optional<bg_palettes_transparent_color_hblank_effect_ptr> create_optional(
            const span<const color>& colors_ref);

    /**
     * @brief Returns the referenced array of 160 color objects
     * which indicate the transparent color of the backgrounds in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive
     * the bg_palettes_transparent_color_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const color> colors_ref() const;

    /**
     * @brief Sets the reference to an array of 160 color objects
     * which indicate the transparent color of the backgrounds in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive
     * the bg_palettes_transparent_color_hblank_effect_ptr to avoid dangling references.
     */
    void set_colors_ref(const span<const color>& colors_ref);

    /**
     * @brief Rereads the content of the referenced color objects
     * which indicate the transparent color of the backgrounds in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive
     * the bg_palettes_transparent_color_hblank_effect_ptr to avoid dangling references.
     */
    void reload_colors_ref();

    /**
     * @brief Exchanges the contents of this bg_palettes_transparent_color_hblank_effect_ptr
     * with those of the other one.
     * @param other bg_palettes_transparent_color_hblank_effect_ptr to exchange the contents with.
     */
    void swap(bg_palettes_transparent_color_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a bg_palettes_transparent_color_hblank_effect_ptr with those of another one.
     * @param a First bg_palettes_transparent_color_hblank_effect_ptr to exchange the contents with.
     * @param b Second bg_palettes_transparent_color_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(bg_palettes_transparent_color_hblank_effect_ptr& a,
                     bg_palettes_transparent_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bg_palettes_transparent_color_hblank_effect_ptr(int id);
};

}

#endif
