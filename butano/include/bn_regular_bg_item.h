/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_ITEM_H
#define BN_REGULAR_BG_ITEM_H

/**
 * @file
 * bn::regular_bg_item header file.
 *
 * @ingroup regular_bg
 * @ingroup tool
 */

#include "bn_fixed_point.h"
#include "bn_bg_palette_item.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_tiles_item.h"

namespace bn
{

class regular_bg_ptr;

/**
 * @brief Contains the required information to generate regular backgrounds and their maps.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `regular_bg` type.
 *
 * Tiles, colors and map cells are not copied but referenced,
 * so they should outlive the regular_bg_item to avoid dangling references.
 *
 * @ingroup regular_bg
 * @ingroup tool
 */
class regular_bg_item
{

public:
    /**
     * @brief Constructor.
     * @param tiles_ref Reference to regular background tiles.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the regular_bg_item to avoid dangling references.
     *
     * @param colors_ref Reference to an array of multiples of 16 colors.
     *
     * The colors are not copied but referenced,
     * so they should outlive the regular_bg_item to avoid dangling references.
     *
     * @param bpp Bits per pixel of referenced colors.
     * @param map_cells_ref Regular background map cells to reference.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_item to avoid dangling references.
     *
     * @param map_dimensions Size in map cells of the referenced map cells.
     */
    constexpr regular_bg_item(const span<const tile>& tiles_ref, const span<const color>& colors_ref,
                              bpp_mode bpp, const regular_bg_map_cell& map_cells_ref, const size& map_dimensions) :
        regular_bg_item(regular_bg_tiles_item(tiles_ref, bpp), bg_palette_item(colors_ref, bpp),
                        regular_bg_map_item(map_cells_ref, map_dimensions))
    {
    }

    /**
     * @brief Constructor.
     * @param tiles_item It creates the tiles of the output regular backgrounds.
     * @param palette_item It creates the color palette of the output regular backgrounds.
     * @param map_item It creates the map of the output regular backgrounds.
     */
    constexpr regular_bg_item(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item,
                              const regular_bg_map_item& map_item) :
        _tiles_item(tiles_item),
        _palette_item(palette_item),
        _map_item(map_item)
    {
        BN_BASIC_ASSERT(tiles_item.bpp() == palette_item.bpp(), "Tiles and palette BPP are different");
    }

    /**
     * @brief Returns the item used to create the tiles of the output regular backgrounds.
     */
    [[nodiscard]] constexpr const regular_bg_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    /**
     * @brief Returns the item used to create the color palette of the output regular backgrounds.
     */
    [[nodiscard]] constexpr const bg_palette_item& palette_item() const
    {
        return _palette_item;
    }

    /**
     * @brief Returns the item used to create the map of the output regular backgrounds.
     */
    [[nodiscard]] constexpr const regular_bg_map_item& map_item() const
    {
        return _map_item;
    }

    /**
     * @brief Indicates if backgrounds generated with this item are big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] constexpr bool big() const
    {
        return _map_item.big();
    }

    /**
     * @brief Decompresses the stored data in the tiles referenced by decompressed_tiles_ref,
     * the colors referenced by decompressed_colors_ref and the map cells referenced by decompressed_cells_ref.
     *
     * If the source and destination tiles, colors or map cells overlap, the behavior is undefined.
     *
     * @param decompressed_tiles_ref Destination of the decompressed tiles.
     * @param decompressed_colors_ref Destination of the decompressed colors.
     * @param decompressed_cells_ref Destination of the decompressed map cells.
     * @return A regular_bg_item pointing to the decompressed tiles, colors and map cells.
     */
    [[nodiscard]] regular_bg_item decompress(
            span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref,
            span<regular_bg_map_cell> decompressed_cells_ref) const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call the other decompress() method instead")]]
    [[nodiscard]] regular_bg_item decompress(
            span<tile> decompressed_tiles_ref, span<color> decompressed_colors_ref,
            regular_bg_map_cell& decompressed_cells_ref, const size& decompressed_dimensions) const;

    /// @endcond

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] regular_bg_ptr create_bg(fixed x, fixed y) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param map_index Index of the map to reference in map_item().
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] regular_bg_ptr create_bg(fixed x, fixed y, int map_index) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param position Position of the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] regular_bg_ptr create_bg(const fixed_point& position) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param position Position of the regular background.
     * @param map_index Index of the map to reference in map_item().
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] regular_bg_ptr create_bg(const fixed_point& position, int map_index) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_ptr> create_bg_optional(fixed x, fixed y) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param map_index Index of the map to reference in map_item().
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_ptr> create_bg_optional(fixed x, fixed y, int map_index) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param position Position of the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_ptr> create_bg_optional(const fixed_point& position) const;

    /**
     * @brief Creates a regular_bg_ptr using the information contained in this item.
     * @param position Position of the regular background.
     * @param map_index Index of the map to reference in map_item().
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_ptr> create_bg_optional(const fixed_point& position, int map_index) const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> find_map() const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * @param map_index Index of the referenced map to search.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> find_map(int map_index) const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] regular_bg_map_ptr create_map() const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] regular_bg_map_ptr create_map(int map_index) const;

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
     * @return regular_bg_map_ptr which references the information provided by this item.
     */
    [[nodiscard]] regular_bg_map_ptr create_new_map() const;

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
     * @param map_index Index of the referenced map to handle.
     * @return regular_bg_map_ptr which references the information provided by this item.
     */
    [[nodiscard]] regular_bg_map_ptr create_new_map(int map_index) const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_map_optional() const;

    /**
     * @brief Searches for a regular_bg_map_ptr which references the information provided by this item.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the information provided by this item if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_map_optional(int map_index) const;

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
     * @return regular_bg_map_ptr which references the information provided by this item
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_new_map_optional() const;

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
     * @param map_index Index of the referenced map to handle.
     * @return regular_bg_map_ptr which references the information provided by this item
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> create_new_map_optional(int map_index) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const regular_bg_item& a, const regular_bg_item& b) = default;

private:
    regular_bg_tiles_item _tiles_item;
    bg_palette_item _palette_item;
    regular_bg_map_item _map_item;
};

}

#endif

