/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_sound.h"
#include "bn_template_actions.h"
#include "bn_value_template_actions.h"

namespace bn
{

// speed

/**
 * @brief Manages the speed of a sound effect.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_speed_manager
{

public:
    /**
     * @brief Returns the speed of the given sound_handle.
     */
    [[nodiscard]] static fixed get(sound_handle handle)
    {
        return handle.speed();
    }

    /**
     * @brief Sets the speed of the given sound_handle.
     */
    static void set(fixed speed, sound_handle handle)
    {
        handle.set_speed(speed);
    }
};


/**
 * @brief Modifies the speed of a sound effect until it has a given state.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_speed_to_action : public to_value_template_action<sound_handle, fixed, sound_speed_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the speed of the given sound_handle is equal to final_speed.
     * @param final_speed Speed when the action is updated duration_updates times.
     */
    sound_speed_to_action(sound_handle handle, int duration_updates, fixed final_speed) :
        to_value_template_action(handle, duration_updates, final_speed)
    {
        BN_BASIC_ASSERT(final_speed >= 0 && final_speed <= 64, "Speed range is [0..64]: ", final_speed);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief Returns the speed of the given sound_handle when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_speed() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the speed of a sound effect from a minimum to a maximum.
 * When the speed is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_speed_loop_action : public loop_value_template_action<sound_handle, fixed, sound_speed_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the speed delta.
     * @param final_speed When the speed of the given sound_handle is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sound_speed_loop_action(sound_handle handle, int duration_updates, fixed final_speed) :
        loop_value_template_action(handle, duration_updates, final_speed)
    {
        BN_BASIC_ASSERT(final_speed >= 0 && final_speed <= 64, "Speed range is [0..64]: ", final_speed);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief When the speed of the given sound_handle is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_speed() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the speed of a sound effect when the action is updated a given number of times.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_speed_toggle_action : public toggle_value_template_action<sound_handle, fixed, sound_speed_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates How much times the action has to be updated to change the speed
     * of the given sound_handle.
     * @param new_speed New speed when the action is updated duration_updates times.
     */
    sound_speed_toggle_action(sound_handle handle, int duration_updates, fixed new_speed) :
        toggle_value_template_action(handle, duration_updates, new_speed)
    {
        BN_BASIC_ASSERT(new_speed >= 0 && new_speed <= 64, "Speed range is [0..64]: ", new_speed);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief Returns the speed of the given sound_handle when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_speed() const
    {
        return new_property();
    }
};


// panning

/**
 * @brief Manages the panning of a sound effect.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_panning_manager
{

public:
    /**
     * @brief Returns the panning of the given sound_handle.
     */
    [[nodiscard]] static fixed get(sound_handle handle)
    {
        return handle.panning();
    }

    /**
     * @brief Sets the panning of the given sound_handle.
     */
    static void set(fixed panning, sound_handle handle)
    {
        handle.set_panning(panning);
    }
};


/**
 * @brief Modifies the panning of a sound effect until it has a given state.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_panning_to_action : public to_value_template_action<sound_handle, fixed, sound_panning_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates Number of times that the action must be updated
     * until the panning of the given sound_handle is equal to final_panning.
     * @param final_panning Panning when the action is updated duration_updates times.
     */
    sound_panning_to_action(sound_handle handle, int duration_updates, fixed final_panning) :
        to_value_template_action(handle, duration_updates, final_panning)
    {
        BN_BASIC_ASSERT(final_panning >= -1 && final_panning <= 1, "Panning range is [-1..1]: ", final_panning);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief Returns the panning of the given sound_handle when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_panning() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the panning of a sound effect from a minimum to a maximum.
 * When the panning is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_panning_loop_action : public loop_value_template_action<sound_handle, fixed, sound_panning_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the panning delta.
     * @param final_panning When the panning of the given sound_handle is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    sound_panning_loop_action(sound_handle handle, int duration_updates, fixed final_panning) :
        loop_value_template_action(handle, duration_updates, final_panning)
    {
        BN_BASIC_ASSERT(final_panning >= -1 && final_panning <= 1, "Panning range is [-1..1]: ", final_panning);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief When the panning of the given sound_handle is equal to this returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_panning() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the panning of a sound effect when the action is updated a given number of times.
 *
 * @ingroup sound
 * @ingroup action
 */
class sound_panning_toggle_action : public toggle_value_template_action<sound_handle, fixed, sound_panning_manager>
{

public:
    /**
     * @brief Constructor.
     * @param handle sound_handle to modify.
     * @param duration_updates How much times the action has to be updated to change the panning
     * of the given sound_handle.
     * @param new_panning New panning when the action is updated duration_updates times.
     */
    sound_panning_toggle_action(sound_handle handle, int duration_updates, fixed new_panning) :
        toggle_value_template_action(handle, duration_updates, new_panning)
    {
        BN_BASIC_ASSERT(new_panning >= -1 && new_panning <= 1, "Panning range is [-1..1]: ", new_panning);
    }

    /**
     * @brief Returns the sound_handle to modify.
     */
    [[nodiscard]] sound_handle handle() const
    {
        return value();
    }

    /**
     * @brief Returns the panning of the given sound_handle when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_panning() const
    {
        return new_property();
    }
};


// master volume

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
