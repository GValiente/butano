/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_AUDIO_H
#define BN_CONFIG_AUDIO_H

/**
 * @file
 * Audio configuration header file.
 *
 * @ingroup audio
 */

#include "bn_audio_mixing_rate.h"

/**
 * @def BN_CFG_AUDIO_MIXING_RATE
 *
 * Specifies the initial Direct Sound mixing rate.
 *
 * Values not specified in BN_AUDIO_MIXING_RATE_* macros are not allowed.
 *
 * @ingroup audio
 */
#ifndef BN_CFG_AUDIO_MIXING_RATE
    #define BN_CFG_AUDIO_MIXING_RATE BN_AUDIO_MIXING_RATE_16_KHZ
#endif

/**
 * @def BN_CFG_AUDIO_MAX_MUSIC_CHANNELS
 *
 * Specifies the maximum number of active Direct Sound music channels.
 *
 * @ingroup music
 */
#ifndef BN_CFG_AUDIO_MAX_MUSIC_CHANNELS
    #define BN_CFG_AUDIO_MAX_MUSIC_CHANNELS 16
#endif

/**
 * @def BN_CFG_AUDIO_MAX_SOUND_CHANNELS
 *
 * Specifies the maximum number of active sound effects.
 *
 * @ingroup sound
 */
#ifndef BN_CFG_AUDIO_MAX_SOUND_CHANNELS
    #define BN_CFG_AUDIO_MAX_SOUND_CHANNELS 4
#endif

/**
 * @def BN_CFG_AUDIO_STEREO
 *
 * Specifies Direct Sound mono/stereo output.
 *
 * Mono output reduces CPU load.
 *
 * @ingroup audio
 */
#ifndef BN_CFG_AUDIO_STEREO
    #define BN_CFG_AUDIO_STEREO true
#endif

/**
 * @def BN_CFG_AUDIO_DYNAMIC_MIXING
 *
 * Specifies if Direct Sound dynamic mixing must be enabled or not.
 *
 * Dynamic mixing automatically adjusts the mixing rate depending on the frequency of the sounds being played,
 * up to a maximum of the mixing rate specified. It often yields a significant performance boost
 * for only a very small drop in quality.
 *
 * @ingroup audio
 */
#ifndef BN_CFG_AUDIO_DYNAMIC_MIXING
    #define BN_CFG_AUDIO_DYNAMIC_MIXING false
#endif

/**
 * @def BN_CFG_AUDIO_MAX_COMMANDS
 *
 * Specifies the size of the audio commands list.
 *
 * This list is processed and cleared when bn::core::update() is called.
 *
 * @ingroup audio
 */
#ifndef BN_CFG_AUDIO_MAX_COMMANDS
    #define BN_CFG_AUDIO_MAX_COMMANDS (((BN_CFG_AUDIO_MAX_SOUND_CHANNELS) * 2) + 1)
#endif

/**
 * @def BN_CFG_AUDIO_MAX_EVENTS
 *
 * Specifies the maximum number of active audio events per update.
 *
 * See https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html
 * for more information about audio events.
 *
 * @ingroup music
 */
#ifndef BN_CFG_AUDIO_MAX_EVENTS
    #define BN_CFG_AUDIO_MAX_EVENTS 8
#endif

#endif
