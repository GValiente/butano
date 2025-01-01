/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_MAP_ITEM_H
#define BN_AFFINE_BG_MAP_ITEM_H

/**
 * @file
 * bn::affine_bg_map_item header file.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup tool
 */

#include "bn_size.h"
#include "bn_point.h"
#include "bn_optional.h"
#include "bn_span_fwd.h"
#include "bn_compression_type.h"
#include "bn_affine_bg_map_cell.h"

namespace bn
{

class bg_palette_ptr;
class affine_bg_map_ptr;
class affine_bg_tiles_ptr;

/**
 * @brief Contains the required information to generate affine background maps.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `affine_bg` type.
 *
 * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
 * to avoid dangling references.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup tool
 */
class affine_bg_map_item
{

public:
    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more affine background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of the referenced map cells.
     */
    constexpr affine_bg_map_item(const affine_bg_map_cell& cells_ref, const size& dimensions) :
        affine_bg_map_item(cells_ref, dimensions, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more affine background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of each referenced map.
     * @param maps_count Number of maps contained in cells_ref.
     */
    constexpr affine_bg_map_item(const affine_bg_map_cell& cells_ref, const size& dimensions, int maps_count) :
        affine_bg_map_item(cells_ref, dimensions, compression_type::NONE, maps_count)
    {
    }

    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more affine background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of the referenced map cells.
     * @param compression Compression type.
     */
    constexpr affine_bg_map_item(
            const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions),
        _maps_count(1),
        _compression(compression),
        _big(_big_dimensions(dimensions))
    {
        BN_ASSERT((dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.width() >= 32 && dimensions.width() <= 2048 && dimensions.width() % 32 == 0),
                  "Invalid width: ", dimensions.width());
        BN_ASSERT((dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.height() >= 32 && dimensions.height() <= 2048 && dimensions.height() % 32 == 0),
                  "Invalid height: ", dimensions.height());
    }

    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more affine background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of each referenced map.
     * @param compression Compression type.
     * @param maps_count Number of maps contained in cells_ref.
     */
    constexpr affine_bg_map_item(
            const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression,
            int maps_count) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions),
        _maps_count(uint16_t(maps_count)),
        _compression(compression),
        _big(_big_dimensions(dimensions))
    {
        BN_ASSERT((dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.width() >= 32 && dimensions.width() <= 2048 && dimensions.width() % 32 == 0),
                  "Invalid width: ", dimensions.width());
        BN_ASSERT((dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.height() >= 32 && dimensions.height() <= 2048 && dimensions.height() % 32 == 0),
                  "Invalid height: ", dimensions.height());
        BN_ASSERT(maps_count > 0 && maps_count < 65536, "Invalid maps count: ", maps_count);
    }

    /**
     * @brief Constructor.
     * @param cells_ref Reference to one or more affine background map cells.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param dimensions Size in map cells of each referenced map.
     * @param compression Compression type.
     * @param maps_count Number of maps contained in cells_ref.
     * @param big Indicates if maps generated with this item are big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    constexpr affine_bg_map_item(
            const affine_bg_map_cell& cells_ref, const size& dimensions, compression_type compression,
            int maps_count, bool big) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions),
        _maps_count(uint16_t(maps_count)),
        _compression(compression),
        _big(big)
    {
        BN_ASSERT((! big && dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.width() >= 32 && dimensions.width() <= 2048 && dimensions.width() % 32 == 0),
                  "Invalid width: ", dimensions.width());
        BN_ASSERT((! big && dimensions.width() == 16 && dimensions.height() == 16) ||
                  (dimensions.height() >= 32 && dimensions.height() <= 2048 && dimensions.height() % 32 == 0),
                  "Invalid height: ", dimensions.height());
        BN_ASSERT(! big || dimensions.width() > 32 || dimensions.height() > 32,
                  "Too small for a big map: ", dimensions.width(), " - ", dimensions.height());
        BN_ASSERT(maps_count > 0 && maps_count < 65536, "Invalid maps count: ", maps_count);
    }

    /**
     * @brief Returns a pointer to the referenced map cells of the first map.
     */
    [[nodiscard]] constexpr const affine_bg_map_cell* cells_ptr() const
    {
        return _cells_ptr;
    }

    /**
     * @brief Returns a pointer to the referenced map cells of the map indicated by map_index.
     */
    [[nodiscard]] constexpr const affine_bg_map_cell* cells_ptr(int map_index) const
    {
        BN_BASIC_ASSERT(compression() == compression_type::NONE || _maps_count == 1,
                        "Compressed items with multiple maps not supported: ", _maps_count);
        BN_ASSERT(map_index >= 0 && map_index < _maps_count, "Invalid map index: ", map_index, " - ", _maps_count);

        return _cells_ptr + (map_index * _dimensions.width() * _dimensions.height());
    }

