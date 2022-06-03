/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_fixed.h"

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
     */
    void play(sound_item item);

    /**
     * @brief Plays the sound effect specified by the given sound_item.
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     */
    void play(sound_item item, fixed volume);

    /**
     * @brief Plays the sound effect specified by the given sound_item.
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     */
    void play(sound_item item, fixed volume, fixed speed, fixed panning);

    /**
     * @brief Plays the sound effect specified by the given sound_item with default settings and the given priority.
     *
     * If there's playing too much sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * Default settings are volume = 1, speed = 1 and panning = 0.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     */
    void play_with_priority(int priority, sound_item item);

    /**
     * @brief Plays the sound effect specified by the given sound_item with the given priority.
     *
     * If there's playing too much sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     */
    void play_with_priority(int priority, sound_item item, fixed volume);

    /**
     * @brief Plays the sound effect specified by the given sound_item with the given priority.
     *
     * If there's playing too much sound effects at the same time,
     * sound effects with higher priority are discarded later.
     *
     * @param priority Priority relative to backgrounds in the range [-32767..32767].
     * @param item Specifies the sound effect to play.
     * @param volume Volume level, in the range [0..1].
     * @param speed Playback rate, in the range [0..64].
     * @param panning Panning level, in the range [-1..1].
     */
    void play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    /**
     * @brief Stops all sound effects that are being played currently.
     */
    void stop_all();
}

#endif
