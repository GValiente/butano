/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CORE_LOCK_H
#define BN_CORE_LOCK_H

/**
 * @file
 * bn::core_lock header file.
 *
 * @ingroup core
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief All Butano subsystems (including interrupts and HDMA) are disabled
 * as long as an object of this class is active.
 *
 * If a Butano function or method is called while a core_lock object is alive, the behavior is undefined.
 *
 * @ingroup core
 */
class core_lock
{

public:
    /**
     * @brief Disables all Butano subsystems (including interrupts and HDMA).
     *
     * If a Butano function or method is called while a core_lock object is alive, the behavior is undefined.
     */
    core_lock();

    core_lock(const core_lock& other) = delete;

    core_lock& operator=(const core_lock& other) = delete;

    /**
     * @brief Restores all Butano subsystems (including interrupts and HDMA).
     */
    ~core_lock();
};

}

#endif
