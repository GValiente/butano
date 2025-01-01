/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_MAT_PD_REGISTER_HBE_PTR_H
#define BN_SPRITE_AFFINE_MAT_PD_REGISTER_HBE_PTR_H

/**
 * @file
 * bn::sprite_affine_mat_pd_register_hbe_ptr header file.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_hbe_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes or the values to commit to the fourth GBA register of a sprite_affine_mat_ptr
 * in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */
class sprite_affine_mat_pd_register_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the attributes to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_mat_attributes objects
     * with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pd_register_hbe_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_pd_register_hbe_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pd_register_hbe_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_pd_register_hbe_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    /**
     * @brief Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the attributes to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_mat_attributes objects
     * with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pd_register_hbe_ptr if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_pd_register_hbe_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_pd_register_hbe_ptr if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_pd_register_hbe_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    /**
     * @brief Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat;
    }

    /**
     * @brief Returns the referenced array of 160 affine_mat_attributes objects with the attributes to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const affine_mat_attributes> attributes_ref() const;

    /**
     * @brief Returns the referenced array of 160 values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const int16_t> values_ref() const;

    /**
     * @brief Sets the reference to an array of 160 affine_mat_attributes objects with the attributes to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Sets the reference to an array of 160 values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    void set_values_ref(const span<const int16_t>& values_ref);

    /**
     * @brief Rereads the content of the referenced attributes to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Rereads the content of the referenced values to commit
     * to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
     */
    void reload_values_ref();

    /**
     * @brief Exchanges the contents of this sprite_affine_mat_pd_register_hbe_ptr with those of the other one.
     * @param other sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.
     */
    void swap(sprite_affine_mat_pd_register_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_affine_mat_pd_register_hbe_ptr with those of another one.
     * @param a First sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.
     * @param b Second sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_mat_pd_register_hbe_ptr& a, sprite_affine_mat_pd_register_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    bool _from_attributes;
    sprite_affine_mat_ptr _affine_mat;

    sprite_affine_mat_pd_register_hbe_ptr(int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat);
};

}

#endif
