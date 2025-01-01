/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_EWRAM_WAIT_STATE_H
#define BN_EWRAM_WAIT_STATE_H

/**
 * @file
 * Available EWRAM wait states header file.
 *
 * @ingroup memory
 */

#include "bn_common.h"

/**
 * @def BN_EWRAM_WAIT_STATE_2
 *
 * EWRAM access time is 2+1 clock cycles for sequential accesses.
 *
 * @ingroup memory
 */
#define BN_EWRAM_WAIT_STATE_2   2

/**
 * @def BN_EWRAM_WAIT_STATE_1
 *
 * EWRAM access time is 1+1 clock cycles for sequential accesses.
 *
 * This wait states setting is not supported by the GBA Micro.
 * Enabling it will not crash the GBA Micro though, it will just fall back to @ref BN_EWRAM_WAIT_STATE_2.
 *
 * Some GBA SPs will crash after a few frames with this setting.
 *
 * @ingroup memory
 */
#define BN_EWRAM_WAIT_STATE_1   1

#endif
