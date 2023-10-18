/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_JINGLE_H
#define BN_JINGLE_H

/**
 * @file
 * bn::jingle header file.
 *
 * @ingroup music
 */

#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{
    class music_item;
}

/**
 * @brief Direct Sound jingles related functions.
 *
 * Jingles are limited to 4 channels only and can't be paused or stopped.
 *
 * @ingroup music
 */
namespace bn::jingle
{
    /**
     * @brief Indicates if currently there's any Direct Sound jingle playing or not.
     */
    [[nodiscard]] bool playing();

    /**
     * @brief Returns the active music_item if there's any Direct Sound jingle playing; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<music_item> playing_item();

    /**
     * @brief Plays the Direct Sound jingle specified by the given music_item with volume = 1.
     *
     * Remember that jingles are limited to 4 channels only and can't be paused or stopped.
     */
    void play(music_item item);

    /**
     * @brief Plays the Direct Sound jingle specified by the given music_item.
     *
     * Remember that jingles are limited to 4 channels only and can't be paused or stopped.
     *
     * @param item Specifies the jingle to play.
     * @param volume Volume level, in the range [0..1].
     */
    void play(music_item item, fixed volume);

    /**
     * @brief Returns the volume of the active Direct Sound jingle.
     */
    [[nodiscard]] fixed volume();

    /**
     * @brief Sets the volume of the active Direct Sound jingle.
     * @param volume Volume level, in the range [0..1].
     */
    void set_volume(fixed volume);
}

#endif
