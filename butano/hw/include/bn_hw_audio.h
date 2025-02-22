/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_H
#define BN_HW_AUDIO_H

#ifdef BN_AUDIO_BACKEND_MAXMOD
    #include "bn_hw_audio_maxmod.h"
#else
    #ifdef BN_AUDIO_BACKEND_AAS
        #include "bn_hw_audio_aas.h"
    #else
        #include "bn_hw_audio_null.h"
    #endif
#endif

#endif
