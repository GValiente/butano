/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CAMERAS_H
#define BN_CAMERAS_H

/**
 * @file
 * bn::cameras header file.
 *
 * @ingroup camera
 */

#include "bn_common.h"

/**
 * @brief Cameras related functions.
 *
 * @ingroup camera
 */
namespace bn::cameras
{
    /**
     * @brief Returns the number of used cameras managed with camera_ptr objects.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available cameras that can be managed with camera_ptr objects.
     */
    [[nodiscard]] int available_items_count();
}

#endif
