/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_BG_PA_REGISTER_HBE_PTR_H
#define BN_PALETTE_BITMAP_BG_PA_REGISTER_HBE_PTR_H

/**
 * @file
 * bn::palette_bitmap_bg_pa_register_hbe_ptr header file.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup affine_mat
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes
 * the attributes or the values to commit to the first GBA register of a palette_bitmap_bg_ptr
 * in each screen horizontal line.
 *
 * @ingroup palette_bitmap_bg
 * @ingroup hblank_effect
 */
class palette_bitmap_bg_pa_register_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit
     * to the first GBA register of a palette_bitmap_bg_ptr.
     * @param bg palette_bitmap_bg_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_bg_mat_attributes objects
     * with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pa_register_hbe_ptr.
     */
    [[nodiscard]] static palette_bitmap_bg_pa_register_hbe_ptr create(
            palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr.
     * @param bg palette_bitmap_bg_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pa_register_hbe_ptr.
     */
    [[nodiscard]] static palette_bitmap_bg_pa_register_hbe_ptr create(
            palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit
     * to the first GBA register of a palette_bitmap_bg_ptr.
     * @param bg palette_bitmap_bg_ptr to be modified.
     * @param attributes_ref Reference to an array of 160 affine_bg_mat_attributes objects
     * with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<palette_bitmap_bg_pa_register_hbe_ptr> create_optional(
            palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref);

    /**
     * @brief Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr.
     * @param bg palette_bitmap_bg_ptr to be modified.
     * @param values_ref Reference to an array of 160 values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     *
     * @return The requested palette_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<palette_bitmap_bg_pa_register_hbe_ptr> create_optional(
            palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref);

    /**
     * @brief Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
     */
    [[nodiscard]] const palette_bitmap_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const affine_bg_mat_attributes> attributes_ref() const;

    /**
     * @brief Returns the referenced array of 160 values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const int16_t> values_ref() const;

    /**
     * @brief Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref);

    /**
     * @brief Sets the reference to an array of 160 values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    void set_values_ref(const span<const int16_t>& values_ref);

    /**
     * @brief Rereads the content of the referenced attributes to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Rereads the content of the referenced values to commit
     * to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
     *
     * The values are not copied but referenced, so they should outlive
     * the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
     */
    void reload_values_ref();

    /**
     * @brief Exchanges the contents of this palette_bitmap_bg_pa_register_hbe_ptr with those of the other one.
     * @param other palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
     */
    void swap(palette_bitmap_bg_pa_register_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a palette_bitmap_bg_pa_register_hbe_ptr with those of another one.
     * @param a First palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
     * @param b Second palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
     */
    friend void swap(palette_bitmap_bg_pa_register_hbe_ptr& a, palette_bitmap_bg_pa_register_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    bool _from_attributes;
    palette_bitmap_bg_ptr _bg;

    palette_bitmap_bg_pa_register_hbe_ptr(int id, bool from_attributes, palette_bitmap_bg_ptr&& bg);
};

}

#endif
