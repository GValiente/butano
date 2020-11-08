/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTE_COLOR_HBLANK_EFFECT_PTR_H
#define BTN_BG_PALETTE_COLOR_HBLANK_EFFECT_PTR_H

#include "btn_bg_palette_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class color;

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the color of a background color palette in each screen horizontal line.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup hblank_effect
 */
class bg_palette_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a bg_palette_color_hblank_effect_ptr which changes
     * the color of a background color palette in each screen horizontal line.
     * @param palette Background color palette to be modified.
     * @param color_index Index of the color of the given background color palette to be modified.
     * @param colors_ref Reference to an array of 160 colors to set to the color of a background color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested bg_palette_color_hblank_effect_ptr.
     */
    [[nodiscard]] static bg_palette_color_hblank_effect_ptr create(
            bg_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    /**
     * @brief Creates a bg_palette_color_hblank_effect_ptr which changes
     * the color of a background color palette in each screen horizontal line.
     * @param palette Background color palette to be modified.
     * @param color_index Index of the color of the given background color palette to be modified.
     * @param colors_ref Reference to an array of 160 colors to set to the color of a background color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested bg_palette_color_hblank_effect_ptr if it could be allocated; `nullopt` otherwise.
     */
    [[nodiscard]] static optional<bg_palette_color_hblank_effect_ptr> create_optional(
            bg_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    /**
     * @brief Returns the background color palette modified by this H-Blank effect.
     */
    [[nodiscard]] const bg_palette_ptr& palette() const
    {
        return _palette;
    }

    /**
     * @brief Returns the index of the color of the given background color palette to be modified.
     */
    [[nodiscard]] int color_index() const
    {
        return _color_index;
    }

    /**
     * @brief Returns the referenced array of 160 colors to set to the color of a background color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    [[nodiscard]] span<const color> colors_ref() const;

    /**
     * @brief Sets the reference to an array of 160 colors to set to the color of a background color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    void set_colors_ref(const span<const color>& colors_ref);

    /**
     * @brief Rereads the content of the referenced colors to set to the color of a background color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    void reload_colors_ref();

    /**
     * @brief Exchanges the contents of this bg_palette_color_hblank_effect_ptr with those of the other one.
     * @param other bg_palette_color_hblank_effect_ptr to exchange the contents with.
     */
    void swap(bg_palette_color_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a bg_palette_color_hblank_effect_ptr with those of another one.
     * @param a First bg_palette_color_hblank_effect_ptr to exchange the contents with.
     * @param b Second bg_palette_color_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(bg_palette_color_hblank_effect_ptr& a, bg_palette_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bg_palette_ptr _palette;
    int16_t _color_index;

    bg_palette_color_hblank_effect_ptr(int id, int color_index, bg_palette_ptr&& palette);
};

}

#endif
