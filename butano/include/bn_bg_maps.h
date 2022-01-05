/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_MAPS_H
#define BN_BG_MAPS_H

/**
 * @file
 * bn::bg_maps header file.
 *
 * @ingroup bg_map
 */

#include "bn_config_log.h"
#include "bn_config_doxygen.h"

/**
 * @brief Background maps related functions.
 *
 * @ingroup bg_map
 */
namespace bn::bg_maps
{
    /**
     * @brief Returns the number of used background map cells.
     */
    [[nodiscard]] int used_cells_count();

    /**
     * @brief Returns the number of available background map cells.
     */
    [[nodiscard]] int available_cells_count();

    /**
     * @brief Returns the number of used background map cell blocks.
     */
    [[nodiscard]] int used_blocks_count();

    /**
     * @brief Returns the number of available background map cell blocks.
     */
    [[nodiscard]] int available_blocks_count();

    #if BN_CFG_LOG_ENABLED || BN_DOXYGEN
        /**
         * @brief Logs the current status of the background blocks manager.
         */
        void log_status();
    #endif
}

#endif
