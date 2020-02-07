#ifndef BTN_CONFIG_AUDIO_H
#define BTN_CONFIG_AUDIO_H

#include "btn_audio_khz.h"

#ifndef BTN_CFG_AUDIO_KHZ
    #define BTN_CFG_AUDIO_KHZ BTN_AUDIO_KHZ_16
#endif

#ifndef BTN_CFG_AUDIO_MAX_CHANNELS
    #define BTN_CFG_AUDIO_MAX_CHANNELS 16 + 4
#endif

#ifndef BTN_CFG_AUDIO_MAX_COMMANDS
    #define BTN_CFG_AUDIO_MAX_COMMANDS BTN_CFG_AUDIO_MAX_CHANNELS * 2
#endif

#endif
