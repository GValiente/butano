/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PALETTE_COLOR_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_PALETTE_COLOR_HBLANK_EFFECT_PTR_H

#include "btn_hblank_effect_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

class color;

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the color of a sprite color palette in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup hblank_effect
 */
class sprite_palette_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief create Creates a sprite_palette_color_hblank_effect_ptr which changes
     * the color of a sprite color palette in each screen horizontal line.
     * @param palette Sprite color palette to be modified.
     * @param color_index Index of the color of the given sprite color palette to be modified.
     * @param colors_ref Reference to an array of 160 colors to set to the color of a sprite color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_palette_color_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_palette_color_hblank_effect_ptr create(
            sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    /**
     * @brief create Creates a sprite_palette_color_hblank_effect_ptr which changes
     * the color of a sprite color palette in each screen horizontal line.
     * @param palette Sprite color palette to be modified.
     * @param color_index Index of the color of the given sprite color palette to be modified.
     * @param colors_ref Reference to an array of 160 colors to set to the color of a sprite color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_palette_color_hblank_effect_ptr if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_palette_color_hblank_effect_ptr> create_optional(
            sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref);

    /**
     * @brief Returns the sprite color palette modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return _palette;
    }

    /**
     * @brief Returns the index of the color of the given sprite color palette to be modified.
     */
    [[nodiscard]] int color_index() const
    {
        return _color_index;
    }

    /**
     * @brief Returns the reference to an array of 160 colors to set to the color of a sprite color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    [[nodiscard]] span<const color> colors_ref() const;

    /**
     * @brief Sets the reference to an array of 160 colors to set to the color of a sprite color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    void set_colors_ref(const span<const color>& colors_ref);

    /**
     * @brief Rereads the content of the referenced colors to set to the color of a sprite color palette
     * in each screen horizontal line.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_color_hblank_effect_ptr
     * to avoid dangling references.
     */
    void reload_colors_ref();

    /**
     * @brief Exchanges the contents of this sprite_palette_color_hblank_effect_ptr with those of the other one.
     * @param other sprite_palette_color_hblank_effect_ptr to exchange the contents with.
     */
    void swap(sprite_palette_color_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_palette_color_hblank_effect_ptr with those of another one.
     * @param a First sprite_palette_color_hblank_effect_ptr to exchange the contents with.
     * @param b Second sprite_palette_color_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(sprite_palette_color_hblank_effect_ptr& a, sprite_palette_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_palette_ptr _palette;
    int16_t _color_index;

    sprite_palette_color_hblank_effect_ptr(int id, int color_index, sprite_palette_ptr&& palette);
};

}

#endif
