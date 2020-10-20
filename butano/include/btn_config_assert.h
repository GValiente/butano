/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_ASSERT_H
#define BTN_CONFIG_ASSERT_H

#include "btn_common.h"

/**
 * @def BTN_CFG_ASSERT_ENABLED
 *
 * Indicates if the asserts are enabled or not.
 *
 * @ingroup assert
 */
#ifndef BTN_CFG_ASSERT_ENABLED
    #define BTN_CFG_ASSERT_ENABLED true
#endif

/**
 * @def BTN_CFG_ASSERT_BUFFER_SIZE
 *
 * Specifies the maximum size of the messages that can be displayed by an assert.
 *
 * @ingroup assert
 */
#ifndef BTN_CFG_ASSERT_BUFFER_SIZE
    #define BTN_CFG_ASSERT_BUFFER_SIZE 512
#endif

#endif
