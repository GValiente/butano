/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GREEN_SWAP_ACTIONS_H
#define BN_GREEN_SWAP_ACTIONS_H

/**
 * @file
 * bn::green_swap actions header file.
 *
 * @ingroup green_swap
 * @ingroup action
 */

#include "bn_green_swap.h"
#include "bn_template_actions.h"

namespace bn
{

/**
 * @brief Manages if green swap must be enabled or not.
 *
 * @ingroup green_swap
 * @ingroup action
 */
class green_swap_manager
{

public:
    /**
     * @brief Indicates if green swap is enabled or not.
     */
    [[nodiscard]] static bool get()
    {
        return green_swap::enabled();
    }

    /**
     * @brief Sets if green swap must be enabled or not.
     */
    static void set(bool enabled)
    {
        green_swap::set_enabled(enabled);
    }
};


/**
 * @brief Toggles if green swap must be enabled or not when the action is updated a given number of times.
 *
 * @ingroup green_swap
 * @ingroup action
 */
class green_swap_toggle_action : public bool_toggle_template_action<green_swap_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to toggle
     * if green swap must be inverted or not.
     */
    explicit green_swap_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};

}

#endif