    /**
     * @brief Returns the referenced map cells of the first map.
     */
    [[nodiscard]] constexpr const affine_bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    /**
     * @brief Returns the referenced map cells of the map indicated by map_index.
     */
    [[nodiscard]] constexpr const affine_bg_map_cell& cells_ref(int map_index) const
    {
        return *cells_ptr(map_index);
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
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] constexpr bool big() const
    {
        return _big;
    }

    /**
     * @brief Returns the number of referenced maps.
     */
    [[nodiscard]] constexpr int maps_count() const
    {
        return _maps_count;
    }

    /**
     * @brief Returns the number of referenced map cells.
     */
    [[nodiscard]] constexpr int cells_count() const
    {
        return _dimensions.width() * _dimensions.height() * _maps_count;
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
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param map_x Horizontal position of the map cell [0..dimensions().width()).
     * @param map_y Vertical position of the map cell [0..dimensions().height()).
     * @return The index of the referenced map cell.
     */
    [[nodiscard]] constexpr int cell_index(int map_x, int map_y) const
    {
        int width = _dimensions.width();
        BN_ASSERT(map_x >= 0 && map_x < width, "Invalid map x: ", map_x, " - ", width);
        BN_ASSERT(map_y >= 0 && map_y < _dimensions.height(), "Invalid map y: ", map_y, " - ", _dimensions.height());
        BN_BASIC_ASSERT(_compression == compression_type::NONE, "Compressed maps not supported");

        return (map_y * width) + map_x;
    }

    /**
     * @brief Returns the index of the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
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
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param map_x Horizontal position of the map cell [0..dimensions().width()).
     * @param map_y Vertical position of the map cell [0..dimensions().height()).
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr affine_bg_map_cell cell(int map_x, int map_y) const
    {
        return _cells_ptr[cell_index(map_x, map_y)];
    }

    /**
     * @brief Returns the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param map_index Referenced map index.
     * @param map_x Horizontal position of the map cell [0..dimensions().width()).
     * @param map_y Vertical position of the map cell [0..dimensions().height()).
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr affine_bg_map_cell cell(int map_index, int map_x, int map_y) const
    {
        BN_ASSERT(map_index >= 0 && map_index < _maps_count, "Invalid map index: ", map_index, " - ", _maps_count);

        int base_index = map_index * _dimensions.width() * _dimensions.height();
        return _cells_ptr[base_index + cell_index(map_x, map_y)];
    }

    /**
     * @brief Returns the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param map_position Position of the map cell.
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr affine_bg_map_cell cell(const point& map_position) const
    {
        return cell(map_position.x(), map_position.y());
    }

    /**
     * @brief Returns the referenced map cell in the specified map coordinates.
     *
     * The map cells are not copied but referenced, so they should outlive the affine_bg_map_item
     * to avoid dangling references.
     *
     * @param map_index Referenced map index.
     * @param map_position Position of the map cell.
     * @return The referenced map cell.
     */
    [[nodiscard]] constexpr affine_bg_map_cell cell(int map_index, const point& map_position) const
    {
        return cell(map_index, map_position.x(), map_position.y());
    }

    /**
     * @brief Decompresses the stored data in the map cells referenced by decompressed_cells_ref.
     *
     * If the source and destination map cells overlap, the behavior is undefined.
     *
     * @param decompressed_cells_ref Destination of the decompressed map cells.
     * @return An affine_bg_map_item pointing to the decompressed map cells.
     */
    [[nodiscard]] affine_bg_map_item decompress(span<affine_bg_map_cell> decompressed_cells_ref) const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call the other decompress() method instead")]]
    [[nodiscard]] affine_bg_map_item decompress(
            affine_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const;

    /// @endcond

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> find_map(
            const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const;

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @param map_index Index of the referenced map to search.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> find_map(
            const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette, int map_index) const;

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] affine_bg_map_ptr create_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] affine_bg_map_ptr create_map(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_map() method instead")]]
    [[nodiscard]] affine_bg_map_ptr create_new_map(affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    [[deprecated("Call create_map() method instead")]]
    [[nodiscard]] affine_bg_map_ptr create_new_map(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const;

    /// @endcond

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> create_map_optional(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    /**
     * @brief Searches for an affine_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_map_ptr> create_map_optional(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_map_optional() method instead")]]
    [[nodiscard]] optional<affine_bg_map_ptr> create_new_map_optional(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    [[deprecated("Call create_map_optional() method instead")]]
    [[nodiscard]] optional<affine_bg_map_ptr> create_new_map_optional(
            affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index) const;

    /// @endcond

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const affine_bg_map_item& a,
                                                   const affine_bg_map_item& b) = default;

private:
    const affine_bg_map_cell* _cells_ptr;
    size _dimensions;
    uint16_t _maps_count;
    compression_type _compression;
    bool _big;

    [[nodiscard]] constexpr static bool _big_dimensions(const size& dimensions)
    {
        int width = dimensions.width();
        return width != dimensions.height() || (width != 16 && width != 32 && width != 64 && width != 128);
    }
};

}

#endif
