/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MUSIC_ACTIONS_H
#define BN_MUSIC_ACTIONS_H

/**
 * @file
 * bn::music actions header file.
 *
 * @ingroup music
 * @ingroup action
 */

#include "bn_music.h"
#include "bn_template_actions.h"

namespace bn
{

// volume

/**
 * @brief Manages the volume of the active Direct Sound music.
 *
 * @ingroup music
 * @ingroup action
 */
class music_volume_manager
{

public:
    /**
     * @brief Returns the volume of the active Direct Sound music.
     */
    [[nodiscard]] static fixed get()
    {
        return music::volume();
    }

    /**
     * @brief Sets the volume of the active Direct Sound music.
     * @param volume New volume in the range [0..1].
     */
    static void set(fixed volume)
    {
        music::set_volume(volume);
    }
};


/**
 * @brief Modifies the volume of the active Direct Sound music until it has a given state.
 *
 * @ingroup music
 * @ingroup action
 */
class music_volume_to_action : public to_template_action<fixed, music_volume_manager>
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
    music_volume_to_action(int duration_updates, fixed final_volume) :
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
 * @brief Modifies the volume of the active Direct Sound music from a minimum to a maximum.
 * When the volume is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup music
 * @ingroup action
 */
class music_volume_loop_action : public loop_template_action<fixed, music_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the volume delta.
     * @param final_volume When the volume is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This volume must be in the range [0..1].
     */
    music_volume_loop_action(int duration_updates, fixed final_volume) :
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
 * @brief Changes the volume of the active Direct Sound music when the action is updated a given number of times.
 *
 * @ingroup music
 * @ingroup action
 */
class music_volume_toggle_action : public toggle_template_action<fixed, music_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the volume.
     * @param new_volume New volume when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    music_volume_toggle_action(int duration_updates, fixed new_volume) :
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


// tempo

/**
 * @brief Manages the tempo of the active Direct Sound music.
 *
 * @ingroup music
 * @ingroup action
 */
class music_tempo_manager
{

public:
    /**
     * @brief Returns the tempo of the active Direct Sound music.
     */
    [[nodiscard]] static fixed get()
    {
        return music::tempo();
    }

    /**
     * @brief Sets the tempo of the active Direct Sound music.
     * @param tempo New tempo in the range [0.5..2].
     */
    static void set(fixed tempo)
    {
        music::set_tempo(tempo);
    }
};


/**
 * @brief Modifies the tempo of the active Direct Sound music until it has a given state.
 *
 * @ingroup music
 * @ingroup action
 */
class music_tempo_to_action : public to_template_action<fixed, music_tempo_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the tempo is equal to final_tempo.
     * @param final_tempo tempo when the action is updated duration_updates times.
     *
     * This tempo must be in the range [0.5..2].
     */
    music_tempo_to_action(int duration_updates, fixed final_tempo) :
        to_template_action(duration_updates, final_tempo)
    {
        BN_ASSERT(final_tempo >= 0.5 && final_tempo <= 2, "Invalid final tempo: ", final_tempo);
    }

    /**
     * @brief Returns the tempo when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_tempo() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the tempo of the active Direct Sound music from a minimum to a maximum.
 * When the tempo is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup music
 * @ingroup action
 */
class music_tempo_loop_action : public loop_template_action<fixed, music_tempo_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the tempo delta.
     * @param final_tempo When the tempo is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This tempo must be in the range [0.5..2].
     */
    music_tempo_loop_action(int duration_updates, fixed final_tempo) :
        loop_template_action(duration_updates, final_tempo)
    {
        BN_ASSERT(final_tempo >= 0.5 && final_tempo <= 2, "Invalid final tempo: ", final_tempo);
    }

    /**
     * @brief When the tempo is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_tempo() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the tempo of the active Direct Sound music when the action is updated a given number of times.
 *
 * @ingroup music
 * @ingroup action
 */
class music_tempo_toggle_action : public toggle_template_action<fixed, music_tempo_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the tempo.
     * @param new_tempo New tempo when the action is updated duration_updates times.
     *
     * This tempo must be in the range [0.5..2].
     */
    music_tempo_toggle_action(int duration_updates, fixed new_tempo) :
        toggle_template_action(duration_updates, new_tempo)
    {
        BN_ASSERT(new_tempo >= 0.5 && new_tempo <= 2, "Invalid new tempo: ", new_tempo);
    }

    /**
     * @brief Returns the tempo when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_tempo() const
    {
        return new_property();
    }
};


// pitch

/**
 * @brief Manages the pitch of the active Direct Sound music.
 *
 * @ingroup music
 * @ingroup action
 */
class music_pitch_manager
{

public:
    /**
     * @brief Returns the pitch of the active Direct Sound music.
     */
    [[nodiscard]] static fixed get()
    {
        return music::pitch();
    }

    /**
     * @brief Sets the pitch of the active Direct Sound music.
     * @param pitch New pitch in the range [0.5..2].
     */
    static void set(fixed pitch)
    {
        music::set_pitch(pitch);
    }
};


/**
 * @brief Modifies the pitch of the active Direct Sound music until it has a given state.
 *
 * @ingroup music
 * @ingroup action
 */
class music_pitch_to_action : public to_template_action<fixed, music_pitch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the pitch is equal to final_pitch.
     * @param final_pitch pitch when the action is updated duration_updates times.
     *
     * This pitch must be in the range [0.5..2].
     */
    music_pitch_to_action(int duration_updates, fixed final_pitch) :
        to_template_action(duration_updates, final_pitch)
    {
        BN_ASSERT(final_pitch >= 0.5 && final_pitch <= 2, "Invalid final pitch: ", final_pitch);
    }

    /**
     * @brief Returns the pitch when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_pitch() const
    {
        return final_property();
    }
};


/**
 * @brief Modifies the pitch of the active Direct Sound music from a minimum to a maximum.
 * When the pitch is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @ingroup music
 * @ingroup action
 */
class music_pitch_loop_action : public loop_template_action<fixed, music_pitch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated
     * before changing the direction of the pitch delta.
     * @param final_pitch When the pitch is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This pitch must be in the range [0.5..2].
     */
    music_pitch_loop_action(int duration_updates, fixed final_pitch) :
        loop_template_action(duration_updates, final_pitch)
    {
        BN_ASSERT(final_pitch >= 0.5 && final_pitch <= 2, "Invalid final pitch: ", final_pitch);
    }

    /**
     * @brief When the pitch is equal to the returned parameter, it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_pitch() const
    {
        return final_property();
    }
};


/**
 * @brief Changes the pitch of the active Direct Sound music when the action is updated a given number of times.
 *
 * @ingroup music
 * @ingroup action
 */
class music_pitch_toggle_action : public toggle_template_action<fixed, music_pitch_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How many times the action has to be updated to change the pitch.
     * @param new_pitch New pitch when the action is updated duration_updates times.
     *
     * This pitch must be in the range [0.5..2].
     */
    music_pitch_toggle_action(int duration_updates, fixed new_pitch) :
        toggle_template_action(duration_updates, new_pitch)
    {
        BN_ASSERT(new_pitch >= 0.5 && new_pitch <= 2, "Invalid new pitch: ", new_pitch);
    }

    /**
     * @brief Returns the pitch when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_pitch() const
    {
        return new_property();
    }
};

}

#endif
