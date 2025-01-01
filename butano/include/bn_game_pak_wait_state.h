/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GAME_PAK_WAIT_STATE_H
#define BN_GAME_PAK_WAIT_STATE_H

/**
 * @file
 * Available Game Pak wait states header file.
 *
 * @ingroup game_pak
 */

#include "bn_common.h"

/**
 * @def BN_GAME_PAK_WAIT_STATE_FIRST_4
 *
 * Game Pak access time is 4+1 clock cycles for non-sequential accesses.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_FIRST_4     0x0000

/**
 * @def BN_GAME_PAK_WAIT_STATE_FIRST_3
 *
 * Game Pak access time is 3+1 clock cycles for non-sequential accesses.
 *
 * This setting is not supported by slow flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_FIRST_3     0x0004

/**
 * @def BN_GAME_PAK_WAIT_STATE_FIRST_2
 *
 * Game Pak access time is 2+1 clock cycles for non-sequential accesses.
 *
 * This setting is not supported by the Nintendo 3DS or slow flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_FIRST_2     0x0008

/**
 * @def BN_GAME_PAK_WAIT_STATE_FIRST_8
 *
 * Game Pak access time is 8+1 clock cycles for non-sequential accesses.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_FIRST_8     0x000C

/**
 * @def BN_GAME_PAK_WAIT_STATE_FIRST_AUTO
 *
 * Game Pak access time is 4+1 clock cycles for non-sequential accesses
 * if a slow flash cart like the SuperCard SD has been detected; 3+1 otherwise.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_FIRST_AUTO   (-1)

/**
 * @def BN_GAME_PAK_WAIT_STATE_SECOND_2
 *
 * Game Pak access time is 2+1 clock cycles for sequential accesses.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_SECOND_2    0x0000

/**
 * @def BN_GAME_PAK_WAIT_STATE_SECOND_1
 *
 * Game Pak access time is 1+1 clock cycles for sequential accesses.
 *
 * This setting is not supported by slow flash carts like the SuperCard SD.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_SECOND_1    0x0010

/**
 * @def BN_GAME_PAK_WAIT_STATE_SECOND_AUTO
 *
 * Game Pak access time is 2+1 clock cycles for sequential accesses
 * if a slow flash cart like the SuperCard SD has been detected; 1+1 otherwise.
 *
 * @ingroup game_pak
 */
#define BN_GAME_PAK_WAIT_STATE_SECOND_AUTO  (-1)

#endif
