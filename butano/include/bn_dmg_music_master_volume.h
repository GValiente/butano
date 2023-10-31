/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_MASTER_VOLUME_H
#define BN_DMG_MUSIC_MASTER_VOLUME_H

/**
 * @file
 * bn::dmg_music_master_volume header file.
 *
 * @ingroup dmg_music
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the available DMG music master volume levels.
 *
 * @ingroup dmg_music
 */
enum class dmg_music_master_volume : uint8_t
{
    QUARTER, //!< 25%
    HALF, //!< 50%
    FULL //!< 100%
};

}

#endif

