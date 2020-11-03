/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_MOSAIC_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class mosaic_attributes;

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes the attributes
 * to commit to the GBA mosaic register in each screen horizontal line.
 *
 * @ingroup mosaic
 * @ingroup hblank_effect
 */
class mosaic_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a mosaic_attributes_hblank_effect_ptr which changes the attributes to commit
     * to the GBA mosaic register in each screen horizontal line.
     * @param attributes_ref Reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested mosaic_attributes_hblank_effect_ptr.
     */
    [[nodiscard]] static mosaic_attributes_hblank_effect_ptr create(
            const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Creates a mosaic_attributes_hblank_effect_ptr which changes the attributes to commit
     * to the GBA mosaic register in each screen horizontal line.
     * @param attributes_ref Reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested mosaic_attributes_hblank_effect_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<mosaic_attributes_hblank_effect_ptr> create_optional(
            const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Returns the reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const mosaic_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 mosaic_attributes objects
     * with the attributes to commit to the GBA mosaic register in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const mosaic_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to the GBA mosaic register
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the mosaic_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this mosaic_attributes_hblank_effect_ptr with those of the other one.
     * @param other mosaic_attributes_hblank_effect_ptr to exchange the contents with.
     */
    void swap(mosaic_attributes_hblank_effect_ptr& other)
    {
        hblank_effect_ptr::swap(other);
    }

    /**
     * @brief Exchanges the contents of a mosaic_attributes_hblank_effect_ptr with those of another one.
     * @param a First mosaic_attributes_hblank_effect_ptr to exchange the contents with.
     * @param b Second mosaic_attributes_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(mosaic_attributes_hblank_effect_ptr& a, mosaic_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    mosaic_attributes_hblank_effect_ptr(int id);
};

}

#endif
