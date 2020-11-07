/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_AUDIO_MIXING_RATE_H
#define BTN_AUDIO_MIXING_RATE_H

#include "btn_common.h"

/**
 * @def BTN_AUDIO_MIXING_RATE_8_KHZ
 *
 * 8KHz mixing rate, provides poor quality.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_8_KHZ     0

/**
 * @def BTN_AUDIO_MIXING_RATE_10_KHZ
 *
 * 10Khz mixing rate, provides low quality.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_10_KHZ    1

/**
 * @def BTN_AUDIO_MIXING_RATE_13_KHZ
 *
 * 13Khz mixing rate, provides almost OK quality.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_13_KHZ    2

/**
 * @def BTN_AUDIO_MIXING_RATE_16_KHZ
 *
 * 16Khz mixing rate, provides OK quality, standard setting.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_16_KHZ    3

/**
 * @def BTN_AUDIO_MIXING_RATE_18_KHZ
 *
 * 18Khz mixing rate, provides better quality, high CPU load.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_18_KHZ    4

/**
 * @def BTN_AUDIO_MIXING_RATE_21_KHZ
 *
 * 21Khz mixing rate, provides even better quality, higher CPU load.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_21_KHZ    5

/**
 * @def BTN_AUDIO_MIXING_RATE_27_KHZ
 *
 * 27Khz mixing rate, provides almost the best quality, almost the highest CPU load.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_27_KHZ    6

/**
 * @def BTN_AUDIO_MIXING_RATE_31_KHZ
 *
 * 31Khz mixing rate, provides the best quality, highest CPU load.
 *
 * @ingroup audio
 */
#define BTN_AUDIO_MIXING_RATE_31_KHZ    7

#endif
