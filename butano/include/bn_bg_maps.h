/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_common.h"

namespace bn
{
    enum class affine_bg_big_map_canvas_size : uint8_t;
}

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

    /**
     * @brief Returns the size of the canvas used to create big affine background maps.
     */
    [[nodiscard]] affine_bg_big_map_canvas_size new_affine_big_map_canvas_size();

    /**
     * @brief Sets the size of the canvas used to create big affine background maps.
     */
    void set_new_affine_big_map_canvas_size(affine_bg_big_map_canvas_size affine_big_map_canvas_size);

    /**
     * @brief Logs the current status of the background blocks manager.
     */
    void log_status();
}

#endif
