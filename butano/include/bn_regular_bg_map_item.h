/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_MAP_ITEM_H
#define BN_REGULAR_BG_MAP_ITEM_H

/**
 * @file
 * bn::regular_bg_map_item header file.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup tool
 */

#include "bn_size.h"
#include "bn_point.h"
#include "bn_optional.h"
#include "bn_compression_type.h"
#include "bn_regular_bg_map_cell.h"

namespace bn
{

class bg_palette_ptr;
class regular_bg_map_ptr;
class regular_bg_tiles_ptr;

/**
 * @brief Contains the required information to generate regular background maps.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `regular_bg` type.
 *
 * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
 * to avoid dangling references.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup tool
 */
class regular_bg_map_item
{

public:
    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more regular background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of the referenced map cells.
     */
    constexpr regular_bg_map_item(const regular_bg_map_cell& cells_ref, const size& dimensions) :
        regular_bg_map_item(cells_ref, dimensions, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more regular background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of the referenced map cells.
     * @param compression Compression type.
     */
    constexpr regular_bg_map_item(const regular_bg_map_cell& cells_ref, const size& dimensions,
                                  compression_type compression) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions),
        _compression(compression)
    {
        BN_ASSERT(dimensions.width() >= 32 && dimensions.width() % 32 == 0,
                  "Invalid width: ", dimensions.width());
        BN_ASSERT(dimensions.height() >= 32 && dimensions.height() % 32 == 0,
                  "Invalid height: ", dimensions.height());
    }

    /**
     * @brief Returns a pointer to the referenced map cells.
     */
    [[nodiscard]] constexpr const regular_bg_map_cell* cells_ptr() const
    {
        return _cells_ptr;
    }

    /**
     * @brief Returns the referenced map cells.
     */
    [[nodiscard]] constexpr const regular_bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    /**
     * @brief Returns the size in map cells of the referenced map cells.
     */
    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    /**
     * @brief Indicates if maps generated with this item are big or not.
     *
     * Big backgrounds are slower CPU wise and don't support wrapping
     * (they can't be moved beyond their boundaries), but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] constexpr bool big() const
    {
        return _dimensions.width() > 64 || _dimensions.height() > 64;
    }

    /**
     * @brief Returns the compression type.
     */
    [[nodiscard]] constexpr compression_type compression() const
    {
        return _compression;
    }

    /**
     * @brief Returns the index of the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param map_x Horizontal position of the map cell [0..dimensions().width()).
     * @param map_y Vertical position of the map cell [0..dimensions().height()).
     * @return The index of the referenced map cell.
     */
    [[nodiscard]] constexpr int cell_index(int map_x, int map_y) const
    {
        int width = _dimensions.width();
        int height = _dimensions.height();
        BN_ASSERT(map_x >= 0 && map_x < width, "Invalid map x: ", map_x, " - ", width);
        BN_ASSERT(map_y >= 0 && map_y < height, "Invalid map y: ", map_y, " - ", height);
        BN_ASSERT(_compression == compression_type::NONE, "Compressed maps not supported");

        if((width == 32 && height == 64) || (width == 64 && height == 32) || (width == 64 && height == 64))
        {
            auto tx = unsigned(map_x);
            auto ty = unsigned(map_y);
            auto pitch = unsigned(width);
            unsigned sbb = ((ty / 32) * (pitch / 32)) + (tx / 32);
            return int((sbb * 1024) + ((ty % 32) * 32) + (tx % 32));
        }

        return (map_y * width) + map_x;
    }

    /**
     * @brief Returns the index of the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param map_position Position of the map cell.
     * @return The index of the referenced map cell.
     */
    [[nodiscard]] constexpr int cell_index(const point& map_position) const
    {
        return cell_index(map_position.x(), map_position.y());
    }

    /**
     * @brief Returns the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param map_x Horizontal position of the map cell [0..dimensions().width()).
     * @param map_y Vertical position of the map cell [0..dimensions().height()).
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr regular_bg_map_cell cell(int map_x, int map_y) const
    {
        return _cells_ptr[cell_index(map_x, map_y)];
    }

    /**
     * @brief Returns the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the regular_bg_map_item
     * to avoid dangling references.
     *
     * @param map_position Position of the map cell.
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr regular_bg_map_cell cell(const point& map_position) const
    {
        return _cells_ptr[cell_index(map_position)];
    }

    /**
     * @brief Decompresses the stored data in the map cells referenced by decompressed_cells_ref.
     *
     * If the source and destination map cells overlap, the behavior is undefined.
     *
     * @param decompressed_cells_ref Destination of the decompressed map cells.
     * @param decompressed_dimensions Size in map cells of the destination data.
     * @return A regular_bg_map_item pointing to the decompressed map cells.
     */
    [[nodiscard]] regular_bg_map_item decompress(regular_bg_map_cell& decompressed_cells_ref,
                                                 const size& decompressed_dimensions) const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call decompress() instead")]]
    [[nodiscard]] regular_bg_map_item uncompress(regular_bg_map_cell& uncompressed_cells_ref,
                                                 const size& uncompressed_dimensions) const
    {
        return decompress(uncompressed_cells_ref, uncompressed_dimensions);
    }

    /// @endcond

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> find_map(
            const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] regular_bg_map_ptr create_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Creates a regular_bg_map_ptr which references the information provided by this item.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the information provided by this item is already referenced or not,
     * you should use the create_map method instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return regular_bg_map_ptr which references the information provided by this item.
     */
    [[nodiscard]] regular_bg_map_ptr create_new_map(regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_map_optional(
            regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Creates a regular_bg_map_ptr which references the information provided by this item.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the information provided by this item is already referenced or not,
     * you should use the create_map_optional method instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return regular_bg_map_ptr which references the information provided by this item
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_new_map_optional(
            regular_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const regular_bg_map_item& a,
                                                   const regular_bg_map_item& b) = default;

private:
    const regular_bg_map_cell* _cells_ptr;
    size _dimensions;
    compression_type _compression;
};

}

#endif
