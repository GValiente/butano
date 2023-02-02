/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MOSAIC_ATTRIBUTES_HBE_PTR_H
#define BN_MOSAIC_ATTRIBUTES_HBE_PTR_H

/**
 * @file
 * bn::mosaic_attributes_hbe_ptr header file.
 *
 * @ingroup mosaic
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_hbe_ptr.h"

namespace bn
{

class mosaic_attributes;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the attributes to commit to the GBA mosaic register in each screen horizontal line.
 *
 * @ingroup mosaic
 * @ingroup hblank_effect
 */
class mosaic_attributes_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a mosaic_attributes_hbe_ptr which changes the attributes to commit
     * to the GBA mosaic register in each screen horizontal line.
     * @param attributes_ref Reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested mosaic_attributes_hbe_ptr.
     */
    [[nodiscard]] static mosaic_attributes_hbe_ptr create(
            const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Creates a mosaic_attributes_hbe_ptr which changes the attributes to commit
     * to the GBA mosaic register in each screen horizontal line.
     * @param attributes_ref Reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hbe_ptr to avoid dangling references.
     *
     * @return The requested mosaic_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<mosaic_attributes_hbe_ptr> create_optional(
            const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Returns the referenced array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const mosaic_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to the GBA mosaic register
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this mosaic_attributes_hbe_ptr with those of the other one.
     * @param other mosaic_attributes_hbe_ptr to exchange the contents with.
     */
    void swap(mosaic_attributes_hbe_ptr& other)
    {
        hbe_ptr::swap(other);
    }

    /**
     * @brief Exchanges the contents of a mosaic_attributes_hbe_ptr with those of another one.
     * @param a First mosaic_attributes_hbe_ptr to exchange the contents with.
     * @param b Second mosaic_attributes_hbe_ptr to exchange the contents with.
     */
    friend void swap(mosaic_attributes_hbe_ptr& a, mosaic_attributes_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    mosaic_attributes_hbe_ptr(int id);
};

}

#endif
