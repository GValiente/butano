/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HBLANK_EFFECT_ACTIONS_H
#define BN_HBLANK_EFFECT_ACTIONS_H

/**
 * @file
 * bn::hblank_effect_ptr actions header file.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */

#include "bn_hblank_effect_ptr.h"
#include "bn_value_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if a hblank_effect_ptr must be committed to the GBA or not.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */
class hblank_effect_visible_manager
{

public:
    /**
     * @brief Indicates if the given hblank_effect_ptr is committed to the GBA or not.
     */
    [[nodiscard]] static bool get(const hblank_effect_ptr& hblank_effect)
    {
        return hblank_effect.visible();
    }

    /**
     * @brief Sets if the given hblank_effect_ptr must be committed to the GBA or not.
     */
    static void set(bool visible, hblank_effect_ptr& hblank_effect)
    {
        hblank_effect.set_visible(visible);
    }
};


/**
 * @brief Toggles if a hblank_effect_ptr must be committed to the GBA or not
 * when the action is updated a given number of times.
 *
 * @ingroup hblank_effect
 * @ingroup action
 */
class hblank_effect_visible_toggle_action :
        public bool_toggle_value_template_action<hblank_effect_ptr, hblank_effect_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param hblank_effect hblank_effect_ptr to copy.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given hblank_effect_ptr must be committed to the GBA or not.
     */
    hblank_effect_visible_toggle_action(const hblank_effect_ptr& hblank_effect, int duration_updates) :
        bool_toggle_value_template_action(hblank_effect, duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param hblank_effect hblank_effect_ptr to move.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the given hblank_effect_ptr must be committed to the GBA or not.
     */
    hblank_effect_visible_toggle_action(hblank_effect_ptr&& hblank_effect, int duration_updates) :
        bool_toggle_value_template_action(move(hblank_effect), duration_updates)
    {
    }

    /**
     * @brief Returns the hblank_effect_ptr to modify.
     */
    [[nodiscard]] const hblank_effect_ptr& hblank_effect() const
    {
        return value();
    }
};

}

#endif
