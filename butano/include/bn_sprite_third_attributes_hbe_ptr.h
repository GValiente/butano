/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_THIRD_ATTRIBUTES_HBE_PTR_H
#define BN_SPRITE_THIRD_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::sprite_third_attributes_hbe_ptr header file.
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
 * which changes the attributes to commit to the third GBA register of a sprite in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup hblank_effect
 */
class sprite_third_attributes_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a sprite_third_attributes_hbe_ptr which changes the attributes to commit
     * to the third GBA register of a sprite in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_third_attributes objects
     * with the attributes to commit to the third GBA register of the given sprite in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sprite_third_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_third_attributes_hbe_ptr.
     */
    [[nodiscard]] static sprite_third_attributes_hbe_ptr create(
            sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_third_attributes_hbe_ptr which changes the attributes to commit
     * to the third GBA register of a sprite in each screen horizontal line.
     * @param sprite Sprite to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_third_attributes objects
     * with the attributes to commit to the third GBA register of the given sprite in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sprite_third_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_third_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_third_attributes_hbe_ptr> create_optional(
            sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref);

    /**
     * @brief Returns the sprite modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    /**
     * @brief Returns the referenced array of 160 sprite_third_attributes objects
     * with the attributes to commit to the third GBA register of the managed sprite in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sprite_third_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const sprite_third_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 sprite_third_attributes objects
     * with the attributes to commit to the third GBA register of the managed sprite in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sprite_third_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const sprite_third_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to commit to the third GBA register
     * of the managed sprite in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sprite_third_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this sprite_third_attributes_hbe_ptr with those of the other one.
     * @param other sprite_third_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(sprite_third_attributes_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_third_attributes_hbe_ptr with those of another one.
     * @param a First sprite_third_attributes_hbe_ptr to exchange the contents with.
     * @param b Second sprite_third_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(sprite_third_attributes_hbe_ptr& a, sprite_third_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite;

    sprite_third_attributes_hbe_ptr(int id, sprite_ptr&& sprite);
};

}

#endif
