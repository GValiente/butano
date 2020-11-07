/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_POSITION_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_POSITION_HBLANK_EFFECT_PTR_H

#include "btn_sprite_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes the position of a sprite
 * in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup hblank_effect
 */
class sprite_position_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a sprite_position_hblank_effect_ptr which changes the horizontal position of a sprite
     * in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal position of the given sprite
     * in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_position_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_position_hblank_effect_ptr create_horizontal(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a sprite_position_hblank_effect_ptr which changes the horizontal position of a sprite
     * in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the horizontal position of the given sprite
     * in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_position_hblank_effect_ptr if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_position_hblank_effect_ptr> create_horizontal_optional(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a sprite_position_hblank_effect_ptr which changes the vertical position of a sprite
     * in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical position of the given sprite
     * in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_position_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_position_hblank_effect_ptr create_vertical(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    /**
     * @brief Creates a sprite_position_hblank_effect_ptr which changes the vertical position of a sprite
     * in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param deltas_ref Reference to an array of 160 deltas to add to the vertical position of the given sprite
     * in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     *
     * @return The requested sprite_position_hblank_effect_ptr if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_position_hblank_effect_ptr> create_vertical_optional(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    /**
     * @brief Returns the sprite modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    /**
     * @brief Returns the referenced array of 160 deltas to add to the horizontal or vertical position
     * of the managed sprite in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     */
    [[nodiscard]] span<const fixed> deltas_ref() const;

    /**
     * @brief Sets the reference to an array of 160 deltas to add to the horizontal or vertical position
     * of the managed sprite in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     */
    void set_deltas_ref(const span<const fixed>& deltas_ref);

    /**
     * @brief Rereads the content of the referenced deltas to add to the horizontal or vertical position
     * of the managed sprite in each screen horizontal line.
     *
     * The deltas are not copied but referenced, so they should outlive the sprite_position_hblank_effect_ptr
     * to avoid dangling references.
     */
    void reload_deltas_ref();

    /**
     * @brief Exchanges the contents of this sprite_position_hblank_effect_ptr with those of the other one.
     * @param other sprite_position_hblank_effect_ptr to exchange the contents with.
     */
    void swap(sprite_position_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_position_hblank_effect_ptr with those of another one.
     * @param a First sprite_position_hblank_effect_ptr to exchange the contents with.
     * @param b Second sprite_position_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(sprite_position_hblank_effect_ptr& a, sprite_position_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite;

    sprite_position_hblank_effect_ptr(int id, sprite_ptr&& sprite);
};

}

#endif
