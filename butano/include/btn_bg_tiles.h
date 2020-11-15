/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_TILES_H
#define BTN_BG_TILES_H

/**
 * @file
 * btn::bg_tiles header file.
 *
 * @ingroup bg
 * @ingroup tile
 */

#include "btn_config_log.h"
#include "btn_config_doxygen.h"

/**
 * @brief Background tiles related functions.
 *
 * @ingroup bg
 * @ingroup tile
 */
namespace btn::bg_tiles
{
    /**
     * @brief Returns the number of used background tiles managed with bg_tiles_ptr objects.
     */
    [[nodiscard]] int used_tiles_count();

    /**
     * @brief Returns the number of available background tiles that can be managed with bg_tiles_ptr objects.
     */
    [[nodiscard]] int available_tiles_count();

    /**
     * @brief Returns the number of used background tile sets created with bg_tiles_ptr static constructors.
     */
    [[nodiscard]] int used_blocks_count();

    /**
     * @brief Returns the number of available background tile sets
     * that can be created with bg_tiles_ptr static constructors.
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
