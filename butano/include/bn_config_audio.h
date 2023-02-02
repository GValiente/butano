/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * Specifies Direct Sound mixing rate in KHz.
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

#endif
