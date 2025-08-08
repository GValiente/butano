/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MUSIC_H
#define BN_MUSIC_H

/**
 * @file
 * bn::music header file.
 *
 * @ingroup music
 */

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{
    class music_item;
}

/**
 * @brief Direct Sound music related functions.
 *
 * @ingroup music
 */
namespace bn::music
{
    /**
     * @brief Indicates if currently there's any Direct Sound music playing or not.
     */
    [[nodiscard]] bool playing();

    /**
     * @brief Returns the active music_item if there's any Direct Sound music playing; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<music_item> playing_item();

    /**
     * @brief Plays the Direct Sound music specified by the given music_item with default settings.
     *
     * Default settings are volume = 1 and loop enabled.
     */
    void play(music_item item);

    /**
     * @brief Plays the Direct Sound music specified by the given music_item.
     * @param item Specifies the music to play.
     * @param volume Volume level, in the range [0..1].
     */
    void play(music_item item, fixed volume);

    /**
     * @brief Plays the Direct Sound music specified by the given music_item.
     * @param item Specifies the music to play.
     * @param volume Volume level, in the range [0..1].
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(music_item item, fixed volume, bool loop);

    /**
     * @brief Stops playback of the active Direct Sound music.
     */
    void stop();

    /**
     * @brief Indicates if the active Direct Sound music has been paused or not.
     */
    [[nodiscard]] bool paused();

    /**
     * @brief Pauses playback of the active Direct Sound music.
     */
    void pause();

    /**
     * @brief Resumes playback of the paused Direct Sound music.
     */
    void resume();

    /**
     * @brief Returns the sequence position of the active Direct Sound music.
     */
    [[nodiscard]] int position();

    /**
     * @brief Sets the sequence position of the active Direct Sound music.
     */
    void set_position(int position);

    /**
     * @brief Returns the volume of the active Direct Sound music.
     */
    [[nodiscard]] fixed volume();

    /**
     * @brief Sets the volume of the active Direct Sound music.
     * @param volume Volume level, in the range [0..1].
     */
    void set_volume(fixed volume);

    /**
     * @brief Returns the tempo of the active Direct Sound music.
     */
    [[nodiscard]] fixed tempo();

    /**
     * @brief Sets the tempo of the active Direct Sound music.
     * @param tempo Tempo level, in the range [0.5..2].
     */
    void set_tempo(fixed tempo);

    /**
     * @brief Returns the pitch of the active Direct Sound music.
     */
    [[nodiscard]] fixed pitch();

    /**
     * @brief Sets the pitch of the active Direct Sound music.
     * @param pitch Pitch level, in the range [0.5..2].
     */
    void set_pitch(fixed pitch);

    /**
     * @brief Indicates if music events are registered or not.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about music events.
     */
    [[nodiscard]] bool event_handler_enabled();

    /**
     * @brief Sets if music events must be registered or not.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about music events.
     */
    void set_event_handler_enabled(bool enabled);

    /**
     * @brief Returns the ID of the music events registered since the last core::update call.
     *
     * To register music events, the event handler must be enabled with music::set_event_handler_enabled.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about music events.
     */
    [[nodiscard]] span<uint8_t> event_ids();
}

#endif
