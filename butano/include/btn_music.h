/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MUSIC_H
#define BTN_MUSIC_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class music_item;
}

/**
 * @brief Music related functions.
 *
 * @ingroup music
 */
namespace btn::music
{
    /**
     * @brief Indicates if currently there's any music playing or not.
     */
    [[nodiscard]] bool playing();

    /**
     * @brief Plays the music specified by the given music_item with default settings.
     *
     * Default settings are volume = 1 and loop enabled.
     */
    void play(music_item item);

    /**
     * @brief Plays the music specified by the given music_item.
     * @param item Specifies the music to play.
     * @param volume Volume level, in the range [0..1].
     */
    void play(music_item item, fixed volume);

    /**
     * @brief Plays the music specified by the given music_item.
     * @param item Specifies the music to play.
     * @param volume Volume level, in the range [0..1].
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(music_item item, fixed volume, bool loop);

    /**
     * @brief Stops playback of the active music.
     */
    void stop();

    /**
     * @brief Indicates if the active music has been paused or not.
     */
    [[nodiscard]] bool paused();

    /**
     * @brief Pauses playback of the active music.
     */
    void pause();

    /**
     * @brief Resumes playback of the paused music.
     */
    void resume();

    /**
     * @brief Returns the sequence position of the active music.
     */
    [[nodiscard]] int position();

    /**
     * @brief Sets the sequence position of the active music.
     */
    void set_position(int position);

    /**
     * @brief Returns the volume of the active music.
     */
    [[nodiscard]] fixed volume();

    /**
     * @brief Sets the volume of the active music.
     * @param volume Volume level, in the range [0..1].
     */
    void set_volume(fixed volume);
}

#endif
