/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_TILES_ITEM_H
#define BN_BG_TILES_ITEM_H

/**
 * @file
 * bn::bg_tiles_item header file.
 *
 * @ingroup bg
 * @ingroup tile
 * @ingroup tool
 */

#include "bn_span.h"
#include "bn_tile.h"
#include "bn_bpp_mode.h"
#include "bn_optional_fwd.h"

namespace bn
{

class bg_tiles_ptr;

/**
 * @brief Contains the required information to generate background tiles.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `regular_bg` type.
 *
 * The tiles are not copied but referenced, so they should outlive the bg_tiles_item to avoid dangling references.
 *
 * @ingroup bg
 * @ingroup tile
 * @ingroup tool
 */
class bg_tiles_item
{

public:
    /**
     * @brief Indicates if the specified tiles count are valid for the specified bits per pixel or not.
     */
    [[nodiscard]] constexpr static bool valid_tiles_count(int tiles_count, bpp_mode bpp)
    {
        if(bpp == bpp_mode::BPP_8)
        {
            return tiles_count && tiles_count < 2048 && (tiles_count % 2) == 0;
        }

        return tiles_count && tiles_count < 1024;
    }

    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_item
     * to avoid dangling references.
     *
     * @param bpp tiles_ref bits per pixel.
     */
    constexpr bg_tiles_item(const span<const tile>& tiles_ref, bpp_mode bpp) :
        _tiles_ref(tiles_ref),
        _bpp(bpp)
    {
        BN_ASSERT(valid_tiles_count(tiles_ref.size(), bpp),
                  "Invalid tiles count: ", _tiles_ref.size(), " - ", int(bpp));
    }

    /**
     * @brief Returns the reference to one or more background tiles.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_item
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
     * @brief Searches for a bg_tiles_ptr which reference the background tiles.
     * @return bg_tiles_ptr which reference the background tiles if they have been found;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_tiles_ptr> find_tiles() const;

    /**
     * @brief Searches for a bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates a bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns a bg_tiles_ptr which reference them.
     */
    [[nodiscard]] bg_tiles_ptr create_tiles() const;

    /**
     * @brief Creates a new bg_tiles_ptr which reference the background tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return bg_tiles_ptr which reference the background tiles.
     */
    [[nodiscard]] bg_tiles_ptr create_new_tiles() const;

    /**
     * @brief Searches for a bg_tiles_ptr which reference the background tiles.
     * If they are not found, it creates a bg_tiles_ptr which reference them.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return bg_tiles_ptr which reference the background tiles if they have been found;
     * otherwise it returns a new bg_tiles_ptr which reference them if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_tiles_ptr> create_tiles_optional() const;

    /**
     * @brief Creates a new bg_tiles_ptr which reference the background tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the bg_tiles_ptr
     * to avoid dangling references.
     *
     * @return bg_tiles_ptr which reference the background tiles if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_tiles_ptr> create_new_tiles_optional() const;

    /**
     * @brief Equal operator.
     * @param a First bg_tiles_item to compare.
     * @param b Second bg_tiles_item to compare.
     * @return `true` if the first bg_tiles_item is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size() &&
                a._bpp == b._bpp;
    }

    /**
     * @brief Not equal operator.
     * @param a First bg_tiles_item to compare.
     * @param b Second bg_tiles_item to compare.
     * @return `true` if the first bg_tiles_item is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles_ref;
    bpp_mode _bpp;
};

}

#endif
