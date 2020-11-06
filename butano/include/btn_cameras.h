/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CAMERAS_H
#define BTN_CAMERAS_H

#include "btn_common.h"

/**
 * @brief Cameras related functions.
 *
 * @ingroup camera
 */
namespace btn::cameras
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
