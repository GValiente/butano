/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_CAMERA_H
#define BN_CONFIG_CAMERA_H

/**
 * @file
 * Cameras configuration header file.
 *
 * @ingroup camera
 */

#include "bn_common.h"

/**
 * @def BN_CFG_CAMERA_MAX_ITEMS
 *
 * Specifies the maximum number of cameras that can be created with bn::camera_ptr static constructors.
 *
 * @ingroup camera
 */
#ifndef BN_CFG_CAMERA_MAX_ITEMS
    #define BN_CFG_CAMERA_MAX_ITEMS 4
#endif

#endif
