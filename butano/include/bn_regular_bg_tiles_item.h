/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_TILES_ITEM_H
#define BN_REGULAR_BG_TILES_ITEM_H

/**
 * @file
 * bn::bg_regular_tiles_item header file.
 *
 * @ingroup regular_bg
 * @ingroup tile
 * @ingroup tool
 */

#include "bn_span.h"
#include "bn_tile.h"
#include "bn_bpp_mode.h"
#include "bn_optional.h"
#include "bn_compression_type.h"

namespace bn
{

class regular_bg_tiles_ptr;

/**
 * @brief Contains the required information to generate regular background tiles.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `regular_bg` type.
 *
 * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
 * to avoid dangling references.
 *
 * @ingroup regular_bg
 * @ingroup tile
 * @ingroup tool
 */
class regular_bg_tiles_item
{

public:
    /**
     * @brief Indicates if the specified tiles count are valid for the specified bits per pixel or not.
     */
    [[nodiscard]] constexpr static bool valid_tiles_count(int tiles_count, bpp_mode bpp)
    {
        if(bpp == bpp_mode::BPP_8)
        {
            return tiles_count && tiles_count <= 2048 && (tiles_count % 2) == 0;
        }

        return tiles_count && tiles_count <= 1024;
    }

    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
     * to avoid dangling references.
     *
     * @param bpp tiles_ref bits per pixel.
     */
    constexpr regular_bg_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp) :
        regular_bg_tiles_item(tiles_ref, bpp, compression_type::NONE)
    {
    }

    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
     * to avoid dangling references.
     *
     * @param bpp tiles_ref bits per pixel.
     * @param compression Compression type.
     */
    constexpr regular_bg_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp, compression_type compression) :
        _tiles_ref(tiles_ref),
        _bpp(bpp),
        _compression(compression)
    {
        BN_ASSERT(valid_tiles_count(tiles_ref.size(), bpp),
                  "Invalid tiles count: ", _tiles_ref.size(), " - ", int(bpp));
    }

    /**
     * @brief Returns the reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    /**
     * @brief Returns the bits per pixel of the referenced tiles.
     */
    [[nodiscard]] constexpr bpp_mode bpp() const
    {
        return _bpp;
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
     * @return A regular_bg_tiles_item pointing to the decompressed tiles.
     */
    [[nodiscard]] regular_bg_tiles_item decompress(span<tile> decompressed_tiles_ref) const;

    /**
     * @brief Searches for a regular_bg_tiles_ptr which reference the background tiles.
     * @return regular_bg_tiles_ptr which reference the background tiles if they have been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_tiles_ptr> find_tiles() const;

    /**
     * @brief Searches for a regular_bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates a regular_bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return regular_bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns a regular_bg_tiles_ptr which reference them.
     */
    [[nodiscard]] regular_bg_tiles_ptr create_tiles() const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_tiles() method instead")]]
    [[nodiscard]] regular_bg_tiles_ptr create_new_tiles() const;

    /// @endcond

    /**
     * @brief Searches for a regular_bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates a regular_bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return regular_bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns a new regular_bg_tiles_ptr which reference them if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_tiles_ptr> create_tiles_optional() const;

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_tiles_optional() method instead")]]
    [[nodiscard]] optional<regular_bg_tiles_ptr> create_new_tiles_optional() const;

    /// @endcond

    /**
     * @brief Equal operator.
     * @param a First regular_bg_tiles_item to compare.
     * @param b Second regular_bg_tiles_item to compare.
     * @return `true` if the first regular_bg_tiles_item is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const regular_bg_tiles_item& a, const regular_bg_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size() &&
                a._bpp == b._bpp;
    }

private:
    span<const tile> _tiles_ref;
    bpp_mode _bpp;
    compression_type _compression;
};

}

#endif
