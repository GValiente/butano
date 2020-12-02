/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HBE_ACTIONS_H
#define BN_HBE_ACTIONS_H

/**
 * @file
 * bn::hbe_ptr actions header file.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */

#include "bn_hbe_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if a hbe_ptr must be committed to the GBA or not.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */
class hbe_visible_manager
{

public:
    /**
     * @brief Indicates if the given hbe_ptr is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(const hbe_ptr& hbe)
    {
        return hbe.visible();
    }

    /**
     * @brief Sets if the given hbe_ptr must be committed to the GBA or not.
     */
    static void set(bool visible, hbe_ptr& hbe)
    {
        hbe.set_visible(visible);
    }
};


/**
 * @brief Toggles if a hbe_ptr must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */
class hbe_visible_toggle_action : public bool_toggle_value_template_action<hbe_ptr, hbe_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param hbe hbe_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given hbe_ptr must be committed to the GBA or not.
     */
    hbe_visible_toggle_action(const hbe_ptr& hbe, int duration_updates) :
        bool_toggle_value_template_action(hbe, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param hbe hbe_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given hbe_ptr must be committed to the GBA or not.
     */
    hbe_visible_toggle_action(hbe_ptr&& hbe, int duration_updates) :
        bool_toggle_value_template_action(move(hbe), duration_updates)
    {
    }

    /**
     * @brief Returns the hbe_ptr to modify.
     */
    [[nodiscard]] const hbe_ptr& hbe() const
    {
        return value();
    }
};

}

#endif
