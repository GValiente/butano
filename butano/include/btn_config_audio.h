/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_AUDIO_H
#define BTN_CONFIG_AUDIO_H

#include "btn_audio_mixing_rate.h"

#ifndef BTN_CFG_AUDIO_MIXING_RATE
    #define BTN_CFG_AUDIO_MIXING_RATE BTN_AUDIO_MIXING_RATE_16_KHZ
#endif

#ifndef BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS
    #define BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS 16
#endif

#ifndef BTN_CFG_AUDIO_MAX_SOUND_CHANNELS
    #define BTN_CFG_AUDIO_MAX_SOUND_CHANNELS 4
#endif

#ifndef BTN_CFG_AUDIO_MAX_COMMANDS
    #define BTN_CFG_AUDIO_MAX_COMMANDS BTN_CFG_AUDIO_MAX_SOUND_CHANNELS * 2
#endif

#endif
