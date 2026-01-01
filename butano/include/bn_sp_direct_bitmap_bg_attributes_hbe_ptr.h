/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SP_DIRECT_BITMAP_BG_ATTRIBUTES_HBE_PTR_H
#define BN_SP_DIRECT_BITMAP_BG_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::sp_direct_bitmap_bg_attributes_hbe_ptr header file.
 *
 * @ingroup sp_direct_bitmap_bg
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the attributes to commit to the GBA register of a single page direct bitmap background
 * in each screen horizontal line.
 *
 * @ingroup sp_direct_bitmap_bg
 * @ingroup hblank_effect
 */
class sp_direct_bitmap_bg_attributes_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a sp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit
     * to the GBA register of a single page direct bitmap background in each screen horizontal line.
     * @param bg Background to be modified.
     * @param attributes_ref Reference to an array of 160 bitmap_bg_attributes objects
     * with the attributes to commit to the GBA register of the given background in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sp_direct_bitmap_bg_attributes_hbe_ptr.
     */
    [[nodiscard]] static sp_direct_bitmap_bg_attributes_hbe_ptr create(
            sp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref);

    /**
     * @brief Creates a sp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit
     * to the GBA register of a single page direct bitmap background in each screen horizontal line.
     * @param bg Background to be modified.
     * @param attributes_ref Reference to an array of 160 bitmap_bg_attributes objects
     * with the attributes to commit to the GBA register of the given background in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested sp_direct_bitmap_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sp_direct_bitmap_bg_attributes_hbe_ptr> create_optional(
            sp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref);

    /**
     * @brief Returns the affine background modified by this H-Blank effect.
     */
    [[nodiscard]] const sp_direct_bitmap_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns the referenced array of 160 bitmap_bg_attributes objects
     * with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const bitmap_bg_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 bitmap_bg_attributes objects
     * with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const bitmap_bg_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to the GBA register of the managed background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this sp_direct_bitmap_bg_attributes_hbe_ptr with those of the other one.
     * @param other sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(sp_direct_bitmap_bg_attributes_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a sp_direct_bitmap_bg_attributes_hbe_ptr with those of another one.
     * @param a First sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
     * @param b Second sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(sp_direct_bitmap_bg_attributes_hbe_ptr& a, sp_direct_bitmap_bg_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    sp_direct_bitmap_bg_ptr _bg;

    sp_direct_bitmap_bg_attributes_hbe_ptr(int id, sp_direct_bitmap_bg_ptr&& bg);
};

}

#endif
