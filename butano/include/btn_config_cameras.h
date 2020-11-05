/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_CAMERA_H
#define BTN_CONFIG_CAMERA_H

#include "btn_common.h"

/**
 * @def BTN_CFG_CAMERA_MAX_ITEMS
 *
 * Specifies the maximum number of cameras that can be created with btn::camera_ptr static constructors.
 *
 * @ingroup camera
 */
#ifndef BTN_CFG_CAMERA_MAX_ITEMS
    #define BTN_CFG_CAMERA_MAX_ITEMS 4
#endif

#endif
