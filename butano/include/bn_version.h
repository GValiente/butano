/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_VERSION_H
#define BTN_VERSION_H

/**
 * @file
 * btn::version header file.
 *
 * @ingroup other
 */

#include "btn_common.h"

/**
 * @brief Current version of butano.
 *
 * @ingroup other
 */
namespace btn::version
{
    /**
     * @brief Returns the current major version.
     */
    [[nodiscard]] constexpr int major()
    {
        return 0;
    }

    /**
     * @brief Returns the current minor version.
     */
    [[nodiscard]] constexpr int minor()
    {
        return 3;
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
