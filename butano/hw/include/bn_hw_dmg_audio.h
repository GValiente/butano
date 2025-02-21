/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DMG_AUDIO_H
#define BN_HW_DMG_AUDIO_H

#ifdef BN_DMG_AUDIO_BACKEND_DEFAULT
    #include "bn_hw_dmg_audio_default.h"
#else
    #include "bn_hw_dmg_audio_null.h"
#endif

#endif
