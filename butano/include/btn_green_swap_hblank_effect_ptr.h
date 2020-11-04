/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GREEN_SWAP_HBLANK_EFFECT_PTR_H
#define BTN_GREEN_SWAP_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

/**
 * @brief Smart pointer that retains shared ownership of a H-Blank effect which changes the state of green swap
 * in each screen horizontal line.
 *
 * @ingroup green_swap
 * @ingroup hblank_effect
 */
class green_swap_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    /**
     * @brief Creates a green_swap_hblank_effect_ptr which changes the state of green swap
     * in each screen horizontal line.
     * @param states_ref Reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested green_swap_hblank_effect_ptr.
     */
    [[nodiscard]] static green_swap_hblank_effect_ptr create(const span<const bool>& states_ref);

    /**
     * @brief Creates a green_swap_hblank_effect_ptr which changes the state of green swap
     * in each screen horizontal line.
     * @param states_ref Reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hblank_effect_ptr to avoid dangling references.
     *
     * @return The requested green_swap_hblank_effect_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<green_swap_hblank_effect_ptr> create_optional(const span<const bool>& states_ref);

    /**
     * @brief Returns the reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hblank_effect_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const bool> states_ref() const;

    /**
     * @brief Sets the reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hblank_effect_ptr to avoid dangling references.
     */
    void set_states_ref(const span<const bool>& states_ref);

    /**
     * @brief Rereads the content of the referenced green swap states in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hblank_effect_ptr to avoid dangling references.
     */
    void reload_states_ref();

    /**
     * @brief Exchanges the contents of this green_swap_hblank_effect_ptr with those of the other one.
     * @param other green_swap_hblank_effect_ptr to exchange the contents with.
     */
    void swap(green_swap_hblank_effect_ptr& other);

    /**
     * @brief Exchanges the contents of a green_swap_hblank_effect_ptr with those of another one.
     * @param a First green_swap_hblank_effect_ptr to exchange the contents with.
     * @param b Second green_swap_hblank_effect_ptr to exchange the contents with.
     */
    friend void swap(green_swap_hblank_effect_ptr& a, green_swap_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    green_swap_hblank_effect_ptr(int id);
};

}

#endif
