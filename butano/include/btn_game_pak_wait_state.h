/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GAME_PAK_WAIT_STATE_H
#define BTN_GAME_PAK_WAIT_STATE_H

/**
 * @file
 * Available Game Pak wait states header file.
 *
 * @ingroup game_pak
 */

#include "btn_common.h"

/**
 * @def BTN_GAME_PAK_WAIT_STATE_FIRST_4
 *
 * Game Pak access time is 4+1 clock cycles for non-sequential accesses.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_FIRST_4     0x0000

/**
 * @def BTN_GAME_PAK_WAIT_STATE_FIRST_3
 *
 * Game Pak access time is 3+1 clock cycles for non-sequential accesses.
 *
 * This wait state is not supported by older flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_FIRST_3     0x0004

/**
 * @def BTN_GAME_PAK_WAIT_STATE_FIRST_2
 *
 * Game Pak access time is 2+1 clock cycles for non-sequential accesses.
 *
 * This wait state is not supported by the Nintendo 3DS nor by older flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_FIRST_2     0x0008

/**
 * @def BTN_GAME_PAK_WAIT_STATE_FIRST_8
 *
 * Game Pak access time is 8+1 clock cycles for non-sequential accesses.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_FIRST_8     0x000C

/**
 * @def BTN_GAME_PAK_WAIT_STATE_SECOND_2
 *
 * Game Pak access time is 2+1 clock cycles for sequential accesses.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_SECOND_2    0x0000

/**
 * @def BTN_GAME_PAK_WAIT_STATE_SECOND_1
 *
 * Game Pak access time is 1+1 clock cycles for sequential accesses.
 *
 * This wait state is not supported by older flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BTN_GAME_PAK_WAIT_STATE_SECOND_1    0x0010

#endif
