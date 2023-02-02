/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SRAM_WAIT_STATE_H
#define BN_SRAM_WAIT_STATE_H

/**
 * @file
 * Available SRAM wait states header file.
 *
 * @ingroup sram
 */

#include "bn_common.h"

/**
 * @def BN_SRAM_WAIT_STATE_4
 *
 * SRAM access time is 4+1 clock cycles.
 *
 * @ingroup sram
 */
#define BN_SRAM_WAIT_STATE_4   0x0000

/**
 * @def BN_SRAM_WAIT_STATE_3
 *
 * SRAM access time is 3+1 clock cycles.
 *
 * @ingroup sram
 */
#define BN_SRAM_WAIT_STATE_3   0x0001

/**
 * @def BN_SRAM_WAIT_STATE_2
 *
 * SRAM access time is 2+1 clock cycles.
 *
 * @ingroup sram
 */
#define BN_SRAM_WAIT_STATE_2   0x0002

/**
 * @def BN_SRAM_WAIT_STATE_8
 *
 * SRAM access time is 8+1 clock cycles.
 *
 * @ingroup sram
 */
#define BN_SRAM_WAIT_STATE_8   0x0003

#endif
