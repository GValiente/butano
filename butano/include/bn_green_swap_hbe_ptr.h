/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GREEN_SWAP_HBE_PTR_H
#define BN_GREEN_SWAP_HBE_PTR_H

/**
 * @file
 * bn::green_swap_hbe_ptr header file.
 *
 * @ingroup green_swap
 * @ingroup hblank_effect
 */

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_hbe_ptr.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect
 * which changes the state of green swap in each screen horizontal line.
 *
 * @ingroup green_swap
 * @ingroup hblank_effect
 */
class green_swap_hbe_ptr : public hbe_ptr
{

public:
    /**
     * @brief Creates a green_swap_hbe_ptr which changes the state of green swap
     * in each screen horizontal line.
     * @param states_ref Reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hbe_ptr to avoid dangling references.
     *
     * @return The requested green_swap_hbe_ptr.
     */
    [[nodiscard]] static green_swap_hbe_ptr create(const span<const bool>& states_ref);

    /**
     * @brief Creates a green_swap_hbe_ptr which changes the state of green swap
     * in each screen horizontal line.
     * @param states_ref Reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hbe_ptr to avoid dangling references.
     *
     * @return The requested green_swap_hbe_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<green_swap_hbe_ptr> create_optional(const span<const bool>& states_ref);

    /**
     * @brief Returns the referenced array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hbe_ptr to avoid dangling references.
     */
    [[nodiscard]] span<const bool> states_ref() const;

    /**
     * @brief Sets the reference to an array of 160 bool values which indicate the state of green swap
     * in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hbe_ptr to avoid dangling references.
     */
    void set_states_ref(const span<const bool>& states_ref);

    /**
     * @brief Rereads the content of the referenced green swap states in each screen horizontal line.
     *
     * The states are not copied but referenced, so they should outlive
     * the green_swap_hbe_ptr to avoid dangling references.
     */
    void reload_states_ref();

    /**
     * @brief Exchanges the contents of this green_swap_hbe_ptr with those of the other one.
     * @param other green_swap_hbe_ptr to exchange the contents with.
     */
    void swap(green_swap_hbe_ptr& other);

    /**
     * @brief Exchanges the contents of a green_swap_hbe_ptr with those of another one.
     * @param a First green_swap_hbe_ptr to exchange the contents with.
     * @param b Second green_swap_hbe_ptr to exchange the contents with.
     */
    friend void swap(green_swap_hbe_ptr& a, green_swap_hbe_ptr& b)
    {
        a.swap(b);
    }

private:
    green_swap_hbe_ptr(int id);
};

}

#endif
