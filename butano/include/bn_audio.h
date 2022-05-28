/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AUDIO_H
#define BN_AUDIO_H

/**
 * @file
 * bn::audio header file.
 *
 * @ingroup audio
 */

#include "bn_common.h"

/**
 * @brief Audio related functions.
 *
 * @ingroup audio
 */
namespace bn::audio
{
    /**
     * @brief Indicates if audio is updated on the V-Blank interrupt or not.
     *
     * Updating audio on the V-Blank interrupt helps to reduce audio noise
     * but increases the possibility of visual bugs because of lack of V-Blank time.
     */
    [[nodiscard]] bool update_on_vblank();

    /**
     * @brief Sets if audio must be updated on the V-Blank interrupt or not.
     *
     * Updating audio on the V-Blank interrupt helps to reduce audio noise
     * but increases the possibility of visual bugs because of lack of V-Blank time.
     */
    void set_update_on_vblank(bool update_on_vblank);

    /**
     * @brief Indicates if DMG music update frequency is synchronized with Direct Sound music.
     */
    [[nodiscard]] bool dmg_sync_enabled();

    /**
     * @brief Sets if DMG music update frequency must be synchronized with Direct Sound music.
     *
     * If this synchronization is enabled, the user is responsible for keeping both music systems
     * in the same position (e.g. pausing and resuming them at the same time).
     */
    void set_dmg_sync_enabled(bool dmg_sync_enabled);
}

#endif
