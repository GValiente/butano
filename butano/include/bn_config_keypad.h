/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_KEYPAD_H
#define BN_CONFIG_KEYPAD_H

/**
 * @file
 * Keypad configuration header file.
 *
 * @ingroup keypad
 */

#include "bn_common.h"

/**
 * @def BN_CFG_KEYPAD_LOG_ENABLED
 *
 * Specifies if keypad logging is enabled or not.
 *
 * @ingroup keypad
 */
#ifndef BN_CFG_KEYPAD_LOG_ENABLED
    #define BN_CFG_KEYPAD_LOG_ENABLED false
#endif

#endif
