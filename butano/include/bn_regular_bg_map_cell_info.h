/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_MAP_CELL_INFO_H
#define BN_REGULAR_BG_MAP_CELL_INFO_H

/**
 * @file
 * bn::regular_bg_map_cell_info header file.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 */

#include "bn_assert.h"
#include "bn_regular_bg_map_cell.h"

namespace bn
{

/**
 * @brief Manages the properties of a regular background map cell.
 */
class regular_bg_map_cell_info
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr regular_bg_map_cell_info() = default;

    /**
     * @brief Constructor.
     * @param cell Managed regular_bg_map_cell.
     */
    constexpr explicit regular_bg_map_cell_info(regular_bg_map_cell cell) :
        _data(cell)
    {
    }

    /**
     * @brief Returns the managed regular_bg_map_cell.
     */
    [[nodiscard]] constexpr regular_bg_map_cell cell() const
    {
        return _data;
    }

    /**
     * @brief Sets the managed regular_bg_map_cell.
     */
    constexpr void set_cell(regular_bg_map_cell cell)
    {
        _data = cell;
    }

    /**
     * @brief Returns the referenced tile index.
     */
    [[nodiscard]] constexpr int tile_index() const
    {
        return int(_fields.tile_index);
    }

    /**
     * @brief Sets the referenced tile index.
     */
    constexpr void set_tile_index(int tile_index)
    {
        BN_ASSERT(tile_index >= 0 && tile_index < 1024, "Invalid tile index: ", tile_index);

        _fields.tile_index = uint16_t(tile_index);
    }

    /**
     * @brief Returns the referenced palette id.
     */
    [[nodiscard]] constexpr int palette_id() const
    {
        return int(_fields.palette_id);
    }

    /**
     * @brief Sets the referenced palette id.
     */
    constexpr void set_palette_id(int palette_id)
    {
        BN_ASSERT(palette_id >= 0 && palette_id < 16, "Invalid palette id: ", palette_id);

        _fields.palette_id = uint8_t(palette_id);
    }

    /**
     * @brief Indicates if the managed cell is flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const
    {
        return _fields.horizontal_flip;
    }

    /**
     * @brief Sets if the managed cell is flipped in the horizontal axis or not.
     */
    void set_horizontal_flip(bool horizontal_flip)
    {
        _fields.horizontal_flip = horizontal_flip;
    }

    /**
     * @brief Indicates if the managed cell is flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const
    {
        return _fields.vertical_flip;
    }

    /**
     * @brief Sets if the managed cell is flipped in the vertical axis or not.
     */
    void set_vertical_flip(bool vertical_flip)
    {
        _fields.vertical_flip = vertical_flip;
    }

    /**
     * @brief Equal operator.
     * @param a First regular_bg_map_cell_info to compare.
     * @param b Second regular_bg_map_cell_info to compare.
     * @return `true` if the first managed cell is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(regular_bg_map_cell_info a, regular_bg_map_cell_info b)
    {
        return a._data == b._data;
    }

    /**
     * @brief Not equal operator.
     * @param a First regular_bg_map_cell_info to compare.
     * @param b Second regular_bg_map_cell_info to compare.
     * @return `true` if the first managed cell is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(regular_bg_map_cell_info a, regular_bg_map_cell_info b)
    {
        return a._data != b._data;
    }

private:
    union
    {
       struct
       {
           uint16_t tile_index:     10;
           uint8_t horizontal_flip: 1;
           uint8_t vertical_flip:   1;
           uint8_t palette_id:      4;
       } _fields;
       regular_bg_map_cell _data = 0;
    };
};

}

#endif
