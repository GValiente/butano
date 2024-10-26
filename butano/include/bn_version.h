/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_VERSION_H
#define BN_VERSION_H

/**
 * @file
 * bn::version header file.
 *
 * @ingroup other
 */

#include "bn_common.h"

/**
 * @def BN_VERSION_MAJOR
 *
 * Specifies the current major version of Butano.
 *
 * @ingroup other
 */
#ifndef BN_VERSION_MAJOR
    #define BN_VERSION_MAJOR 18
#endif

/**
 * @def BN_VERSION_MINOR
 *
 * Specifies the current minor version of Butano.
 *
 * @ingroup other
 */
#ifndef BN_VERSION_MINOR
    #define BN_VERSION_MINOR 2
#endif

/**
 * @def BN_VERSION_PATCH
 *
 * Specifies the current patch version of Butano.
 *
 * @ingroup other
 */
#ifndef BN_VERSION_PATCH
    #define BN_VERSION_PATCH 1
#endif

/**
 * @def BN_VERSION_STRING
 *
 * Specifies the current version of Butano in string format.
 *
 * @ingroup other
 */
#ifndef BN_VERSION_STRING
    #define BN_VERSION_STRING "18.2.1"
#endif

/**
 * @brief Current version of Butano.
 *
 * @ingroup other
 */
namespace bn::version
{
    /**
     * @brief Returns the current major version of Butano.
     */
    [[nodiscard]] constexpr int major()
    {
        return BN_VERSION_MAJOR;
    }

    /**
     * @brief Returns the current minor version of Butano.
     */
    [[nodiscard]] constexpr int minor()
    {
        return BN_VERSION_MINOR;
    }

    /**
     * @brief Returns the current patch version of Butano.
     */
    [[nodiscard]] constexpr int patch()
    {
        return BN_VERSION_PATCH;
    }
}

#endif
