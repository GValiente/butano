/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BACKDROP_ACTIONS_H
#define BN_BACKDROP_ACTIONS_H

/**
 * @file
 * bn::backdrop actions header file.
 *
 * @ingroup backdrop
 * @ingroup action
 */

#include "bn_backdrop.h"
#include "bn_template_actions.h"

namespace bn
{

/**
 * @brief Manages if the backdrop must be part of the blending bottom layer or not.
 *
 * @ingroup backdrop
 * @ingroup action
 */
class backdrop_blending_bottom_enabled_manager
{

public:
    /**
     * @brief Indicates if the backdrop is part of the blending bottom layer or not.
     */
    [[nodiscard]] static bool get()
    {
        return backdrop::blending_bottom_enabled();
    }

    /**
     * @brief Sets if the backdrop must be part of the blending bottom layer or not.
     */
    static void set(bool enabled)
    {
        backdrop::set_blending_bottom_enabled(enabled);
    }
};


/**
 * @brief Toggles if the backdrop is part of the blending bottom layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup backdrop
 * @ingroup action
 */
class backdrop_blending_bottom_enabled_toggle_action :
        public bool_toggle_template_action<backdrop_blending_bottom_enabled_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to toggle
     * if the backdrop is part of the blending bottom layer or not.
     */
    explicit backdrop_blending_bottom_enabled_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};

}

#endif
