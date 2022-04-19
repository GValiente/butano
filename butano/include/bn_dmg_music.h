/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_H
#define BN_DMG_MUSIC_H

/**
 * @file
 * bn::dmg_music header file.
 *
 * @ingroup dmg_music
 */

#include "bn_optional_fwd.h"

namespace bn
{
    class dmg_music_item;
}

/**
 * @brief DMG music related functions.
 *
 * @ingroup dmg_music
 */
namespace bn::dmg_music
{
    /**
     * @brief Indicates if currently there's any DMG music playing or not.
     */
    [[nodiscard]] bool playing();

    /**
     * @brief Returns the active dmg_music_item if there's any DMG music playing; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<dmg_music_item> playing_item();

    /**
     * @brief Plays the DMG music specified by the given dmg_music_item with default settings.
     *
     * Default settings are speed = 1 and loop enabled.
     */
    void play(dmg_music_item item);

    /**
     * @brief Plays the DMG music specified by the given dmg_music_item.
     * @param item Specifies the DMG music to play.
     * @param speed Playback speed, in the range [1..256].
     */
    void play(dmg_music_item item, int speed);

    /**
     * @brief Plays the DMG music specified by the given dmg_music_item.
     * @param item Specifies the DMG music to play.
     * @param speed Playback speed, in the range [1..256].
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(dmg_music_item item, int speed, bool loop);

    /**
     * @brief Stops playback of the active DMG music.
     */
    void stop();

    /**
     * @brief Indicates if the active DMG music has been paused or not.
     */
    [[nodiscard]] bool paused();

    /**
     * @brief Pauses playback of the active DMG music.
     */
    void pause();

    /**
     * @brief Resumes playback of the paused DMG music.
     */
    void resume();
}

#endif
