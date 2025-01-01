/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_TYPE_H
#define BN_DMG_MUSIC_TYPE_H

/**
 * @file
 * bn::dmg_music_type header file.
 *
 * @ingroup dmg_music
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the available DMG music types.
 *
 * @ingroup dmg_music
 */
enum class dmg_music_type : uint8_t
{
    GBT_PLAYER, //!< GBT Player module files with `*.mod` and `*.s3m` extensions.
    VGM //!< VGM audio files with `*.vgm` extension.
};

}

#endif

