/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_LOG_H
#define BN_CONFIG_LOG_H

/**
 * @file
 * Logging configuration header file.
 *
 * @ingroup log
 */

#include "bn_log_backend.h"

/**
 * @def BN_CFG_LOG_ENABLED
 *
 * Specifies if logging is enabled or not.
 *
 * @ingroup log
 */
#ifndef BN_CFG_LOG_ENABLED
    #define BN_CFG_LOG_ENABLED true
#endif

/**
 * @def BN_CFG_LOG_BACKEND
 *
 * Specifies the log backend to use.
 *
 * Values not specified in BN_LOG_BACKEND_* macros are not allowed.
 *
 * @ingroup log
 */
#ifndef BN_CFG_LOG_BACKEND
    #define BN_CFG_LOG_BACKEND BN_LOG_BACKEND_MGBA
#endif

/**
 * @def BN_CFG_LOG_MAX_SIZE
 *
 * Specifies the maximum number of characters that can be logged with one BN_LOG call.
 *
 * @ingroup log
 */
#ifndef BN_CFG_LOG_MAX_SIZE
    #define BN_CFG_LOG_MAX_SIZE 0x100
#endif

static_assert(BN_CFG_LOG_MAX_SIZE >= 16);

#endif
