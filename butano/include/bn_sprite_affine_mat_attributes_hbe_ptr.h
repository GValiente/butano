/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBE_PTR_H
#define BN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::sprite_affine_mat_attributes_hbe_ptr header file.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_sprite_affine_mat_pa_register_hbe_ptr.h"
#include "bn_sprite_affine_mat_pb_register_hbe_ptr.h"
#include "bn_sprite_affine_mat_pc_register_hbe_ptr.h"
#include "bn_sprite_affine_mat_pd_register_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */
class sprite_affine_mat_attributes_hbe_ptr
{

public:
    /**
     * @brief Creates a sprite_affine_mat_attributes_hbe_ptr which changes
     * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_mat_attributes objects
     * with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_attributes_hbe_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_attributes_hbe_ptr create(
            const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_attributes_hbe_ptr which changes
     * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_mat_attributes objects
     * with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_attributes_hbe_ptr if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_attributes_hbe_ptr> create_optional(
            const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _pa_hbe_ptr.id();
    }

    /**
     * @brief Indicates if this H-Blank effect must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const
    {
        return _pa_hbe_ptr.visible();
    }

    /**
     * @brief Sets if this H-Blank effect must be committed to the GBA or not.
     */
    void set_visible(bool visible);

    /**
     * @brief Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
     */
    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _pa_hbe_ptr.affine_mat();
    }

    /**
     * @brief Returns the referenced array of 160 affine_mat_attributes objects
     * to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const affine_mat_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 affine_mat_attributes objects
     * to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to set to the given sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this sprite_affine_mat_attributes_hbe_ptr with those of the other one.
     * @param other sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(sprite_affine_mat_attributes_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_affine_mat_attributes_hbe_ptr with those of another one.
     * @param a First sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.
     * @param b Second sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_mat_attributes_hbe_ptr& a, sprite_affine_mat_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First sprite_affine_mat_attributes_hbe_ptr to compare.
     * @param b Second sprite_affine_mat_attributes_hbe_ptr to compare.
     * @return `true` if the first sprite_affine_mat_attributes_hbe_ptr is equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const sprite_affine_mat_attributes_hbe_ptr& a,
                                         const sprite_affine_mat_attributes_hbe_ptr& b)
    {
        return a._pa_hbe_ptr == b._pa_hbe_ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a First sprite_affine_mat_attributes_hbe_ptr to compare.
     * @param b Second sprite_affine_mat_attributes_hbe_ptr to compare.
     * @return `true` if the first sprite_affine_mat_attributes_hbe_ptr is not equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const sprite_affine_mat_attributes_hbe_ptr& a,
                                         const sprite_affine_mat_attributes_hbe_ptr& b)
    {
        return a._pa_hbe_ptr != b._pa_hbe_ptr;
    }

private:
    sprite_affine_mat_pa_register_hbe_ptr _pa_hbe_ptr;
    sprite_affine_mat_pb_register_hbe_ptr _pb_hbe_ptr;
    sprite_affine_mat_pc_register_hbe_ptr _pc_hbe_ptr;
    sprite_affine_mat_pd_register_hbe_ptr _pd_hbe_ptr;

    sprite_affine_mat_attributes_hbe_ptr(
            sprite_affine_mat_pa_register_hbe_ptr&& pa_hbe_ptr,
            sprite_affine_mat_pb_register_hbe_ptr&& pb_hbe_ptr,
            sprite_affine_mat_pc_register_hbe_ptr&& pc_hbe_ptr,
            sprite_affine_mat_pd_register_hbe_ptr&& pd_hbe_ptr);
};


/**
 * @brief Hash support for sprite_affine_mat_attributes_hbe_ptr.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 * @ingroup functional
 */
template<>
struct hash<sprite_affine_mat_attributes_hbe_ptr>
{
    /**
     * @brief Returns the hash of the given sprite_affine_mat_attributes_hbe_ptr.
     */
    [[nodiscard]] unsigned operator()(const sprite_affine_mat_attributes_hbe_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
