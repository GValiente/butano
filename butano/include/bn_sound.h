/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SOUND_H
#define BN_SOUND_H

/**
 * @file
 * bn::sound header file.
 *
 * @ingroup sound
 */

#include "bn_sound_handle.h"

namespace bn
{
    class sound_item;
}

/**
 * @brief Sound effects related functions.
 *
 * @ingroup sound
 */
namespace bn::sound
{
    /**
     * @brief Plays the sound effect specified by the given sound_item with default settings.
     *
     * Default settings are volume = 1, speed = 1 and panning = 0.
     * @param item Specifies the sound effect to play.
     * @return Sound effect handle.
     */
    sound_handle play(sound_item item);

    /**
     * @brief Plays the sound effect specified by the given sound_item.
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @return Sound effect handle.
     */
    sound_handle play(sound_item item, fixed volume);

    /**
     * @brief Plays the sound effect specified by the given sound_item.
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     * @return Sound effect handle.
     */
    sound_handle play(sound_item item, fixed volume, fixed speed, fixed panning);

    /**
     * @brief Plays the sound effect specified by the given sound_item with default settings and the given priority.
     *
     * If too many sound effects are being played at the same time,
     * sound effects with higher priority are discarded later.
     *
     * Default settings are volume = 1, speed = 1 and panning = 0.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority, sound_item item);

    /**
     * @brief Plays the sound effect specified by the given sound_item with the given priority.
     *
     * If too many sound effects are being played at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority, sound_item item, fixed volume);

    /**
     * @brief Plays the sound effect specified by the given sound_item with the given priority.
     *
     * If too many sound effects are being played at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     * @return Sound effect handle.
     */
    sound_handle play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    /**
     * @brief Stops all sound effects that are being played currently.
     */
    void stop_all();

    /**
     * @brief Returns the master volume of all sound effects.
     */
    [[nodiscard]] fixed master_volume();

    /**
     * @brief Sets the master volume of all sound effects.
     * @param volume Master volume level, in the range [0..1].
     */
    void set_master_volume(fixed volume);
}

#endif
