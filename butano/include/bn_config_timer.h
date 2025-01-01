/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_TIMER_H
#define BN_CONFIG_TIMER_H

/**
 * @file
 * Timer configuration header file.
 *
 * @ingroup timer
 */

#include "bn_common.h"

/**
 * @def BN_CFG_TIMER_FREQUENCY
 *
 * Specifies timer frequency in clock cycles.
 *
 * Allowed values are 1, 64, 256 and 1024.
 *
 * @ingroup timer
 */
#ifndef BN_CFG_TIMER_FREQUENCY
    #define BN_CFG_TIMER_FREQUENCY 64
#endif

#endif
