/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_ACTIONS_H
#define BN_DMG_MUSIC_ACTIONS_H

/**
 * @file
 * bn::dmg_music actions header file.
 *
 * @ingroup dmg_music
 * @ingroup action
 */

#include "bn_dmg_music.h"
#include "bn_fixed_point.h"
#include "bn_template_actions.h"

namespace bn
{

/**
 * @brief Manages the volume of the active DMG music.
 *
 * @ingroup dmg_music
 * @ingroup action
 */
class dmg_music_volume_manager
{

public:
    /**
     * @brief Returns the volume of the active DMG music for both speakers.
     */
    [[nodiscard]] static fixed_point get()
    {
        return fixed_point(dmg_music::left_volume(), dmg_music::right_volume());
    }

    /**
     * @brief Sets the volume of the active DMG music for both speakers.
     *
     * Volume change is not supported by the VGM player.
     *
     * @param volume New volume in the range [0..1].
     */
    static void set(const fixed_point& volume)
    {
        dmg_music::set_volume(volume.x(), volume.y());
    }
};


/**
 * @brief Modifies the volume of the active DMG music until it has a given state.
 *
 * Volume change is not supported by the VGM player.
 *
 * @ingroup dmg_music
 * @ingroup action
 */
class dmg_music_volume_to_action : public to_template_action<fixed_point, dmg_music_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the volume is equal to final_volume.
     * @param final_volume Volume for both speakers when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_to_action(int duration_updates, fixed final_volume) :
        to_template_action(duration_updates, fixed_point(final_volume, final_volume))
    {
        BN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    /**
     * @brief Constructor.
     * @param duration_updates Number of times that the action must be updated
     * until the volume is equal to final_left_volume and final_right_volume.
     * @param final_left_volume Volume for the left speaker when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     * @param final_right_volume Volume for the right speaker when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_to_action(int duration_updates, fixed final_left_volume, fixed final_right_volume) :
        to_template_action(duration_updates, fixed_point(final_left_volume, final_right_volume))
    {
        BN_ASSERT(final_left_volume >= 0 && final_left_volume <= 1,
                  "Invalid final left volume: ", final_left_volume);
        BN_ASSERT(final_right_volume >= 0 && final_right_volume <= 1,
                  "Invalid final right volume: ", final_right_volume);
    }

    /**
     * @brief Returns the volume for the left speaker when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_left_volume() const
    {
        return final_property().x();
    }

    /**
     * @brief Returns the volume for the right speaker when the action is updated the given number of times.
     */
    [[nodiscard]] fixed final_right_volume() const
    {
        return final_property().y();
    }
};


/**
 * @brief Modifies the volume of the active DMG music from a minimum to a maximum.
 * When the volume is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * Volume change is not supported by the VGM player.
 *
 * @ingroup dmg_music
 * @ingroup action
 */
class dmg_music_volume_loop_action : public loop_template_action<fixed_point, dmg_music_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the volume delta.
     * @param final_volume When the volume for both speakers is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_loop_action(int duration_updates, fixed final_volume) :
        loop_template_action(duration_updates, fixed_point(final_volume, final_volume))
    {
        BN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the volume delta.
     * @param final_left_volume When the volume for the left speaker is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This volume must be in the range [0..1].
     * @param final_right_volume When the volume for the right speaker is equal to this parameter,
     * it goes back to its initial state and vice versa.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_loop_action(int duration_updates, fixed final_left_volume, fixed final_right_volume) :
        loop_template_action(duration_updates, fixed_point(final_left_volume, final_right_volume))
    {
        BN_ASSERT(final_left_volume >= 0 && final_left_volume <= 1,
                  "Invalid final left volume: ", final_left_volume);
        BN_ASSERT(final_right_volume >= 0 && final_right_volume <= 1,
                  "Invalid final right volume: ", final_right_volume);
    }

    /**
     * @brief When the volume for the left speaker is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_left_volume() const
    {
        return final_property().x();
    }

    /**
     * @brief When the volume for the right speaker is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] fixed final_right_volume() const
    {
        return final_property().y();
    }
};


/**
 * @brief Changes the volume of the active DMG music when the action is updated a given number of times.
 *
 * Volume change is not supported by the VGM player.
 *
 * @ingroup dmg_music
 * @ingroup action
 */
class dmg_music_volume_toggle_action : public toggle_template_action<fixed_point, dmg_music_volume_manager>
{

public:
    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the volume.
     * @param new_volume New volume for both speakers when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_toggle_action(int duration_updates, fixed new_volume) :
        toggle_template_action(duration_updates, fixed_point(new_volume, new_volume))
    {
        BN_ASSERT(new_volume >= 0 && new_volume <= 1, "Invalid new volume: ", new_volume);
    }

    /**
     * @brief Constructor.
     * @param duration_updates How much times the action has to be updated to change the volume.
     * @param new_left_volume New volume for the left speaker when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     * @param new_right_volume New volume for the right speaker when the action is updated duration_updates times.
     *
     * This volume must be in the range [0..1].
     */
    dmg_music_volume_toggle_action(int duration_updates, fixed new_left_volume, fixed new_right_volume) :
        toggle_template_action(duration_updates, fixed_point(new_left_volume, new_right_volume))
    {
        BN_ASSERT(new_left_volume >= 0 && new_left_volume <= 1, "Invalid new left volume: ", new_left_volume);
        BN_ASSERT(new_right_volume >= 0 && new_right_volume <= 1, "Invalid new right volume: ", new_right_volume);
    }

    /**
     * @brief Returns the volume for the left speaker when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_left_volume() const
    {
        return new_property().x();
    }

    /**
     * @brief Returns the volume for the right speaker when the action is updated the given number of times.
     */
    [[nodiscard]] fixed new_right_volume() const
    {
        return new_property().y();
    }
};

}

#endif
