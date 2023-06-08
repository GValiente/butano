/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SOUND_ACTIONS_H
#define BN_SOUND_ACTIONS_H

/**
 * @file
 * bn::sound actions header file.
 *
 * @ingroup sound
 * @ingroup action
 */

#include "bn_fixed.h"
#include "bn_sound.h"
#include "bn_template_actions.h"

namespace bn
{

// volume

/**
 * @brief Manages the master volume of all sound effects.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_master_volume_manager
{

public:
    /**
     * @brief Returns the master volume of all sound effects.
     */
    [[nodiscard]] static fixed get()
    {
        return sound::master_volume();
    }

    /**
     * @brief Sets the master volume of all sound effects.
     * @param volume New volume in the range [0..1].
     */
    static void set(fixed volume)
    {
        sound::set_master_volume(volume);
    }
};


/**
 * @brief Modifies the master volume of all sound effects until it has a given state.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_master_volume_to_action : public to_template_action<fixed, sound_master_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the volume is equal to final_volume.
     * @param final_volume Volume when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    sound_master_volume_to_action(int duration_updates, fixed final_volume) :
        to_template_action(duration_updates, final_volume)
    {
        BN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    /**
     * @brief Returns the volume when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_volume() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the master volume of all sound effects from a minimum to a maximum.
 * When the volume is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_master_volume_loop_action : public loop_template_action<fixed, sound_master_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the volume delta.
     * @param final_volume When the volume is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This volume must be in the range [0..1].
     */
    sound_master_volume_loop_action(int duration_updates, fixed final_volume) :
        loop_template_action(duration_updates, final_volume)
    {
        BN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    /**
     * @brief When the volume is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_volume() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the master volume of all sound effects when the action is updated a given number of times.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_master_volume_toggle_action : public toggle_template_action<fixed, sound_master_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the volume.
     * @param new_volume New volume when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    sound_master_volume_toggle_action(int duration_updates, fixed new_volume) :
        toggle_template_action(duration_updates, new_volume)
    {
        BN_ASSERT(new_volume >= 0 && new_volume <= 1, "Invalid new volume: ", new_volume);
    }

    /**
     * @brief Returns the volume when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_volume() const
    {
        return new_property();
    }
};

}

#endif
