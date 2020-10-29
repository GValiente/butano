/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_MAT_PC_REGISTER_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_AFFINE_MAT_PC_REGISTER_HBLANK_EFFECT_PTR_H

#include "btn_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes or the values to commit to the third GBA register of a sprite_affine_mat_ptr
 * in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */
class sprite_affine_mat_pc_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a sprite_affine_mat_pc_register_hblank_effect_ptr which changes the attributes to commit
     * to the third GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pc_register_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_pc_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_pc_register_hblank_effect_ptr which changes the values to commit
     * to the third GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pc_register_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_pc_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    /**
     * @brief Creates a sprite_affine_mat_pc_register_hblank_effect_ptr which changes the attributes to commit
     * to the third GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pc_register_hblank_effect_ptr if it can be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_pc_register_hblank_effect_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_pc_register_hblank_effect_ptr which changes the values to commit
     * to the third GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pc_register_hblank_effect_ptr if it can be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_pc_register_hblank_effect_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    /**
     * @brief Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat;
    }

    /**
     * @brief Returns the reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const sprite_affine_mat_attributes> attributes_ref() const;

    /**
     * @brief Returns the reference to an array of 160 values to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const int16_t> values_ref() const;

    /**
     * @brief Sets the reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Sets the reference to an array of 160 values to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    void set_values_ref(const span<const int16_t>& values_ref);

    /**
     * @brief Rereads the content of the referenced attributes to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Rereads the content of the referenced values to commit
     * to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pc_register_hblank_effect_ptr to avoid dangling references.
     */
    void reload_values_ref();

    /**
     * @brief Exchanges the contents of this sprite_affine_mat_pc_register_hblank_effect_ptr
     * with those of the other one.
     * @param other sprite_affine_mat_pc_register_hblank_effect_ptr to exchange the contents with.
     */
    void swap(sprite_affine_mat_pc_register_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_affine_mat_pc_register_hblank_effect_ptr
     * with those of another one.
     * @param a First sprite_affine_mat_pc_register_hblank_effect_ptr to exchange the contents with.
     * @param b Second sprite_affine_mat_pc_register_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_mat_pc_register_hblank_effect_ptr& a,
                     sprite_affine_mat_pc_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bool _from_attributes;
    sprite_affine_mat_ptr _affine_mat;

    sprite_affine_mat_pc_register_hblank_effect_ptr(int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat);
};

}

#endif
