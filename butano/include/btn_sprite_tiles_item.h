/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_TILES_ITEM_H
#define BTN_SPRITE_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional_fwd.h"

namespace btn
{

class sprite_tiles_ptr;

/**
 * @brief Contains the required information to generate sprite tiles.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with <b>"sprite"</b> type.
 *
 * The tiles are not copied but referenced, so they should outlive the sprite_tiles_item to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup tool
 */
class sprite_tiles_item
{

public:
    /**
     * @brief Constructor.
     * @param tiles_ref Reference to one or more sprite tile sets.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
     * to avoid dangling references.
     *
     * @param graphics_count Number of sprite tile sets contained in tiles_ref.
     */
    constexpr sprite_tiles_item(const span<const tile>& tiles_ref, int graphics_count) :
        _tiles_ref(tiles_ref),
        _graphics_count(graphics_count),
        _tiles_count_per_graphic(0)
    {
        BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is empty");
        BTN_ASSERT(graphics_count > 0, "Invalid graphics count: ", graphics_count);
        BTN_ASSERT(graphics_count <= tiles_ref.size(), "Invalid tiles or graphics count: ",
                   tiles_ref.size(), " - ", graphics_count);
        BTN_ASSERT(tiles_ref.size() % graphics_count == 0, "Invalid tiles or graphics count: ",
                   tiles_ref.size(), " - ", graphics_count);

        _tiles_count_per_graphic = tiles_ref.size() / graphics_count;
    }

    /**
     * @brief Returns the number of sprite tile sets contained in tiles_ref.
     */
    [[nodiscard]] constexpr int graphics_count() const
    {
        return _graphics_count;
    }

    /**
     * @brief Returns the number of sprite tiles contained in each sprite tile set.
     */
    [[nodiscard]] constexpr int tiles_count_per_graphic() const
    {
        return _tiles_count_per_graphic;
    }

    /**
     * @brief Returns the reference to one or more sprite tile sets.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    /**
     * @brief Returns the reference to the first sprite tile set.
     */
    [[nodiscard]] constexpr span<const tile> graphics_tiles_ref() const
    {
        return span<const tile>(_tiles_ref.data(), _tiles_count_per_graphic);
    }

    /**
     * @brief Returns the reference to the sprite tile set indicated by graphics_index.
     */
    [[nodiscard]] constexpr span<const tile> graphics_tiles_ref(int graphics_index) const
    {
        BTN_ASSERT(graphics_index >= 0, "Invalid graphics index: ", graphics_index);
        BTN_ASSERT(graphics_index < _graphics_count, "Invalid graphics index: ",
                   graphics_index, " - ", _graphics_count);

        int tiles_count = _tiles_count_per_graphic;
        return span<const tile>(_tiles_ref.data() + (graphics_index * tiles_count), tiles_count);
    }

    /**
     * @brief Searches for a sprite_tiles_ptr which references the first sprite tile set.
     * @return sprite_tiles_ptr which references the first sprite tile set if it has been found;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> find_tiles() const;

    /**
     * @brief Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     * @param graphics_index Index of the tile set to search.
     * @return sprite_tiles_ptr which references the indicated sprite tile set if it has been found;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> find_tiles(int graphics_index) const;

    /**
     * @brief Searches for a sprite_tiles_ptr which references the first sprite tile set.
     * If it is not found, it creates a sprite_tiles_ptr which references it.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @return sprite_tiles_ptr which references the first sprite tile set if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references it.
     */
    [[nodiscard]] sprite_tiles_ptr create_tiles() const;

    /**
     * @brief Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     * If it is not found, it creates a sprite_tiles_ptr which references it.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @param graphics_index Index of the tile set to search or handle.
     * @return sprite_tiles_ptr which references the sprite tile set indicated by graphics_index if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references it.
     */
    [[nodiscard]] sprite_tiles_ptr create_tiles(int graphics_index) const;

    /**
     * @brief Creates a new sprite_tiles_ptr which references the first sprite tile set.
     *
     * The sprite tiles system does not support multiple sprite_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @return sprite_tiles_ptr which references the first sprite tile set.
     */
    [[nodiscard]] sprite_tiles_ptr create_new_tiles() const;

    /**
     * @brief Creates a new sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     *
     * The sprite tiles system does not support multiple sprite_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @param graphics_index Index of the tile set to reference.
     * @return sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     */
    [[nodiscard]] sprite_tiles_ptr create_new_tiles(int graphics_index) const;

    /**
     * @brief Searches for a sprite_tiles_ptr which references the first sprite tile set.
     * If it is not found, it creates a sprite_tiles_ptr which references it.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @return sprite_tiles_ptr which references the first sprite tile set if it has been found;
     * otherwise it returns a new sprite_tiles_ptr which references it if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> create_tiles_optional() const;

    /**
     * @brief Searches for a sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     * If it is not found, it creates a sprite_tiles_ptr which references it.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @param graphics_index Index of the tile set to search or handle.
     * @return sprite_tiles_ptr which references the sprite tile set indicated by graphics_index if it has been found;
     * otherwise it returns a new sprite_tiles_ptr which references it if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> create_tiles_optional(int graphics_index) const;

    /**
     * @brief Creates a new sprite_tiles_ptr which references the first sprite tile set.
     *
     * The sprite tiles system does not support multiple sprite_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @return sprite_tiles_ptr which references the first sprite tile set if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> create_new_tiles_optional() const;

    /**
     * @brief Creates a new sprite_tiles_ptr which references the sprite tile set indicated by graphics_index.
     *
     * The sprite tiles system does not support multiple sprite_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not, you should use create_tiles methods.
     *
     * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr
     * to avoid dangling references.
     *
     * @param graphics_index Index of the tile set to reference.
     * @return sprite_tiles_ptr which references the sprite tile set indicated by graphics_index
     * if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_tiles_ptr> create_new_tiles_optional(int graphics_index) const;

    /**
     * @brief Equal operator.
     * @param a First sprite_tiles_item to compare.
     * @param b Second sprite_tiles_item to compare.
     * @return <b>true</b> if the first sprite_tiles_item is equal to the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size() &&
                a._graphics_count == b._graphics_count;
    }

    /**
     * @brief Not equal operator.
     * @param a First sprite_tiles_item to compare.
     * @param b Second sprite_tiles_item to compare.
     * @return <b>true</b> if the first sprite_tiles_item is not equal to the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator!=(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles_ref;
    int _graphics_count;
    int _tiles_count_per_graphic;
};

}

#endif
