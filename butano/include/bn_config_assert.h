/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_ASSERT_H
#define BN_CONFIG_ASSERT_H

/**
 * @file
 * Asserts configuration header file.
 *
 * @ingroup assert
 */

#include "bn_common.h"

/**
 * @def BN_CFG_ASSERT_ENABLED
 *
 * Specifies if asserts must be enabled or not.
 *
 * @ingroup assert
 */
#ifndef BN_CFG_ASSERT_ENABLED
    #define BN_CFG_ASSERT_ENABLED true
#endif

/**
 * @def BN_CFG_ASSERT_SHOW_DIAGNOSTIC
 *
 * Specifies if asserts diagnostic information must be shown or not.
 *
 * Disabling diagnostic information reduces ROM size.
 *
 * @ingroup assert
 */
#ifndef BN_CFG_ASSERT_SHOW_DIAGNOSTIC
    #define BN_CFG_ASSERT_SHOW_DIAGNOSTIC true
#endif

/**
 * @def BN_CFG_ASSERT_BUFFER_SIZE
 *
 * Specifies the maximum size of the messages that can be displayed by an assert.
 *
 * @ingroup assert
 */
#ifndef BN_CFG_ASSERT_BUFFER_SIZE
    #define BN_CFG_ASSERT_BUFFER_SIZE 512
#endif

/**
 * @def BN_CFG_ASSERT_STACKTRACE_DEMANGLE
 *
 * Indicates if the stack trace logged in error messages must be demangled or not.
 *
 * Stack trace demangle requires to link standard system libraries (`DEFAULTLIBS` makefile variable).
 *
 * Expect a crash after logging the stack trace.
 *
 * @ingroup assert
 */
#ifndef BN_CFG_ASSERT_STACKTRACE_DEMANGLE
    #define BN_CFG_ASSERT_STACKTRACE_DEMANGLE false
#endif

#endif
