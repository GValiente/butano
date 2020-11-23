/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * @brief Current version of butano.
 *
 * @ingroup other
 */
namespace bn::version
{
    /**
     * @brief Returns the current major version.
     */
    [[nodiscard]] constexpr int major()
    {
        return 3;
    }

    /**
     * @brief Returns the current minor version.
     */
    [[nodiscard]] constexpr int minor()
    {
        return 2;
    }

    /**
     * @brief Returns the current patch version.
     */
    [[nodiscard]] constexpr int patch()
    {
        return 0;
    }
}

#endif
