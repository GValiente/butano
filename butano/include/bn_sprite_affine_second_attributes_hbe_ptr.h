/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBE_PTR_H
#define BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::sprite_affine_second_attributes_hbe_ptr header file.
 *
 * @ingroup sprite
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_sprite_ptr.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the attributes to commit to the second GBA register of an affine sprite
 * (with an attached sprite_affine_mat_ptr) in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup hblank_effect
 */
class sprite_affine_second_attributes_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a sprite_affine_second_attributes_hbe_ptr which changes the attributes to commit
     * to the second GBA register of an affine sprite in each screen horizontal line.
     * @param sprite Affine sprite (with an attached sprite_affine_mat_ptr) to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_second_attributes objects
     * with the attributes to commit to the second GBA register of the given affine sprite
     * in each screen horizontal line.
     *
     * The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
     * In other words, they are relative instead of absolute.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_second_attributes_hbe_ptr.
     */
    [[nodiscard]] static sprite_affine_second_attributes_hbe_ptr create(
            sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_second_attributes_hbe_ptr which changes the attributes to commit
     * to the second GBA register of an affine sprite in each screen horizontal line.
     * @param sprite Affine sprite (with an attached sprite_affine_mat_ptr) to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_second_attributes objects
     * with the attributes to commit to the second GBA register of the given affine sprite
     * in each screen horizontal line.
     *
     * The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
     * In other words, they are relative instead of absolute.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_second_attributes_hbe_ptr if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_second_attributes_hbe_ptr> create_optional(
            sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref);

    /**
     * @brief Returns the affine sprite modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    /**
     * @brief Returns the referenced array of 160 sprite_affine_second_attributes objects
     * with the attributes to commit to the second GBA register of the given affine sprite
     * in each screen horizontal line.
     *
     * The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
     * In other words, they are relative instead of absolute.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const sprite_affine_second_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 sprite_affine_second_attributes objects
     * with the attributes to commit to the second GBA register of the given affine sprite
     * in each screen horizontal line.
     *
     * The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
     * In other words, they are relative instead of absolute.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const sprite_affine_second_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to commit
     * to the second GBA register of the given affine sprite in each screen horizontal line.
     *
     * The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
     * In other words, they are relative instead of absolute.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this sprite_affine_second_attributes_hbe_ptr with those of the other one.
     * @param other sprite_affine_second_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(sprite_affine_second_attributes_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_affine_second_attributes_hbe_ptr with those of another one.
     * @param a First sprite_affine_second_attributes_hbe_ptr to exchange the contents with.
     * @param b Second sprite_affine_second_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_second_attributes_hbe_ptr& a, sprite_affine_second_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite;

    sprite_affine_second_attributes_hbe_ptr(int id, sprite_ptr&& sprite);
};

}

#endif
