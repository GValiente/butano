/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_LOG_H
#define BTN_CONFIG_LOG_H

/**
 * @file
 * Logging configuration header file.
 *
 * @ingroup log
 */

#include "btn_log_backend.h"

/**
 * @def BTN_CFG_LOG_ENABLED
 *
 * Specifies if logging is enabled or not.
 *
 * @ingroup log
 */
#ifndef BTN_CFG_LOG_ENABLED
    #define BTN_CFG_LOG_ENABLED true
#endif

/**
 * @def BTN_CFG_LOG_BACKEND
 *
 * Specifies if emulator logging is enabled or not.
 *
 * Values not specified in BTN_LOG_BACKEND_* macros are not allowed.
 *
 * @ingroup log
 */
#ifndef BTN_CFG_LOG_BACKEND
    #define BTN_CFG_LOG_BACKEND BTN_LOG_BACKEND_MGBA
#endif

/**
 * @def BTN_CFG_LOG_MAX_SIZE
 *
 * Specifies the maximum number of characters that can be logged with one BTN_LOG call.
 *
 * @ingroup log
 */
#ifndef BTN_CFG_LOG_MAX_SIZE
    #define BTN_CFG_LOG_MAX_SIZE 0x100
#endif

#endif
