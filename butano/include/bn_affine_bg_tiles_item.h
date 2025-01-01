/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_TILES_ITEM_H
#define BN_AFFINE_BG_TILES_ITEM_H

/**
 * @file
 * bn::bg_affine_tiles_item header file.
 *
 * @ingroup affine_bg
 * @ingroup tile
 * @ingroup tool
 */

#include "bn_span.h"
#include "bn_tile.h"
#include "bn_optional.h"
#include "bn_compression_type.h"

namespace bn
{

class affine_bg_tiles_ptr;

/**
 * @brief Contains the required information to generate affine background tiles.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `affine_bg` type.
 *
 * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
 * to avoid dangling references.
 *
 * @ingroup affine_bg
 * @ingroup tile
 * @ingroup tool
 */
class affine_bg_tiles_item
{

public:
    /**
     * @brief Indicates if the specified tiles count are valid or not.
     */
    [[nodiscard]] constexpr static bool valid_tiles_count(int tiles_count)
    {
        return tiles_count && tiles_count <= 256 * 2;
    }

    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
     * to avoid dangling references.
     */
    constexpr explicit affine_bg_tiles_item(const span<const tile>& tiles_ref) :
        affine_bg_tiles_item(tiles_ref, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
     * to avoid dangling references.
     *
     * @param compression Compression type.
     */
    constexpr affine_bg_tiles_item(const span<const tile>& tiles_ref, compression_type compression) :
        _tiles_ref(tiles_ref),
        _compression(compression)
    {
        BN_ASSERT(valid_tiles_count(tiles_ref.size()), "Invalid tiles count: ", _tiles_ref.size());
    }

    /**
     * @brief Returns the compression type.
     */
    [[nodiscard]] constexpr compression_type compression() const
    {
        return _compression;
    }

    /**
     * @brief Decompresses the stored data in the tiles referenced by decompressed_tiles_ref.
     *
     * If the source and destination tiles overlap, the behavior is undefined.
     *
     * @param decompressed_tiles_ref Destination of the decompressed tiles.
     * @return An affine_bg_tiles_item pointing to the decompressed tiles.
     */
    [[nodiscard]] affine_bg_tiles_item decompress(span<tile> decompressed_tiles_ref) const;

    /**
     * @brief Returns the reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    /**
     * @brief Searches for an affine_bg_tiles_ptr which reference the background tiles.
     * @return affine_bg_tiles_ptr which reference the background tiles if they have been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_tiles_ptr> find_tiles() const;

    /**
     * @brief Searches for an affine_bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates an affine_bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return affine_bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns an affine_bg_tiles_ptr which reference them.
     */
    [[nodiscard]] affine_bg_tiles_ptr create_tiles() const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_tiles() method instead")]]
    [[nodiscard]] affine_bg_tiles_ptr create_new_tiles() const;

    /// @endcond

    /**
     * @brief Searches for an affine_bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates an affine_bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return affine_bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns a new affine_bg_tiles_ptr which reference them if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<affine_bg_tiles_ptr> create_tiles_optional() const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_tiles_optional() method instead")]]
    [[nodiscard]] optional<affine_bg_tiles_ptr> create_new_tiles_optional() const;

    /// @endcond

    /**
     * @brief Equal operator.
     * @param a First affine_bg_tiles_item to compare.
     * @param b Second affine_bg_tiles_item to compare.
     * @return `true` if the first affine_bg_tiles_item is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const affine_bg_tiles_item& a, const affine_bg_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size();
    }

private:
    span<const tile> _tiles_ref;
    compression_type _compression;
};

}

#endif
