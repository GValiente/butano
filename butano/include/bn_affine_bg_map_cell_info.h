/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_MAP_CELL_INFO_H
#define BN_AFFINE_BG_MAP_CELL_INFO_H

/**
 * @file
 * bn::affine_bg_map_cell_info header file.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 */

#include "bn_assert.h"
#include "bn_affine_bg_map_cell.h"

namespace bn
{

/**
 * @brief Manages the properties of an affine background map cell.
 */
class affine_bg_map_cell_info
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr affine_bg_map_cell_info() = default;

    /**
     * @brief Constructor.
     * @param cell Managed affine_bg_map_cell.
     */
    constexpr explicit affine_bg_map_cell_info(affine_bg_map_cell cell) :
        _data(cell)
    {
    }

    /**
     * @brief Returns the managed affine_bg_map_cell.
     */
    [[nodiscard]] constexpr affine_bg_map_cell cell() const
    {
        return _data;
    }

    /**
     * @brief Sets the managed affine_bg_map_cell.
     */
    constexpr void set_cell(affine_bg_map_cell cell)
    {
        _data = cell;
    }

    /**
     * @brief Returns the referenced tile index.
     */
    [[nodiscard]] constexpr int tile_index() const
    {
        return int(_data);
    }

    /**
     * @brief Sets the referenced tile index.
     */
    constexpr void set_tile_index(int tile_index)
    {
        BN_ASSERT(tile_index >= 0 && tile_index < 256, "Invalid tile index: ", tile_index);

        _data = affine_bg_map_cell(tile_index);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(affine_bg_map_cell_info a, affine_bg_map_cell_info b) = default;

private:
    affine_bg_map_cell _data = 0;
};

}

#endif
