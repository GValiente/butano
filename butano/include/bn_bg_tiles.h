/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_TILES_H
#define BN_BG_TILES_H

/**
 * @file
 * bn::bg_tiles header file.
 *
 * @ingroup bg
 * @ingroup tile
 */

#include "bn_common.h"

/**
 * @brief Background tiles related functions.
 *
 * @ingroup bg
 * @ingroup tile
 */
namespace bn::bg_tiles
{
    /**
     * @brief Returns the number of used background tiles managed with
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
     */
    [[nodiscard]] int used_tiles_count();

    /**
     * @brief Returns the number of available background tiles that can be managed with
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
     */
    [[nodiscard]] int available_tiles_count();

    /**
     * @brief Returns the number of used background tile sets created with
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr static constructors.
     */
    [[nodiscard]] int used_blocks_count();

    /**
     * @brief Returns the number of available background tile sets that can be created with
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr static constructors.
     */
    [[nodiscard]] int available_blocks_count();

    /**
     * @brief Specifies if tile offsets are allowed to improve VRAM usage when creating
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
     */
    [[nodiscard]] bool allow_offset();

    /**
     * @brief Sets if tile offsets are allowed to improve VRAM usage when creating
     * regular_bg_tiles_ptr and affine_bg_tiles_ptr objects.
     */
    void set_allow_offset(bool allow_offset);

    /**
     * @brief Logs the current status of the background blocks manager.
     */
    void log_status();
}

#endif
