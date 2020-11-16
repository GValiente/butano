/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_SPRITE_TEXT_H
#define BN_CONFIG_SPRITE_TEXT_H

/**
 * @file
 * Sprite text configuration header file.
 *
 * @ingroup text
 */

#include "bn_common.h"

/**
 * @def BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS
 *
 * Specifies the maximum number of UTF-8 characters that are supported by a bn::sprite_font.
 *
 * @ingroup text
 */
#ifndef BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS
    #define BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS 64
#endif

#endif
