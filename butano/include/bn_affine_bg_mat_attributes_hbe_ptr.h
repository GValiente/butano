/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_MAT_ATTRIBUTES_HBE_PTR_H
#define BN_AFFINE_BG_MAT_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::affine_bg_mat_attributes_hbe_ptr header file.
 *
 * @ingroup affine_bg
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */

#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_pb_register_hbe_ptr.h"
#include "bn_affine_bg_pc_register_hbe_ptr.h"
#include "bn_affine_bg_pd_register_hbe_ptr.h"
#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes which define the position and the transformation matrix of an affine_bg_ptr
 * in each screen horizontal line.
 *
 * @ingroup affine_bg
 * @ingroup hblank_effect
 */
class affine_bg_mat_attributes_hbe_ptr
{

public:
    /**
     * @brief Creates an affine_bg_mat_attributes_hbe_ptr which changes
     * the attributes which define the position and the transformation matrix of an affine_bg_ptr
     * in each screen horizontal line.
     * @param bg affine_bg_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_bg_mat_attributes objects
     * with the attributes to set to the given affine_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested affine_bg_mat_attributes_hbe_ptr.
     */
    [[nodiscard]] static affine_bg_mat_attributes_hbe_ptr create(
            const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref);

    /**
     * @brief Creates an affine_bg_mat_attributes_hbe_ptr which changes
     * the attributes which define the position and the transformation matrix of an affine_bg_ptr
     * in each screen horizontal line.
     * @param bg affine_bg_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_bg_mat_attributes objects
     * with the attributes to set to the given affine_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested affine_bg_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_mat_attributes_hbe_ptr> create_optional(
            const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref);

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
     * @brief Returns the affine_bg_ptr modified by this H-Blank effect.
     */
    [[nodiscard]] const affine_bg_ptr& bg() const
    {
        return _pa_hbe_ptr.bg();
    }

    /**
     * @brief Returns the referenced array of 160 affine_bg_mat_attributes objects
     * to set to the given affine_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const affine_bg_mat_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 affine_bg_mat_attributes objects
     * to set to the given affine_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to set to the given affine_bg_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this affine_bg_mat_attributes_hbe_ptr with those of the other one.
     * @param other affine_bg_mat_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(affine_bg_mat_attributes_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of an affine_bg_mat_attributes_hbe_ptr with those of another one.
     * @param a First affine_bg_mat_attributes_hbe_ptr to exchange the contents with.
     * @param b Second affine_bg_mat_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(affine_bg_mat_attributes_hbe_ptr& a, affine_bg_mat_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First affine_bg_mat_attributes_hbe_ptr to compare.
     * @param b Second affine_bg_mat_attributes_hbe_ptr to compare.
     * @return `true` if the first affine_bg_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const affine_bg_mat_attributes_hbe_ptr& a,
                                         const affine_bg_mat_attributes_hbe_ptr& b)
    {
        return a._pa_hbe_ptr == b._pa_hbe_ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a First affine_bg_mat_attributes_hbe_ptr to compare.
     * @param b Second affine_bg_mat_attributes_hbe_ptr to compare.
     * @return `true` if the first affine_bg_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const affine_bg_mat_attributes_hbe_ptr& a,
                                         const affine_bg_mat_attributes_hbe_ptr& b)
    {
        return a._pa_hbe_ptr != b._pa_hbe_ptr;
    }

private:
    affine_bg_pa_register_hbe_ptr _pa_hbe_ptr;
    affine_bg_pb_register_hbe_ptr _pb_hbe_ptr;
    affine_bg_pc_register_hbe_ptr _pc_hbe_ptr;
    affine_bg_pd_register_hbe_ptr _pd_hbe_ptr;
    affine_bg_dx_register_hbe_ptr _dx_hbe_ptr;
    affine_bg_dy_register_hbe_ptr _dy_hbe_ptr;

    affine_bg_mat_attributes_hbe_ptr(
            affine_bg_pa_register_hbe_ptr&& pa_hbe_ptr,
            affine_bg_pb_register_hbe_ptr&& pb_hbe_ptr,
            affine_bg_pc_register_hbe_ptr&& pc_hbe_ptr,
            affine_bg_pd_register_hbe_ptr&& pd_hbe_ptr,
            affine_bg_dx_register_hbe_ptr&& dx_hbe_ptr,
            affine_bg_dy_register_hbe_ptr&& dy_hbe_ptr);
};


/**
 * @brief Hash support for affine_bg_mat_attributes_hbe_ptr.
 *
 * @ingroup affine_bg
 * @ingroup affine_mat
 * @ingroup hblank_effect
 * @ingroup functional
 */
template<>
struct hash<affine_bg_mat_attributes_hbe_ptr>
{
    /**
     * @brief Returns the hash of the given affine_bg_mat_attributes_hbe_ptr.
     */
    [[nodiscard]] unsigned operator()(const affine_bg_mat_attributes_hbe_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
