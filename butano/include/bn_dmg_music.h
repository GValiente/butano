/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_dmg_music_master_volume.h"

namespace bn
{
    class dmg_music_item;
    class dmg_music_position;
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
    void play(const dmg_music_item& item);

    /**
     * @brief Plays the DMG music specified by the given dmg_music_item.
     * @param item Specifies the DMG music to play.
     * @param speed Playback speed, in the range [1..256].
     *
     * VGM player only supports the default playback speed (1).
     */
    void play(const dmg_music_item& item, int speed);

    /**
     * @brief Plays the DMG music specified by the given dmg_music_item.
     * @param item Specifies the DMG music to play.
     * @param speed Playback speed, in the range [1..256].
     *
     * VGM player only supports the default playback speed (1).
     *
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(const dmg_music_item& item, int speed, bool loop);

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

    /**
     * @brief Returns the sequence position of the active DMG music.
     */
    [[nodiscard]] const dmg_music_position& position();

    /**
     * @brief Sets the sequence position of the active DMG music.
     * @param pattern Pattern order.
     * @param row Row inside the pattern.
     */
    void set_position(int pattern, int row);

    /**
     * @brief Sets the sequence position of the active DMG music.
     */
    void set_position(const dmg_music_position& position);

    /**
     * @brief Returns the volume of the active DMG music for the left speaker.
     */
    [[nodiscard]] fixed left_volume();

    /**
     * @brief Returns the volume of the active DMG music for the right speaker.
     */
    [[nodiscard]] fixed right_volume();

    /**
     * @brief Sets the volume of the active DMG music for the left speaker.
     *
     * Volume change is not supported by the VGM player.
     *
     * @param left_volume Left speaker volume level, in the range [0..1].
     */
    void set_left_volume(fixed left_volume);

    /**
     * @brief Sets the volume of the active DMG music for the right speaker.
     *
     * Volume change is not supported by the VGM player.
     *
     * @param right_volume Right speaker volume level, in the range [0..1].
     */
    void set_right_volume(fixed right_volume);

    /**
     * @brief Sets the volume of the active DMG music for both speakers.
     *
     * Volume change is not supported by the VGM player.
     *
     * @param volume Volume level, in the range [0..1].
     */
    void set_volume(fixed volume);

    /**
     * @brief Sets the volume of the active DMG music for both speakers.
     *
     * Volume change is not supported by the VGM player.
     *
     * @param left_volume Left speaker volume level, in the range [0..1].
     * @param right_volume Right speaker volume level, in the range [0..1].
     */
    void set_volume(fixed left_volume, fixed right_volume);

    /**
     * @brief Returns the master volume of all DMG music.
     */
    [[nodiscard]] dmg_music_master_volume master_volume();

    /**
     * @brief Sets the master volume of all DMG music.
     */
    void set_master_volume(dmg_music_master_volume volume);
}

#endif
