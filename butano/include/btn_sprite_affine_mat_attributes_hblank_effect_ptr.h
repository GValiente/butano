/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_sprite_affine_mat_pa_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pb_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pc_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pd_register_hblank_effect_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */
class sprite_affine_mat_attributes_hblank_effect_ptr
{

public:
    /**
     * @brief Creates a sprite_affine_mat_attributes_hblank_effect_ptr which changes
     * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_attributes_hblank_effect_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_attributes_hblank_effect_ptr create(
            const sprite_affine_mat_ptr& affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a sprite_affine_mat_attributes_hblank_effect_ptr which changes
     * the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
     * @param affine_mat sprite_affine_mat_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 sprite_affine_mat_attributes objects
     * with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested sprite_affine_mat_attributes_hblank_effect_ptr if it can be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_attributes_hblank_effect_ptr> create_optional(
            const sprite_affine_mat_ptr& affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _pa_hblank_effect_ptr.id();
    }

    /**
     * @brief Indicates if this H-Blank effect must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const
    {
        return _pa_hblank_effect_ptr.visible();
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
        return _pa_hblank_effect_ptr.affine_mat();
    }

    /**
     * @brief Returns the reference to an array of 160 sprite_affine_mat_attributes objects
     * to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const sprite_affine_mat_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 sprite_affine_mat_attributes objects
     * to set to the given sprite_affine_mat_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const sprite_affine_mat_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to set to the given sprite_affine_mat_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * sprite_affine_mat_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this sprite_affine_mat_attributes_hblank_effect_ptr
     * with those of the other one.
     * @param other sprite_affine_mat_attributes_hblank_effect_ptr to exchange the contents with.
     */
    void swap(sprite_affine_mat_attributes_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a sprite_affine_mat_attributes_hblank_effect_ptr
     * with those of another one.
     * @param a First sprite_affine_mat_attributes_hblank_effect_ptr to exchange the contents with.
     * @param b Second sprite_affine_mat_attributes_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_mat_attributes_hblank_effect_ptr& a,
                     sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First sprite_affine_mat_attributes_hblank_effect_ptr to compare.
     * @param b Second sprite_affine_mat_attributes_hblank_effect_ptr to compare.
     * @return <b>true</b> if the first sprite_affine_mat_attributes_hblank_effect_ptr is equal to the second one,
     * otherwise <b>false</b>.
     */
    [[nodiscard]] friend bool operator==(const sprite_affine_mat_attributes_hblank_effect_ptr& a,
                                         const sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        return a._pa_hblank_effect_ptr == b._pa_hblank_effect_ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a First sprite_affine_mat_attributes_hblank_effect_ptr to compare.
     * @param b Second sprite_affine_mat_attributes_hblank_effect_ptr to compare.
     * @return <b>true</b> if the first sprite_affine_mat_attributes_hblank_effect_ptr is not equal to the second one,
     * otherwise <b>false</b>.
     */
    [[nodiscard]] friend bool operator!=(const sprite_affine_mat_attributes_hblank_effect_ptr& a,
                                         const sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        return ! (a == b);
    }

private:
    sprite_affine_mat_pa_register_hblank_effect_ptr _pa_hblank_effect_ptr;
    sprite_affine_mat_pb_register_hblank_effect_ptr _pb_hblank_effect_ptr;
    sprite_affine_mat_pc_register_hblank_effect_ptr _pc_hblank_effect_ptr;
    sprite_affine_mat_pd_register_hblank_effect_ptr _pd_hblank_effect_ptr;

    sprite_affine_mat_attributes_hblank_effect_ptr(
            sprite_affine_mat_pa_register_hblank_effect_ptr&& pa_hblank_effect_ptr,
            sprite_affine_mat_pb_register_hblank_effect_ptr&& pb_hblank_effect_ptr,
            sprite_affine_mat_pc_register_hblank_effect_ptr&& pc_hblank_effect_ptr,
            sprite_affine_mat_pd_register_hblank_effect_ptr&& pd_hblank_effect_ptr);
};


/**
 * @brief Hash support for sprite_affine_mat_attributes_hblank_effect_ptr.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */
template<>
struct hash<sprite_affine_mat_attributes_hblank_effect_ptr>
{
    /**
     * @brief Returns the hash of the given sprite_affine_mat_attributes_hblank_effect_ptr.
     */
    [[nodiscard]] unsigned operator()(const sprite_affine_mat_attributes_hblank_effect_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
