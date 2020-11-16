/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_AUDIO_H
#define BTN_CONFIG_AUDIO_H

/**
 * @file
 * Audio configuration header file.
 *
 * @ingroup audio
 */

#include "btn_audio_mixing_rate.h"

/**
 * @def BTN_CFG_AUDIO_MIXING_RATE
 *
 * Specifies software audio mixing rate in KHz.
 *
 * Values not specified in BTN_AUDIO_MIXING_RATE_* macros are not allowed.
 *
 * @ingroup audio
 */
#ifndef BTN_CFG_AUDIO_MIXING_RATE
    #define BTN_CFG_AUDIO_MIXING_RATE BTN_AUDIO_MIXING_RATE_16_KHZ
#endif

/**
 * @def BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS
 *
 * Specifies the maximum number of active music channels.
 *
 * @ingroup music
 */
#ifndef BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS
    #define BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS 16
#endif

/**
 * @def BTN_CFG_AUDIO_MAX_SOUND_CHANNELS
 *
 * Specifies the maximum number of active sound effects.
 *
 * @ingroup sound
 */
#ifndef BTN_CFG_AUDIO_MAX_SOUND_CHANNELS
    #define BTN_CFG_AUDIO_MAX_SOUND_CHANNELS 4
#endif

/**
 * @def BTN_CFG_AUDIO_MAX_COMMANDS
 *
 * Specifies the size of the audio commands queue.
 *
 * This queue is processed and cleared when btn::core::update() is called.
 *
 * @ingroup audio
 */
#ifndef BTN_CFG_AUDIO_MAX_COMMANDS
    #define BTN_CFG_AUDIO_MAX_COMMANDS (BTN_CFG_AUDIO_MAX_SOUND_CHANNELS * 2) + 1
#endif

#endif
