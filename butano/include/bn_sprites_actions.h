/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_ACTIONS_H
#define BN_SPRITES_ACTIONS_H

/**
 * @file
 * bn::sprites actions header file.
 *
 * @ingroup sprite
 * @ingroup action
 */

#include "bn_sprites.h"
#include "bn_template_actions.h"

namespace bn
{

// visible

/**
 * @brief Manages if the sprite layer must be shown or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprites_visible_manager
{

public:
    /**
     * @brief Indicates if the sprite layer is visible or not.
     */
    [[nodiscard]] static bool get()
    {
        return sprites::visible();
    }

    /**
     * @brief Sets if the sprite layer is visible or not.
     */
    static void set(bool enabled)
    {
        sprites::set_visible(enabled);
    }
};


/**
 * @brief Toggles if the sprite layer must be shown or not when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprites_visible_toggle_action : public bool_toggle_template_action<sprites_visible_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the sprite layer must be shown or not.
     */
    explicit sprites_visible_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};


// blending

/**
 * @brief Manages if the sprite layer must be part of the blending bottom layer or not.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprites_blending_bottom_enabled_manager
{

public:
    /**
     * @brief Indicates if the sprite layer is part of the blending bottom layer or not.
     */
    [[nodiscard]] static bool get()
    {
        return sprites::blending_bottom_enabled();
    }

    /**
     * @brief Sets if the sprite layer must be part of the blending bottom layer or not.
     */
    static void set(bool enabled)
    {
        sprites::set_blending_bottom_enabled(enabled);
    }
};


/**
 * @brief Toggles if the sprite layer is part of the blending bottom layer or not
 * when the action is updated a given number of times.
 *
 * @ingroup sprite
 * @ingroup action
 */
class sprites_blending_bottom_enabled_toggle_action :
        public bool_toggle_template_action<sprites_blending_bottom_enabled_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to toggle
     * if the sprite layer is part of the blending bottom layer or not.
     */
    explicit sprites_blending_bottom_enabled_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};

}

#endif
