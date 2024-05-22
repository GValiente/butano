/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_RUMBLE_H
#define BN_RUMBLE_H

/**
 * @file
 * bn::rumble header file.
 *
 * @ingroup rumble
 */

#include "bn_common.h"

/**
 * @brief Rumble related functions.
 *
 * @ingroup rumble
 */
namespace bn::rumble
{
    /**
     * @brief Indicates if rumble is enabled or not.
     *
     * Butano does not know if your cart supports rumble or not,
     * so it can report that it is enabled even when it is not supported.
     */
    [[nodiscard]] bool enabled();

    /**
     * @brief Sets if rumble must be enabled or not.
     *
     * Remember that rumble only works if you cart supports it.
     */
    void set_enabled(bool enabled);
}

#endif
