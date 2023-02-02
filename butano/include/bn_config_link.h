/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_LINK_H
#define BN_CONFIG_LINK_H

/**
 * @file
 * Link communication configuration header file.
 *
 * @ingroup link
 */

#include "bn_link_baud_rate.h"

/**
 * @def BN_CFG_LINK_BAUD_RATE
 *
 * Specifies transmission rate in bps.
 *
 * Values not specified in BN_LINK_BAUD_RATE_* macros are not allowed.
 *
 * @ingroup link
 */
#ifndef BN_CFG_LINK_BAUD_RATE
    #define BN_CFG_LINK_BAUD_RATE BN_LINK_BAUD_RATE_9600_BPS
#endif

/**
 * @def BN_CFG_LINK_SEND_WAIT
 *
 * Specifies how much time the GBA has to wait before sending pending messages.
 *
 * If this parameter is too low, some messages will be lost,
 * but if it is too high, CPU usage will increase a lot.
 *
 * @ingroup link
 */
#ifndef BN_CFG_LINK_SEND_WAIT
    #define BN_CFG_LINK_SEND_WAIT 50
#endif

/**
 * @def BN_CFG_LINK_MAX_MESSAGES
 *
 * Specifies the maximum number of stored messages for each player.
 *
 * @ingroup link
 */
#ifndef BN_CFG_LINK_MAX_MESSAGES
    #define BN_CFG_LINK_MAX_MESSAGES 8
#endif

/**
 * @def BN_CFG_LINK_MAX_MISSING_MESSAGES
 *
 * Specifies the maximum number of missing messages before resetting the communication.
 *
 * @ingroup link
 */
#ifndef BN_CFG_LINK_MAX_MISSING_MESSAGES
    #define BN_CFG_LINK_MAX_MISSING_MESSAGES 4
#endif

#endif
