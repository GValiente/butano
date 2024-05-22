/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_MAP_PTR_H
#define BN_REGULAR_BG_MAP_PTR_H

/**
 * @file
 * bn::regular_bg_map_ptr header file.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 */

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_regular_bg_map_cell.h"

namespace bn
{

class size;
class bg_palette_ptr;
class bg_palette_item;
class regular_bg_item;
class regular_bg_map_item;
class regular_bg_tiles_ptr;
class regular_bg_tiles_item;
enum class bpp_mode : uint8_t;
enum class compression_type : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a regular background map.
 *
 * Several regular_bg_map_ptr objects may own the same regular background.
 *
 * The regular background map is released when the last remaining regular_bg_map_ptr owning it is destroyed.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 */
class regular_bg_map_ptr
{

public:
    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * @param map_item regular_bg_map_item which references the map cells to search.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> find(
            const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * @param map_item regular_bg_map_item which references the map cells to search.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @param map_index Index of the referenced map to search.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> find(
            const regular_bg_map_item& map_item, const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette,
            int map_index);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to search.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> find(const regular_bg_item& item);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to search.
     * @param map_index Index of the referenced map to search.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> find(const regular_bg_item& item, int map_index);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] static regular_bg_map_ptr create(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] static regular_bg_map_ptr create(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] static regular_bg_map_ptr create(const regular_bg_item& item);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it.
     */
    [[nodiscard]] static regular_bg_map_ptr create(const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return regular_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static regular_bg_map_ptr create_new(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @param map_index Index of the referenced map to handle.
     * @return regular_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static regular_bg_map_ptr create_new(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to handle.
     * @return regular_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static regular_bg_map_ptr create_new(const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to handle.
     * @param map_index Index of the referenced map to handle.
     * @return regular_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static regular_bg_map_ptr create_new(const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     * @param dimensions Size in map cells of the map to allocate.
     * @param tiles Referenced tiles of the map to allocate.
     * @param palette Referenced color palette of the map to allocate.
     * @return regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     */
    [[nodiscard]] static regular_bg_map_ptr allocate(
            const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_optional(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_optional(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_optional(const regular_bg_item& item);

    /**
     * @brief Searches for a regular_bg_map_ptr which references the given information.
     * If it is not found, it creates a regular_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns a regular_bg_map_ptr which references it if the regular_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_optional(const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return regular_bg_map_ptr which references the given information
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_new_optional(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return regular_bg_map_ptr which references the given information
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_new_optional(
            const regular_bg_map_item& map_item, regular_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to handle.
     * @return regular_bg_map_ptr which references the given information
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_new_optional(const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param item regular_bg_item which references the tiles, the color palette and the map cells to handle.
     * @param map_index Index of the referenced map to handle.
     * @return regular_bg_map_ptr which references the given information
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> create_new_optional(
            const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     * @param dimensions Size in map cells of the map to allocate.
     * @param tiles Referenced tiles of the map to allocate.
     * @param palette Referenced color palette of the map to allocate.
     * @return regular_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen
     * if the regular_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_map_ptr> allocate_optional(
            const size& dimensions, regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Copy constructor.
     * @param other regular_bg_map_ptr to copy.
     */
    regular_bg_map_ptr(const regular_bg_map_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other regular_bg_map_ptr to copy.
     * @return Reference to this.
     */
    regular_bg_map_ptr& operator=(const regular_bg_map_ptr& other);

    /**
     * @brief Move constructor.
     * @param other regular_bg_map_ptr to move.
     */
    regular_bg_map_ptr(regular_bg_map_ptr&& other) noexcept :
        regular_bg_map_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other regular_bg_map_ptr to move.
     * @return Reference to this.
     */
    regular_bg_map_ptr& operator=(regular_bg_map_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced map cells if no more regular_bg_map_ptr objects reference to them.
     */
    ~regular_bg_map_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const;

    /**
     * @brief Returns the size in map cells of the referenced map.
     */
    [[nodiscard]] size dimensions() const;

    /**
     * @brief Indicates if this map is big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] bool big() const;

    /**
     * @brief Returns the bits per pixel of the referenced color palette.
     */
    [[nodiscard]] bpp_mode bpp() const;

    /**
     * @brief Returns how much tiles to offset in the cells of this map before writing them in VRAM.
     */
    [[nodiscard]] int tiles_offset() const;

    /**
     * @brief Returns how much palette banks to offset in the cells of this map before writing them in VRAM.
     */
    [[nodiscard]] int palette_banks_offset() const;

    /**
     * @brief Returns the compression of the referenced map cells.
     */
    [[nodiscard]] compression_type compression() const;

    /**
     * @brief Returns the referenced map cells unless it was created with allocate or allocate_optional.
     * In that case, it returns bn::nullopt.
     */
    [[nodiscard]] optional<span<const regular_bg_map_cell>> cells_ref() const;

    /**
     * @brief Sets the map cells to handle.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to set.
     */
    void set_cells_ref(const regular_bg_map_item& map_item);

    /**
     * @brief Sets the map cells to handle.
     *
     * The map system does not support multiple regular_bg_map_ptr items referencing to the same map cells.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the regular_bg_map_ptr to avoid dangling references.
     *
     * @param map_item regular_bg_map_item which references the map cells to set.
     * @param map_index Index of the referenced map to set.
     */
    void set_cells_ref(const regular_bg_map_item& map_item, int map_index);

    /**
     * @brief Uploads the referenced map cells to VRAM again to make visible the possible changes in them.
     */
    void reload_cells_ref();

    /**
     * @brief Returns the referenced tiles.
     */
    [[nodiscard]] const regular_bg_tiles_ptr& tiles() const;

    /**
     * @brief Sets the referenced tiles.
     * @param tiles regular_bg_tiles_ptr to copy.
     *
     * It must be compatible with the referenced color palette.
     */
    void set_tiles(const regular_bg_tiles_ptr& tiles);

    /**
     * @brief Sets the referenced tiles.
     * @param tiles regular_bg_tiles_ptr to move.
     *
     * It must be compatible with the referenced color palette.
     */
    void set_tiles(regular_bg_tiles_ptr&& tiles);

    /**
     * @brief Replaces the referenced tiles with a new tile set created with the given regular_bg_tiles_item.
     *
     * Before creating a new background tile set, the regular_bg_tiles_ptr referenced by this map is removed,
     * so VRAM usage is reduced.
     *
     * The new background tiles must be compatible with the referenced color palette.
     *
     * @param tiles_item It creates the new background tiles to reference.
     */
    void set_tiles(const regular_bg_tiles_item& tiles_item);

    /**
     * @brief Returns the referenced color palette.
     */
    [[nodiscard]] const bg_palette_ptr& palette() const;

    /**
     * @brief Sets the referenced color palette.
     * @param palette bg_palette_ptr to copy.
     *
     * It must be compatible with the referenced tiles.
     */
    void set_palette(const bg_palette_ptr& palette);

    /**
     * @brief Sets the referenced color palette.
     * @param palette bg_palette_ptr to move.
     *
     * It must be compatible with the referenced tiles.
     */
    void set_palette(bg_palette_ptr&& palette);

    /**
     * @brief Replaces the referenced color palette with a new tile set created with the given bg_palette_item.
     *
     * Before creating a new color palette, the bg_palette_ptr referenced by this map is removed,
     * so VRAM usage is reduced.
     *
     * The new color palette must be compatible with the referenced tiles.
     *
     * @param palette_item It creates the new color palette to reference.
     */
    void set_palette(const bg_palette_item& palette_item);

    /**
     * @brief Sets the tiles and the color palette to reference.
     * @param tiles regular_bg_tiles_ptr to reference.
     * @param palette bg_palette_ptr to reference.
     */
    void set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Replaces the referenced tiles and color palette
     * with the created with the given regular_bg_tiles_item and bg_palette_item.
     *
     * Before creating new resources, the resources referenced by this map are removed,
     * so VRAM usage is reduced.
     *
     * @param tiles_item It creates the new background tiles to reference.
     * @param palette_item It creates the color palette to reference.
     */
    void set_tiles_and_palette(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item);

    /**
     * @brief Returns the allocated memory in VRAM
     * if this regular_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<span<regular_bg_map_cell>> vram();

    /**
     * @brief Returns the internal handle.
     */
    [[nodiscard]] int handle() const
    {
        return _handle;
    }

    /**
     * @brief Exchanges the contents of this regular_bg_map_ptr with those of the other one.
     * @param other regular_bg_map_ptr to exchange the contents with.
     */
    void swap(regular_bg_map_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of a regular_bg_map_ptr with those of another one.
     * @param a First regular_bg_map_ptr to exchange the contents with.
     * @param b Second regular_bg_map_ptr to exchange the contents with.
     */
    friend void swap(regular_bg_map_ptr& a, regular_bg_map_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const regular_bg_map_ptr& a, const regular_bg_map_ptr& b) = default;

private:
    int8_t _handle;

    explicit regular_bg_map_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }
};


/**
 * @brief Hash support for regular_bg_map_ptr.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup functional
 */
template<>
struct hash<regular_bg_map_ptr>
{
    /**
     * @brief Returns the hash of the given regular_bg_map_ptr.
     */
    [[nodiscard]] unsigned operator()(const regular_bg_map_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
