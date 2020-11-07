/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_REGULAR_BG_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_REGULAR_BG_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_regular_bg_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes the attributes
 * to commit to the GBA register of a regular background in each screen horizontal line.
 *
 * @ingroup regular_bg
 * @ingroup hblank_effect
 */
class regular_bg_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a regular_bg_attributes_hblank_effect_ptr which changes the attributes to commit
     * to the GBA register of a regular background in each screen horizontal line.
     * @param bg Regular background to be modified.
     * @param attributes_ref Reference to an array of 160 regular_bg_attributes objects
     * with the attributes to commit to the GBA register of the given regular background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the regular_bg_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested regular_bg_attributes_hblank_effect_ptr.
     */
    [[nodiscard]] static regular_bg_attributes_hblank_effect_ptr create(
            regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref);

    /**
     * @brief Creates a regular_bg_attributes_hblank_effect_ptr which changes the attributes to commit
     * to the GBA register of a regular background in each screen horizontal line.
     * @param bg Regular background to be modified.
     * @param attributes_ref Reference to an array of 160 regular_bg_attributes objects
     * with the attributes to commit to the GBA register of the given regular background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the regular_bg_attributes_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested regular_bg_attributes_hblank_effect_ptr if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<regular_bg_attributes_hblank_effect_ptr> create_optional(
            regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref);

    /**
     * @brief Returns the regular background modified by this H-Blank effect.
     */
    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns the referenced array of 160 regular_bg_attributes objects
     * with the attributes to commit to the GBA register of the managed regular background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the regular_bg_attributes_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const regular_bg_attributes> attributes_ref() const;

    /**
     * @brief Sets the reference to an array of 160 regular_bg_attributes objects
     * with the attributes to commit to the GBA register of the managed regular background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the regular_bg_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void set_attributes_ref(const span<const regular_bg_attributes>& attributes_ref);

    /**
     * @brief Rereads the content of the referenced attributes to the GBA register of the managed regular background
     * in each screen horizontal line.
     *
     * The attributes are not copied but referenced, so they should outlive
     * the regular_bg_attributes_hblank_effect_ptr to avoid dangling references.
     */
    void reload_attributes_ref();

    /**
     * @brief Exchanges the contents of this regular_bg_attributes_hblank_effect_ptr with those of the other one.
     * @param other regular_bg_attributes_hblank_effect_ptr to exchange the contents with.
     */
    void swap(regular_bg_attributes_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a regular_bg_attributes_hblank_effect_ptr with those of another one.
     * @param a First regular_bg_attributes_hblank_effect_ptr to exchange the contents with.
     * @param b Second regular_bg_attributes_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(regular_bg_attributes_hblank_effect_ptr& a, regular_bg_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    regular_bg_ptr _bg;

    regular_bg_attributes_hblank_effect_ptr(int id, regular_bg_ptr&& bg);
};

}

#endif
