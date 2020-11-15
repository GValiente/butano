/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_MAPS_H
#define BTN_BG_MAPS_H

/**
 * @file
 * btn::bg_maps header file.
 *
 * @ingroup bg_map
 */

#include "btn_config_log.h"
#include "btn_config_doxygen.h"

/**
 * @brief Background maps related functions.
 *
 * @ingroup bg_map
 */
namespace btn::bg_maps
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

    #if BTN_CFG_LOG_ENABLED || BTN_DOXYGEN
        /**
         * @brief Logs the current status of the background blocks manager.
         */
        void log_status();
    #endif
}

#endif
