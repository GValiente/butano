/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_PTR_H
#define BTN_BLENDING_FADE_ALPHA_HBLANK_EFFECT_PTR_H

/**
 * @file
 * btn::blending_fade_alpha_hblank_effect_ptr header file.
 *
 * @ingroup blending
 * @ingroup hblank_effect
 */

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class blending_fade_alpha;

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes
 * the weight of the fade blending in each screen horizontal line.
 *
 * Keep in mind that fade blending and other blendings can't be enabled at the same time.
 *
 * @ingroup blending
 * @ingroup hblank_effect
 */
class blending_fade_alpha_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a blending_fade_alpha_hblank_effect_ptr which changes the weight of the fade blending
     * in each screen horizontal line.
     * @param alphas_ref Reference to an array of 160 blending_fade_alpha objects
     * which indicate the weight of the fade blending in each screen horizontal line.
     *
     * The alphas are not copied but referenced, so they should outlive
     * the blending_fade_alpha_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested blending_fade_alpha_hblank_effect_ptr.
     */
    [[nodiscard]] static blending_fade_alpha_hblank_effect_ptr create(
            const span<const blending_fade_alpha>& alphas_ref);

    /**
     * @brief Creates a blending_fade_alpha_hblank_effect_ptr which changes the weight of the fade blending
     * in each screen horizontal line.
     * @param alphas_ref Reference to an array of 160 blending_fade_alpha objects
     * which indicate the weight of the fade blending in each screen horizontal line.
     *
     * The alphas are not copied but referenced, so they should outlive
     * the blending_fade_alpha_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested blending_fade_alpha_hblank_effect_ptr if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] static optional<blending_fade_alpha_hblank_effect_ptr> create_optional(
            const span<const blending_fade_alpha>& alphas_ref);

    /**
     * @brief Returns the referenced array of 160 blending_fade_alpha objects
     * which indicate the weight of the fade blending in each screen horizontal line.
     *
     * The alphas are not copied but referenced, so they should outlive
     * the blending_fade_alpha_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const blending_fade_alpha> alphas_ref() const;

    /**
     * @brief Sets the reference to an array of 160 blending_fade_alpha objects
     * which indicate the weight of the fade blending in each screen horizontal line.
     *
     * The alphas are not copied but referenced, so they should outlive
     * the blending_fade_alpha_hblank_effect_ptr to avoid dangling references.
     */
    void set_alphas_ref(const span<const blending_fade_alpha>& alphas_ref);

    /**
     * @brief Rereads the content of the referenced blending_fade_alpha objects
     * which indicate the weight of the fade blending in each screen horizontal line.
     *
     * The alphas are not copied but referenced, so they should outlive
     * the blending_fade_alpha_hblank_effect_ptr to avoid dangling references.
     */
    void reload_alphas_ref();

    /**
     * @brief Exchanges the contents of this blending_fade_alpha_hblank_effect_ptr with those of the other one.
     * @param other blending_fade_alpha_hblank_effect_ptr to exchange the contents with.
     */
    void swap(blending_fade_alpha_hblank_effect_ptr& other)
    {
        hblank_effect_ptr::swap(other);
    }

    /**
     * @brief Exchanges the contents of a blending_fade_alpha_hblank_effect_ptr with those of another one.
     * @param a First blending_fade_alpha_hblank_effect_ptr to exchange the contents with.
     * @param b Second blending_fade_alpha_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(blending_fade_alpha_hblank_effect_ptr& a, blending_fade_alpha_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    blending_fade_alpha_hblank_effect_ptr(int id);
};

}

#endif
