/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RUMBLE_ACTIONS_H
#define BN_RUMBLE_ACTIONS_H

/**
 * @file
 * bn::rumble actions header file.
 *
 * @ingroup rumble
 * @ingroup action
 */

#include "bn_rumble.h"
#include "bn_template_actions.h"

namespace bn
{

/**
 * @brief Manages if rumble must be enabled or not.
 *
 * @ingroup rumble
 * @ingroup action
 */
class rumble_manager
{

public:
    /**
     * @brief Indicates if rumble is enabled or not.
     */
    [[nodiscard]] static bool get()
    {
        return rumble::enabled();
    }

    /**
     * @brief Sets if rumble must be enabled or not.
     */
    static void set(bool enabled)
    {
        rumble::set_enabled(enabled);
    }
};


/**
 * @brief Toggles if rumble must be enabled or not when the action is updated a given number of times.
 *
 * @ingroup rumble
 * @ingroup action
 */
class rumble_toggle_action : public bool_toggle_template_action<rumble_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to toggle
     * if rumble must be inverted or not.
     */
    explicit rumble_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};

}

#endif
