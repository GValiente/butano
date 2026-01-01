/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AUDIO_MIXING_RATE_H
#define BN_AUDIO_MIXING_RATE_H

/**
 * @file
 * bn::audio_mixing_rate header file.
 *
 * @ingroup audio
 */

#include "bn_common.h"

/**
 * @def BN_AUDIO_MIXING_RATE_8_KHZ
 *
 * 8KHz mixing rate, provides poor quality.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_8_KHZ     0

/**
 * @def BN_AUDIO_MIXING_RATE_10_KHZ
 *
 * 10Khz mixing rate, provides low quality.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_10_KHZ    1

/**
 * @def BN_AUDIO_MIXING_RATE_12_KHZ
 *
 * 12Khz mixing rate, provides almost OK quality.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_12_KHZ    8

/**
 * @def BN_AUDIO_MIXING_RATE_13_KHZ
 *
 * 13Khz mixing rate, provides almost OK quality.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_13_KHZ    2

/**
 * @def BN_AUDIO_MIXING_RATE_16_KHZ
 *
 * 16Khz mixing rate, provides OK quality, standard setting.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_16_KHZ    3

/**
 * @def BN_AUDIO_MIXING_RATE_18_KHZ
 *
 * 18Khz mixing rate, provides better quality, high CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_18_KHZ    4

/**
 * @def BN_AUDIO_MIXING_RATE_20_KHZ
 *
 * 20Khz mixing rate, provides better quality, high CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_20_KHZ    9

/**
 * @def BN_AUDIO_MIXING_RATE_21_KHZ
 *
 * 21Khz mixing rate, provides even better quality, higher CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_21_KHZ    5

/**
 * @def BN_AUDIO_MIXING_RATE_24_KHZ
 *
 * 24Khz mixing rate, provides even better quality, higher CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_24_KHZ    10

/**
 * @def BN_AUDIO_MIXING_RATE_27_KHZ
 *
 * 27Khz mixing rate, provides almost the best quality, almost the highest CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_27_KHZ    6

/**
 * @def BN_AUDIO_MIXING_RATE_28_KHZ
 *
 * 28Khz mixing rate, provides almost the best quality, almost the highest CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_28_KHZ    11

/**
 * @def BN_AUDIO_MIXING_RATE_31_KHZ
 *
 * 31Khz mixing rate, provides the best quality, highest CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_31_KHZ    7

/**
 * @def BN_AUDIO_MIXING_RATE_32_KHZ
 *
 * 32Khz mixing rate, provides the best quality, highest CPU load.
 *
 * @ingroup audio
 */
#define BN_AUDIO_MIXING_RATE_32_KHZ    12


namespace bn
{

/**
 * @brief Specifies the available Direct Sound mixing rates.
 *
 * @ingroup audio
 */
enum class audio_mixing_rate : uint8_t
{
    KHZ_8 = BN_AUDIO_MIXING_RATE_8_KHZ, //!< 8KHz mixing rate, provides poor quality.
    KHZ_10 = BN_AUDIO_MIXING_RATE_10_KHZ, //!< 10Khz mixing rate, provides low quality.
    KHZ_12 = BN_AUDIO_MIXING_RATE_12_KHZ, //!< 12Khz mixing rate, provides almost OK quality.
    KHZ_13 = BN_AUDIO_MIXING_RATE_13_KHZ, //!< 13Khz mixing rate, provides almost OK quality.
    KHZ_16 = BN_AUDIO_MIXING_RATE_16_KHZ, //!< 16Khz mixing rate, provides OK quality, standard setting.
    KHZ_18 = BN_AUDIO_MIXING_RATE_18_KHZ, //!< 18Khz mixing rate, provides better quality, high CPU load.
    KHZ_20 = BN_AUDIO_MIXING_RATE_20_KHZ, //!< 20Khz mixing rate, provides better quality, high CPU load.
    KHZ_21 = BN_AUDIO_MIXING_RATE_21_KHZ, //!< 21Khz mixing rate, provides even better quality, higher CPU load.
    KHZ_24 = BN_AUDIO_MIXING_RATE_24_KHZ, //!< 24Khz mixing rate, provides even better quality, higher CPU load.
    KHZ_27 = BN_AUDIO_MIXING_RATE_27_KHZ, //!< 27Khz mixing rate, provides almost the best quality, almost the highest CPU load.
    KHZ_28 = BN_AUDIO_MIXING_RATE_28_KHZ, //!< 28Khz mixing rate, provides almost the best quality, almost the highest CPU load.
    KHZ_31 = BN_AUDIO_MIXING_RATE_31_KHZ, //!< 31Khz mixing rate, provides the best quality, highest CPU load.
    KHZ_32 = BN_AUDIO_MIXING_RATE_32_KHZ //!< 32Khz mixing rate, provides the best quality, highest CPU load.
};

}

#endif
