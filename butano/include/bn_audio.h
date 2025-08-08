/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_span.h"

namespace bn
{
    enum class audio_mixing_rate : uint8_t;
}

/**
 * @brief Audio related functions.
 *
 * @ingroup audio
 */
namespace bn::audio
{
    /**
     * @brief Returns the available Direct Sound mixing rates.
     */
    [[nodiscard]] span<const audio_mixing_rate> available_mixing_rates();

    /**
     * @brief Returns the current Direct Sound mixing rate.
     */
    [[nodiscard]] audio_mixing_rate mixing_rate();

    /**
     * @brief Specifies the current Direct Sound mixing rate.
     *
     * All Direct Sound audio playback must be stopped before changing the mixing rate.
     */
    void set_mixing_rate(audio_mixing_rate mixing_rate);

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
     * @brief Indicates if audio events are registered or not.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about audio events.
     */
    [[nodiscard]] bool event_handler_enabled();

    /**
     * @brief Sets if audio events must be registered or not.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about audio events.
     */
    void set_event_handler_enabled(bool enabled);

    /**
     * @brief Returns the ID of the audio events registered since the last core::update call.
     *
     * To register audio events, the event handler must be enabled with audio::set_event_handler_enabled.
     *
     * See https://blocksds.skylyrac.net/docs/maxmod/md_documentation_2using__song__events.html
     * for more information about audio events.
     */
    [[nodiscard]] span<uint8_t> event_ids();
}

#endif
